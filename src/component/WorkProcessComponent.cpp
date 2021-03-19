#include "../seadrip/component/WorkProcessComponent.h"
#include <signal.h>
#include <unistd.h>
#include <string>
using namespace SeaDrip;

// ============================ WorkProcessNoticePack =======================================

WorkProcessNoticePack::WorkProcessNoticePack( const int op, const sigval data )
    : m_n_op( op ), m_union_data( data )
{}

const int WorkProcessNoticePack::GetOption() const noexcept
{
    return this->m_n_op;
}

const sigval WorkProcessNoticePack::GetData() const noexcept
{
    return this->m_union_data;
}

const int WorkProcessNoticePack::GetDataAsInt() const noexcept
{
    return this->m_union_data.sival_int;
}
const void* WorkProcessNoticePack::GetDataAsPtr() const noexcept
{
    return this->m_union_data.sival_ptr;
}

// ============================= WorkProcessClient ===========================================

WorkProcessClient::WorkProcessClient( const int parentPid ) : m_n_parent_pid( parentPid ), m_b_client_running( false )
{}

const int WorkProcessClient::GetParentPid() const noexcept
{
    return this->m_n_parent_pid;
}

const bool WorkProcessClient::ClientRunning() const noexcept
{
    return this->m_b_client_running;
}

const bool WorkProcessClient::Init( void(*handler)(int, siginfo_t*, void*), const std::map<int, WorkProcessClientEventProc> *workmap )
{
    struct sigaction action;
    action.sa_sigaction = handler;
    action.sa_flags = SA_SIGINFO;
    if( sigaction( SIGUSR1, &action, nullptr ) == -1 )
    {
        return false;
    }
    this->m_b_client_running = true;
    this->m_p_method_dict = workmap;
    return this->Ready();
}

void WorkProcessClient::Stop()
{
    this->m_b_client_running = false;
}

void WorkProcessClient::Work( WorkProcessNoticePack* event )
{
    auto method = this->m_p_method_dict->find( event->GetOption() );
    if( method != this->m_p_method_dict->end() )
    {
        auto func = method->second;
        func( this, event->GetData() );
    }
    delete event;
}

const bool WorkProcessClient::Ready()
{
    return this->ReportEvent( static_cast<int>(WorkProcessEvents::READY), {0} );
}

const bool WorkProcessClient::ReportEvent( const int op, const sigval data )
{
    sigval sigdata;
    sigdata.sival_ptr = new WorkProcessNoticePack( op, data );
    return sigqueue( this->GetParentPid(), SIGUSR1, sigdata );
}

// ============================= WorkProcessComponent ========================================

WorkProcessComponent::~WorkProcessComponent()
{
    for( auto item : this->m_map_worker )
    {
        this->NoticeWorker( item.first, static_cast<int>( WorkProcessOptions::STOP ), {0} );
    }
}

std::string makePath( const char* path )
{
    if( path[ 0 ] == '/' )
    {
        return std::string( path );
    }
    char buff[ 256 ];
    readlink( path, buff, 256 );
    std::string ret = buff;
    return ret + path;
}

bool WorkProcessComponent::Fork( const char* worker, const char* withUser, const int workerNum )
{
    auto path = makePath( worker );
    char pidstr[ 32 ] = "";
    sprintf( pidstr, "%d", getpid() );
    for( int i=0; i<workerNum; i++ )
    {
        int forkedPid = fork();
        if( forkedPid == -1 )
        {
            return false;
        }
        if( forkedPid > 0 )
        {
            this->m_map_worker[ forkedPid ] = WorkProcessStatus::CREATE;
            continue;
        }
        execl( path.c_str(), pidstr );
    }
    return true;
}

int WorkProcessComponent::PopFreeWorker()
{
    if( this->m_queue_free.empty() )
    {
        return 0;
    }
    int ret = this->m_queue_free.at( 0 );
    this->m_queue_free.erase( this->m_queue_free.begin() );
    return ret;
}

bool WorkProcessComponent::NoticeFreeWorker( const int op, const sigval data )
{
    int worker = this->PopFreeWorker();
    return ( worker > 0 ) && this->NoticeWorker( worker, op, data );
}

bool WorkProcessComponent::NoticeWorker( const int workerPid, const int op, const sigval data )
{
    sigval d;
    d.sival_ptr = new WorkProcessNoticePack( op, data );
    return sigqueue( workerPid, SIGUSR1, d ) != -1;
}