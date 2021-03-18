#include "../seadrip/component/WorkProcessComponent.h"
#include <signal.h>
#include <unistd.h>
#include <string>
using namespace SeaDrip;

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