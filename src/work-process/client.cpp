#include "../seadrip/work-process/Client.h"
using namespace SeaDrip::WorkProcess;

Client::Client( const int parent, const int in_sock, const int out_sock )
    : SockPair( in_sock, out_sock ),
    m_n_parent_pid( parent ), m_b_running( false )
{}

const bool Client::IsClientRunning() const noexcept
{
    return this->m_b_running;
}

const bool Client::WellLoaded() const noexcept
{
    return true
        // && all listener
        && true;
}

int Client::Listen()
{
    this->m_b_running = this->WellLoaded();
    while( this->IsClientRunning() )
    {
        /*
        if( ) // get task failed
        {
            continue;
        }
        */
    }

    return 0;
}
