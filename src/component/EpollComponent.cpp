#include "../seadrip/component/EpollComponent.h"
#include <unistd.h>
#include <errno.h>
using namespace SeaDrip;

EpollComponent::EpollComponent() : m_n_poll_fd( 0 ), m_p_events( nullptr ), m_n_event_num( 0 )
{}

/*
EpollComponent::EpollComponent( const EpollComponent& copy )
{
    if( copy.m_n_event_num > 0 )
    {
        this->m_n_event_num = copy.m_n_event_num;
        this->m_p_events = new epoll_event[ copy.m_n_event_num ];
    }
    this->m_n_poll_fd = copy.m_n_poll_fd;
}
*/

EpollComponent::~EpollComponent()
{
    if( this->m_p_events )
    {
        delete [] this->m_p_events;
        this->m_p_events = nullptr;
        this->m_n_event_num = 0;
    }

    //if( is_open( this->m_n_poll_fd ) ) { close(this->m_n_poll_fd); }
    if( this->m_n_poll_fd > 0 )
    {
        close( this->m_n_poll_fd );
        this->m_n_poll_fd = 0;
    }
}

EpollComponent& EpollComponent::SetEpollFd( int fd )
{
    this->m_n_poll_fd = fd;
    return *this;
}

const int EpollComponent::GetPollFd() const noexcept
{
    return this->m_n_poll_fd;
}

bool EpollComponent::Create( int nMaxEvents, void(*InitEventProc)(epoll_event*, void*), void* res )
{
    int fd = epoll_create( nMaxEvents );
    if( !this->checkBoolReturn( fd ) )
    {
        return false;
    }
    this->m_n_poll_fd = fd;
    this->m_n_event_num = nMaxEvents;
    this->m_p_events = new epoll_event[ nMaxEvents ];
    for( int i=0; i<nMaxEvents; i++ )
    {
        InitEventProc( this->m_p_events + i, res );
    }
    return true;
}

bool EpollComponent::BindEvent( const int fd )
{
    return this->checkBoolReturn( epoll_ctl( this->m_n_poll_fd, EPOLL_CTL_ADD, fd, this->m_p_events ) );
}

std::vector<epoll_event*> EpollComponent::Listen( const int timeout, const bool bBlock )
{
    std::vector<epoll_event*> ret;
    int num = epoll_wait( this->m_n_poll_fd, this->m_p_events, this->m_n_event_num, timeout );
    for( int i=0; i<num;i++ )
    {
        ret.push_back( this->m_p_events + i );
    }
    return ret;
}

const int EpollComponent::GetLastError() const noexcept
{
    return this->m_n_last_error;
}

// ========================= protected methods ========================================
const bool EpollComponent::checkBoolReturn( const int ret_val, const int err )
{
    if( ret_val != err )
    {
        return true;
    }
    this->m_n_last_error = errno;
    return false;
}
