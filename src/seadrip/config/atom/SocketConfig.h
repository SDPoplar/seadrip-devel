#ifndef     __SD_CONF_ATOM_SOCKET_H__
#define     __SD_CONF_ATOM_SOCKET_H__

#include "../base/ConfigProperty.hpp"

namespace SeaDrip
{
    class SocketConfig
    {
    public:
        SocketConfig( int port, std::string addr = "" );

        const std::string GetListenAddr( void ) const noexcept;
        SocketConfig &SetListenAddr( const std::string& addr, const EConfigSetFrom from = EConfigSetFrom::RUNTIME );
        const int GetListenPort( void ) const noexcept;
        SocketConfig &SetListenPort( const int port, const EConfigSetFrom from = EConfigSetFrom::RUNTIME );
    
    protected:
        SeaDrip::TConfigProperty<std::string> m_s_listen_addr;
        SeaDrip::TConfigProperty<int> m_n_listen_port;
    };
}

#endif
