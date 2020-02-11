#ifndef __SD_DAEMON_CONFIG_H__
#define __SD_DAEMON_CONFIG_H__

#include "ConfigProperty.hpp"

namespace SeaDrip
{
    class DaemonConfig : public BaseConfig
    {
    public:
        DaemonConfig( std::string def_cfg_path = "" );
        std::string GetPidPath( void ) const noexcept { return this->m_s_pid_path.Get(); }
        int GetExitSig( void ) const noexcept { return this->m_n_exit_sig.Get(); }
        std::string GetLogPath( void ) const noexcept { return this->m_s_log_path.Get(); }
        std::string GetLogLevel( void ) const noexcept { return this->m_s_log_level.Get(); }
        //  const std::set<ELogLevel>& GetLogForceSave() const noexcept { return this->m_set_force_save; }

    protected:
        virtual bool CfgFileOverride( std::string key, std::string val ) override;

        TConfigProperty<std::string> m_s_pid_path;
        TConfigProperty<int> m_n_exit_sig;
        TConfigProperty<std::string> m_s_log_path;
        TConfigProperty<std::string> m_s_log_level;
        //  std::set<ELogLevel> m_set_force_save;
    };

    class SocketDaemonConfig : public DaemonConfig
    {
    public:
        SocketDaemonConfig( std::string def_cfg_path = "", int def_listen_port = 0 );
        int GetListenPort() const noexcept { return this->m_n_listen_port.Get(); }
        in_addr_t GetSockAddr() const noexcept { return this->m_n_sock_addr.Get(); }

    protected:
        virtual bool CfgFileOverride( std::string key, std::string val ) override;

        TConfigProperty<int> m_n_listen_port;
        TConfigProperty<in_addr_t> m_n_sock_addr;
    };
};

#endif
