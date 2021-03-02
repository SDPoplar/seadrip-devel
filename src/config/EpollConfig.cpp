#include "../seadrip/config/atom/EpollConfig.h"

using namespace SeaDrip;

EpollConfig::EpollConfig( int maxEventNum ) : m_n_epoll_max_event( maxEventNum )
{}

const int EpollConfig::GetMaxEvent() const noexcept
{
    return this->m_n_epoll_max_event.Get();
}

EpollConfig &EpollConfig::SetMaxEvent( const int maxEventNum, const EConfigSetFrom from )
{
    this->m_n_epoll_max_event.Set( from, maxEventNum );
    return *this;
}
