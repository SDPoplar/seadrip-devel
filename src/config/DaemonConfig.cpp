#include "seadrip/config/DaemonConfig.h"
#include "seadrip/LinuxSigMap.h"
#include <boost/algorithm/string.hpp>
using namespace SeaDrip;

//  ================    DaemonConfig    =================================================

DaemonConfig::DaemonConfig( const std::string& def_cfg_path, const std::string& def_pid_path )
    : BaseConfig( def_cfg_path ), m_s_pid_path( def_pid_path ), m_n_exit_sig( SIGUSR2 )
    //  m_s_log_path( "" ), m_s_log_level( "error" )
{}

const std::string DaemonConfig::GetPidPath( void ) const noexcept
{
    return this->m_s_pid_path.Get();
}

DaemonConfig& DaemonConfig::SetPidPath( const std::string& pid_path, EConfigSetFrom from )
{
    this->m_s_pid_path.Set( from, pid_path );
    return *this;
}

const int DaemonConfig::GetExitSig( void ) const noexcept
{
    return this->m_n_exit_sig.Get();
}

DaemonConfig& DaemonConfig::SetExitSig( const int sig, EConfigSetFrom from )
{
    this->m_n_exit_sig.Set( from, sig );
    return *this;
}

/*
std::string DaemonConfig::GetLogPath( void ) const noexcept
{
    return this->m_s_log_path.Get();
}

std::string DaemonConfig::GetLogLevel( void ) const noexcept
{
    return this->m_s_log_level.Get();
}
*/
