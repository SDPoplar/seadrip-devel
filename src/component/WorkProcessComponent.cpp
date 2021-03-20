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

WorkProcessEventPack::WorkProcessEventPack( const int event, sigval data )
    : WorkProcessNoticePack( event, data )
{
    this->m_n_pid = getpid();
}

const int WorkProcessEventPack::FromProcess() const noexcept
{
    return this->m_n_pid;
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
    return this->ReportReady();
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

const bool WorkProcessClient::ReportReady()
{
    return this->ReportEvent( static_cast<int>(WorkProcessEvents::READY), {0} );
}

const bool WorkProcessClient::ReportError( const char* message )
{
    sigval msg;
    msg.sival_ptr = ( void* )message;
    return this->ReportEvent( static_cast<int>( WorkProcessEvents::ERROR ), msg );
}

const bool WorkProcessClient::ReportEvent( const int op, const sigval data )
{
    sigval sigdata;
    sigdata.sival_ptr = new WorkProcessEventPack( op, data );
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

WorkProcessComponent* forked = nullptr;

void WorkProcessEventHandler(int, siginfo_t* info, void* data )
{
    if( forked )
    {
        forked->OnEvent( (WorkProcessEventPack*)data );
    }
}

bool WorkProcessComponent::Fork( const char* worker, const char* withUser, const int workerNum )
{
    forked = this;
    struct sigaction sighandler;
    sighandler.sa_sigaction = WorkProcessEventHandler;
    sighandler.sa_flags = SA_SIGINFO;
    if( sigaction( SIGUSR1, &sighandler, nullptr ) == -1 )
    {
        return false;
    }
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

void WorkProcessComponent::OnEvent( WorkProcessEventPack* event )
{
    switch (event->GetOption())
    {
    case static_cast<int>(WorkProcessEvents::READY):
        this->OnWorkProcessReady(event->FromProcess());
        break;
    case static_cast<int>(WorkProcessEvents::ERROR):
        // save log or fix it
        break;
    default:
        break;
    }
    delete event;
}

void WorkProcessComponent::OnWorkProcessReady( const int processPid )
{
    auto found = this->m_map_worker.find(processPid);
    if( found != this->m_map_worker.end() )
    {
        found->second = WorkProcessStatus::READY;
        this->m_queue_free.push_back(processPid);
    }
 
}
