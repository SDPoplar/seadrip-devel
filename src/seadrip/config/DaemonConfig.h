#ifndef __SD_DAEMON_CONFIG_H__
#define __SD_DAEMON_CONFIG_H__

//  #include "ConfigProperty.hpp"
#include "BaseConfig.h"

namespace SeaDrip
{
    class DaemonConfig : public BaseConfig
    {
    public:
        DaemonConfig( std::string def_cfg_path, int argc, char** argv );
        std::string GetPidPath( void ) const noexcept;
        int GetExitSig( void ) const noexcept;
        std::string GetLogPath( void ) const noexcept;
        std::string GetLogLevel( void ) const noexcept;
        //  const std::set<ELogLevel>& GetLogForceSave() const noexcept { return this->m_set_force_save; }

    protected:
        virtual ShellInput* RegistShellItem( ShellInput* catcher ) override;
        virtual bool ShellOverride( std::string key, std::string val ) override;
        virtual bool CfgFileOverride( std::string key, std::string val ) override;

        TConfigProperty<std::string> m_s_pid_path;
        TConfigProperty<int> m_n_exit_sig;
        TConfigProperty<std::string> m_s_log_path;
        TConfigProperty<std::string> m_s_log_level;
        //  std::set<ELogLevel> m_set_force_save;
    };
};

#endif
