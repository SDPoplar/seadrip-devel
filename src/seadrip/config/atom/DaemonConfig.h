#ifndef __SD_CONF_ATOM_DAEMON_H__
#define __SD_CONF_ATOM_DAEMON_H__

#include "../base/ConfigProperty.hpp"

namespace SeaDrip
{
    class DaemonConfig
    {
    public:
        DaemonConfig( const std::string& def_cfg_path, const std::string& def_pid_path );
        const std::string GetPidPath( void ) const noexcept;
        DaemonConfig& SetPidPath( const std::string& pid_path, EConfigSetFrom from = EConfigSetFrom::RUNTIME );
        const int GetExitSig( void ) const noexcept;
        DaemonConfig& SetExitSig( const int sig, EConfigSetFrom from = EConfigSetFrom::RUNTIME );

        //  std::string GetLogPath( void ) const noexcept;
        //  std::string GetLogLevel( void ) const noexcept;
        //  const std::set<ELogLevel>& GetLogForceSave() const noexcept { return this->m_set_force_save; }

    protected:
        TConfigProperty<std::string> m_s_pid_path;
        TConfigProperty<int> m_n_exit_sig;
        //  TConfigProperty<std::string> m_s_log_path;
        //  TConfigProperty<std::string> m_s_log_level;
        //  std::set<ELogLevel> m_set_force_save;
    };
};

#endif
