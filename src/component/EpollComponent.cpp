#include "../seadrip/component/EpollComponent.h"
using namespace SeaDrip;

EpollComponent::EpollComponent() : m_n_poll_fd( 0 )
{}

const int EpollComponent::GetPollFd() const noexcept
{
    return this->m_n_poll_fd;
}
