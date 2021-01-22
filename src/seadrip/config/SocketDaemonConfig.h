#ifndef     __SD_SOCKET_DAEMON_CONFIG_H__
#define     __SD_SOCKET_DAEMON_CONFIG_H__

#include "DaemonConfig.h"

namespace SeaDrip
{
    class SocketConfig
    {
    public:
        SocketConfig( int port, std::string addr = "" );

        const std::string GetListenAddr( void ) const noexcept;
        SocketConfig &SetListenAddr( const std::string& addr, EConfigSetFrom from = EConfigSetFrom::RUNTIME );
        const int GetListenPort( void ) const noexcept;
        SocketConfig &SetListenPort( const int port, EConfigSetFrom from = EConfigSetFrom::RUNTIME );
    
    protected:
        SeaDrip::TConfigProperty<std::string> m_s_listen_addr;
        SeaDrip::TConfigProperty<int> m_n_listen_port;
    };

    class SocketDaemonConfig : public DaemonConfig, public SocketConfig
    {
    public:
        SocketDaemonConfig( std::string def_cfg_path, std::string def_pid_path, int def_port, std::string addr = "" );
    };
}

#endif