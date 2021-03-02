#include "../seadrip/config/atom/SocketConfig.h"
using namespace SeaDrip;

SocketConfig::SocketConfig( int port, std::string addr )
    : m_s_listen_addr( addr ), m_n_listen_port( port )
{}

const int SocketConfig::GetListenPort() const noexcept
{
    return this->m_n_listen_port.Get();
}

SocketConfig &SocketConfig::SetListenPort( const int port, const EConfigSetFrom from )
{
    this->m_n_listen_port.Set( from, port );
    return *this;
}

const std::string SocketConfig::GetListenAddr( void ) const noexcept
{
    return this->m_s_listen_addr.Get();
}

SocketConfig &SocketConfig::SetListenAddr( const std::string &listen_addr, const EConfigSetFrom from )
{
    this->m_s_listen_addr.Set( from, listen_addr );
    return *this;
}
