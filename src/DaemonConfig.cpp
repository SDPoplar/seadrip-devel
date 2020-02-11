#include "seadrip/DaemonConfig.h"
#include "seadrip/LinuxSigMap.h"
#include <boost/algorithm/string.hpp>
using namespace SeaDrip;

//  ================    DaemonConfig    =================================================

DaemonConfig::DaemonConfig( std::string def_cfg_path ) : BaseConfig( def_cfg_path ),
    m_s_pid_path( "" ), m_n_exit_sig( SIGUSR2 ), m_s_log_path( "" ), m_s_log_level( "error" )
{}

bool DaemonConfig::CfgFileOverride( std::string key, std::string val )
{
    if( key == "pid" )
    {
        this->m_s_pid_path.Set( EConfigSetFrom::CFGFILE, val );
        return true;
    }
    if( key == "exit_sig" )
    {
        std::string upperval = boost::to_upper_copy( val );
        auto sigfound = linux_sig_map.find( upperval );
        int nsig = (sigfound != linux_sig_map.end()) ? sigfound->second : atoi( val.c_str() );
        this->m_n_exit_sig.Set( EConfigSetFrom::CFGFILE, nsig );
        return true;
    }
    if( key == "log" )
    {
        this->m_s_log_path.Set( EConfigSetFrom::CFGFILE, val );
        return true;
    }
    if( key == "log_level" )
    {
        this->m_s_log_level.Set( EConfigSetFrom::CFGFILE, val );
        return true;
    }
    return false;
}

//  =====================   SocketDaemonConfig  ============================================

SocketDaemonConfig::SocketDaemonConfig( std::string def_cfg_path, int def_listen_port )
    : DaemonConfig( def_cfg_path ), m_n_sock_addr( INADDR_ANY ), m_n_listen_port( def_listen_port )
{}

bool SocketDaemonConfig::CfgFileOverride( std::string key, std::string val )
{
    if( DaemonConfig::CfgFileOverride( key, val ) )
    {
        return true;
    }
    if( key == "sock_addr" )
    {
        //  translate val to in_addr_t
        this->m_n_sock_addr.Set( EConfigSetFrom::CFGFILE, INADDR_ANY );
        return true;
    }
    if( key == "listen" )
    {
        this->m_n_listen_port.Set( EConfigSetFrom::CFGFILE, atoi( val.c_str() ) );
        return true;
    }
    return false;
}
