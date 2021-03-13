#ifndef __SD_COMPONENT_ATOM_SOCKET_H__
#define __SD_COMPONENT_ATOM_SOCKET_H__

#include <netinet/in.h>

namespace SeaDrip
{
    class SocketComponent
    {
    public:
        SocketComponent();

        const int GetSocket() const noexcept;

    protected:
        int m_n_socket;
    };

    class InetSocketComponent : public SocketComponent
    {
    public:
        InetSocketComponent( const bool autoCreate = false );
        const bool Create( const bool asShared = false );
        const bool BindAndListen( in_port_t port );
    };
};

#endif
