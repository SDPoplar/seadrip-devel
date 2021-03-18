#include "../seadrip/component/SocketComponent.h"
#include <sys/socket.h>
// #include <unistd.h>
#include <fcntl.h>
using namespace SeaDrip;

InetSocketComponent::InetSocketComponent( const bool autoCreate ) : SocketComponent()
{
    autoCreate && this->Create();
}

const bool InetSocketComponent::Create( const bool asShared )
{
    int sock = socket( AF_INET, SOCK_STREAM, 0 );
    if( sock == -1 )
    {
        return false;
    }

    if( asShared )
    {
        fcntl( sock, F_SETFL, O_NONBLOCK );
    }

    this->m_n_socket = sock;
    return true;
}

const bool InetSocketComponent::BindAndListen( in_port_t port )
{
    sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = port;
    return true
        && ( bind( this->m_n_socket, (const sockaddr*)&addr, sizeof(addr) ) != -1 )
        && ( listen( this->m_n_socket, 5 ) != -1 )
        && true;
}
