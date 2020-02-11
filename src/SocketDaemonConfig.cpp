#include "seadrip/SocketDaemonConfig.h"
using namespace SeaDrip;

//  =====================   SocketDaemonConfig  ============================================

SocketDaemonConfig::SocketDaemonConfig( std::string def_cfg_path, unsigned int def_listen_port )
    : DaemonConfig( def_cfg_path ), /*m_n_sock_addr( INADDR_ANY ),*/ m_u_listen_port( def_listen_port )
{}

bool SocketDaemonConfig::CfgFileOverride( std::string key, std::string val )
{
    if( DaemonConfig::CfgFileOverride( key, val ) )
    {
        return true;
    }
    /*
    if( key == "sock_addr" )
    {
        //  translate val to in_addr_t
        this->m_n_sock_addr.Set( EConfigSetFrom::CFGFILE, INADDR_ANY );
        return true;
    }
    */
    if( key == "listen" )
    {
        this->m_u_listen_port.Set( EConfigSetFrom::CFGFILE, atoi( val.c_str() ) );
        return true;
    }
    return false;
}

unsigned int SocketDaemonConfig::GetListenPort() const noexcept
{
    return this->m_u_listen_port.Get();
}

unsigned int SocketDaemonConfig::GetProcessNum() const noexcept
{
    return this->m_u_process_num.Get();
}

/*
in_addr_t SocketDaemonConfig::GetSockAddr() const noexcept
{
    return this->m_n_sock_addr.Get();
}
*/
