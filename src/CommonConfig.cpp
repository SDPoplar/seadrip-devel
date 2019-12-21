#include "seadrip/ConfigProperty.hpp"
#if defined( linux ) or defined( __GNUC__ )
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#endif
#include "seadrip/LinuxSigMap.h"
#include "seadrip/KvFileReader.hpp"
#include <iostream>
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
    if( this->m_s_config_file.Get().empty() )
    {
        std::cout << "No config file given" << std::endl;
        return;
    }
    KvFileReader cfg( this->m_s_config_file.Get(), '#' );
    if( !cfg )
    {
        std::cout << "Cannot open config file: " << this->m_s_config_file.Get() << std::endl;
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

std::string BaseConfig::DefCfgPath() const noexcept
{
    return "";
}

//  ================    DaemonConfig    =================================================

DaemonConfig::DaemonConfig() : BaseConfig(), m_s_pid_path( "" ), m_n_exit_sig( SIGUSR2 ),
    m_s_log_path( "" ), m_e_log_level( ELogLevel::Error ), m_set_force_save( {} )
{}

bool TransLogLevel( std::string cfg, ELogLevel& level )
{
    const std::map<std::string, ELogLevel> lm = {
        std::make_pair( "DEBUG", ELogLevel::Debug ),
        std::make_pair( "ERROR", ELogLevel::Error ),
        std::make_pair( "WARN", ELogLevel::Warn ),
        std::make_pair( "NONE", ELogLevel::None ),
        std::make_pair( "INFO", ELogLevel::Info )
    };
    std::string target = boost::to_upper_copy( cfg );
    auto cursor = lm.find( target );
    if( cursor != lm.end() )
    {
        level = cursor->second;
        return true;
    }
    return false;
}

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
        this->m_s_config_file.Set( EConfigSetFrom::CFGFILE, val );
        return true;
    }
    if( key == "log_level" )
    {
        ELogLevel level;
        if( TransLogLevel( val, level ) )
        {
            this->m_e_log_level = level;
            return true;
        }
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

int SocketDaemonConfig::DefListenPort() const noexcept
{
    return 0;
}

