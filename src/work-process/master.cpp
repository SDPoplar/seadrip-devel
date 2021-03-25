#include "../seadrip/work-process/Master.h"
using namespace SeaDrip::WorkProcess;

#include <unistd.h>
#include <sys/socket.h>

// ===================== Worker ==================================================================

Worker::Worker( const int in_sock, const int out_sock )
    : m_e_status( ClientStatus::CREATE ), SockPair( in_sock, out_sock )
{}

Worker::Worker( const Worker& copy ) : SockPair( copy )
{
    this->m_e_status = copy.m_e_status;
}

// ===================== Master ==================================================================

const bool Master::Fork( const char* worker, const char* useAccount, const int workerNum )
{
    char selfpid[ 16 ] = "";
    sprintf( selfpid, "%d", getpid() );
    for( int i=0; i<workerNum; i++ )
    {
        int socks[ 2 ];
        if( socketpair( AF_UNIX, SOCK_STREAM, 0, socks ) == -1 )
        {
            return false;
        }
        int pid = fork();
        if( pid == -1 )
        {
            return false;
        }

        if( pid > 0 )
        {
            this->m_map_clients[ pid ] = Worker( socks[ 0 ], socks[ 1 ] );
            continue;
        }

        char in_sock[ 16 ] = "", out_sock[ 16 ] = "";
        sprintf( in_sock, "%d", socks[ 0 ] );
        sprintf( out_sock, "%d", socks[ 1 ] );
        if( execl( worker, worker, selfpid, out_sock, in_sock ) == -1 )
        {
            return false;
        }
    }
    return true;
}

bool Master::SetClientReady( const int clientPid )
{
    auto worker = this->FindWorkerByPid( clientPid );
    if( !worker )
    {
        return false;
    }

    *worker = ClientStatus::READY;
    this->m_queue_free.push_back( worker );
    return true;
}

Worker* Master::PopFreeClient()
{
    if( this->m_queue_free.empty() )
    {
        return nullptr;
    }
    auto ret = this->m_queue_free.at( 0 );
    this->m_queue_free.erase( this->m_queue_free.begin() );
    return ret;
}

Worker* Master::FindWorkerByPid( const int pid )
{
    auto worker = this->m_map_clients.find( pid );
    return ( worker == this->m_map_clients.end() ) ? nullptr : &(worker->second);
}
