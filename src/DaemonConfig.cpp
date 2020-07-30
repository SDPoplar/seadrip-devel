#include "seadrip/config/DaemonConfig.h"
#include "seadrip/LinuxSigMap.h"
#include <boost/algorithm/string.hpp>
using namespace SeaDrip;

//  ================    DaemonConfig    =================================================

DaemonConfig::DaemonConfig( std::string def_cfg_path, int argc, char** argv ) : BaseConfig( def_cfg_path, argc, argv ),
    m_s_pid_path( "" ), m_n_exit_sig( SIGUSR2 ), m_s_log_path( "" ), m_s_log_level( "error" )
{}

ShellInput* DaemonConfig::RegistShellItem( ShellInput* catcher )
{
    return BaseConfig::RegistShellItem( catcher );
}

bool DaemonConfig::ShellOverride( std::string key, std::string val )
{
    return BaseConfig::ShellOverride( key, val );
}

bool DaemonConfig::CfgFileOverride( std::string key, std::string val )
{
    if( BaseConfig::CfgFileOverride( key, val ) )
    {
        return true;
    }
    CONFIG_OVERRIDE_FROM_CFGFILE( "pid", this->m_s_pid_path );
    if( key == "exit_sig" )
    {
        std::string upperval = boost::to_upper_copy( val );
        auto sigfound = linux_sig_map.find( upperval );
        int nsig = (sigfound != linux_sig_map.end()) ? sigfound->second : atoi( val.c_str() );
        this->m_n_exit_sig.Set( EConfigSetFrom::CFGFILE, nsig );
        return true;
    }
    CONFIG_OVERRIDE_FROM_CFGFILE( "log", this->m_s_log_path );
    CONFIG_OVERRIDE_FROM_CFGFILE( "log_level", this->m_s_log_level );
    return false;
}

std::string DaemonConfig::GetPidPath( void ) const noexcept
{
    return this->m_s_pid_path.Get();
}

int DaemonConfig::GetExitSig( void ) const noexcept
{
    return this->m_n_exit_sig.Get();
}

std::string DaemonConfig::GetLogPath( void ) const noexcept
{
    return this->m_s_log_path.Get();
}

std::string DaemonConfig::GetLogLevel( void ) const noexcept
{
    return this->m_s_log_level.Get();
}
