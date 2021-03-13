#include "../seadrip/component/SocketComponent.h"
using namespace SeaDrip;

SocketComponent::SocketComponent() : m_n_socket( 0 )
{}

const int SocketComponent::GetSocket() const noexcept
{
    return this->m_n_socket;
}
