#ifndef __SD_COMPONENT_ATOM_EPOLL_H__
#define __SD_COMPONENT_ATOM_EPOLL_H__

#include <sys/epoll.h>
#include <vector>

namespace SeaDrip
{
    class EpollComponent
    {
    public:
        EpollComponent();
        EpollComponent( const EpollComponent& copy ) = delete;
        virtual ~EpollComponent();

        bool Create( int nMaxEvents, void(*InitEventProc)(epoll_event*, void*), void* res = nullptr );
        const int GetPollFd() const noexcept;
        std::vector<epoll_event*> Listen( const int timeout, const bool block = false );

        bool BindEvent( const int fd );

    protected:
        EpollComponent& SetEpollFd( int fd );

    private:
        int m_n_poll_fd;
        epoll_event* m_p_events;
        int m_n_event_num;
    };
};

#endif
