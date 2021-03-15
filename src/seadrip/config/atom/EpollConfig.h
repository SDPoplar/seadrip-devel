#ifndef __SD_CONF_ATOM_EPOLL_H__
#define __SD_CONF_ATOM_EPOLL_H__

#include "../base/ConfigProperty.hpp"

namespace SeaDrip
{
    class EpollConfig
    {
    public:
        EpollConfig( int def_max_event );
        EpollConfig( const EpollConfig& copy ) = default;
        virtual ~EpollConfig() = default;

        const int GetMaxEvent() const noexcept;
        EpollConfig &SetMaxEvent( const int max_event_num, const EConfigSetFrom from = EConfigSetFrom::RUNTIME );

    protected:
        SeaDrip::TConfigProperty<int> m_n_epoll_max_event;
    };
};

#endif
