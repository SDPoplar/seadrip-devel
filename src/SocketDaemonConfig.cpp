#include "seadrip/config/SocketDaemonConfig.h"
using namespace SeaDrip;

//  ========================    SocketConfig    ============================================
SocketConfig::SocketConfig( int port, std::string addr )
    : m_s_listen_addr( addr ), m_n_listen_port( port )
{}

const int SocketConfig::GetListenPort() const noexcept
{
    return this->m_n_listen_port.Get();
}

SocketConfig &SocketConfig::SetListenPort( int port, EConfigSetFrom from )
{
    this->m_n_listen_port.Set( from, port );
    return *this;
}

const std::string SocketConfig::GetListenAddr( void ) const noexcept
{
    return this->m_s_listen_addr.Get();
}

SocketConfig &SocketConfig::SetListenAddr( const std::string &listen_addr, EConfigSetFrom from )
{
    this->m_s_listen_addr.Set( from, listen_addr );
    return *this;
}

//  =====================   SocketDaemonConfig  ============================================

SocketDaemonConfig::SocketDaemonConfig( std::string def_cfg_path, std::string def_pid_path, int def_listen_port, std::string def_listen_addr )
    : DaemonConfig( def_cfg_path, def_pid_path ), SocketConfig( def_listen_port, def_listen_addr )
{}
