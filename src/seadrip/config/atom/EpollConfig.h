#ifndef __SD_CONF_ATOM_EPOLL_H__
#define __SD_CONF_ATOM_EPOLL_H__

#include "../base/ConfigProperty.hpp"

namespace SeaDrip
{
    class EpollConfig
    {
    public:
        EpollConfig( const int def_max_event, const int def_timeout = 1000 );
        EpollConfig( const EpollConfig& copy ) = default;
        virtual ~EpollConfig() = default;

        const int GetMaxEvent() const noexcept;
        EpollConfig &SetMaxEvent( const int max_event_num, const EConfigSetFrom from = EConfigSetFrom::RUNTIME );
        const int GetTimeout() const noexcept;
        EpollConfig &SetTimeout( const int timeout, const EConfigSetFrom from = EConfigSetFrom::RUNTIME );

    protected:
        SeaDrip::TConfigProperty<int> m_n_epoll_max_event;
        SeaDrip::TConfigProperty<int> m_n_epoll_timeout;
    };
};

#endif
