#include "seadrip/ConfigProperty.hpp"
#if defined( linux ) or defined( __GNUC__ )
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#endif
#include "seadrip/LinuxSigMap.h"
#include "seadrip/KvFileReader.hpp"
using namespace SeaDrip;

//  ==================  BaseConfig  ============================================

BaseConfig::BaseConfig()
{
    this->m_s_config_file.Set( EConfigSetFrom::DEF, this->DefCfgPath() );
}

void BaseConfig::Init( int argc, char** argv )
{
#if defined( linux ) or defined( __GNUC__ )
    char ch;
    while( (ch = getopt( argc, argv, this->m_s_shell_options.c_str() )) != -1 )
    {
        if( ch == 'c' )
        {
            this->m_s_config_file.Set( EConfigSetFrom::SHELL, optarg );
            continue;
        }
        auto prop = this->m_map_bool_props.find( ch );
        if( prop != this->m_map_bool_props.end() )
        {
            prop->second->Set( EConfigSetFrom::SHELL, true );
        }
        else
        {
            this->ShellOverride( ch, optarg );
        }
    }
#endif
    KvFileReader cfg( this->m_s_config_file.Get(), '#' );
    if( !cfg )
    {
        return;
    }
    std::string key, val;
    while( cfg.Next( key, val ) )
    {
        if( val.empty() )
        {
            continue;
        }
        if( !this->CfgFileOverride( key, val ) )
        {
            std::cout << "Unknown config item: " << key << std::endl;
        }
    }
}

//  ================    DaemonConfig    =================================================

DaemonConfig::DaemonConfig() : BaseConfig(), m_s_pid_path( "" ), m_n_exit_sig( SIGUSR2 ),
    m_s_log_path( "" ), m_e_log_level( ELogLevel::Error ), m_set_force_save( {} )
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
    return false;
}

//  =====================   SocketDaemonConfig  ============================================

SocketDaemonConfig::SocketDaemonConfig() : DaemonConfig(), m_n_sock_addr( INADDR_ANY )
{
    this->m_n_listen_port.Set( EConfigSetFrom::DEF, this->DefListenPort() );
}

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
