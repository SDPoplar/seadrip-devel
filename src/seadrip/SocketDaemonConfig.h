#ifndef __SD_SOCKET_DAEMON_CONFIG_H__
#define __SD_SOCKET_DAEMON_CONFIG_H__

#include "DaemonConfig.h"

namespace SeaDrip
{
    class SocketDaemonConfig : public DaemonConfig
    {
    public:
        SocketDaemonConfig( std::string def_cfg_path = "", unsigned int def_listen_port = 0 );
        unsigned int GetListenPort() const noexcept;
        unsigned int GetProcessNum() const noexcept;

        //  in_addr_t GetSockAddr() const noexcept { return this->m_n_sock_addr.Get(); }

    protected:
        virtual bool CfgFileOverride( std::string key, std::string val ) override;

        TConfigProperty<unsigned int> m_u_listen_port;
        TConfigProperty<unsigned int> m_u_process_num;
        //  TConfigProperty<in_addr_t> m_n_sock_addr;
    };
}

#endif
