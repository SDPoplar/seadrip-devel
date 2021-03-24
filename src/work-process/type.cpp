#include "../seadrip/work-process/Type.h"
using namespace SeaDrip::WorkProcess;

#include <unistd.h>

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
