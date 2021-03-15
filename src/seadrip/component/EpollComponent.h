#ifndef __SD_COMPONENT_ATOM_EPOLL_H__
#define __SD_COMPONENT_ATOM_EPOLL_H__

namespace SeaDrip
{
    class EpollComponent
    {
    public:
        EpollComponent();
        virtual ~EpollComponent() = default;

        const int GetPollFd() const noexcept;

    protected:
        int m_n_poll_fd;
    };
};

#endif
