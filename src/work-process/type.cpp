#include "../seadrip/work-process/Type.h"
using namespace SeaDrip::WorkProcess;

#include <unistd.h>

// ===================================================== Task =====================================================

const Task Task::QuitTask = Task( Task::OPTION_QUIT );

Task::Task( const int option ) : m_n_option( option )
{}

const int Task::GetOption() const noexcept
{
    return this->m_n_option;
}

// ===================================================== Event ====================================================

const Event Event::ErrorEvent = Event( Event::EVENT_ERROR );

Event::Event( const int event ) : m_n_event( event )
{
    this->m_n_client_pid = getpid();
}

const int Event::GetClient() const noexcept
{
    return this->m_n_client_pid;
}

const int Event::GetEvent() const noexcept
{
    return this->m_n_event;
}

// ===================================================== SockPair =================================================

SockPair::SockPair( const int in_sock, const int out_sock )
    : m_sock_in( in_sock ), m_sock_out( out_sock )
{
    close( in_sock );
}

SockPair::SockPair( const SockPair& copy )
{
    this->m_sock_in = copy.m_sock_in;
    this->m_sock_out = copy.m_sock_out;
}
