#ifndef __SD_CONF_ATOM_DEBUG_H__
#define __SD_CONF_ATOM_DEBUG_H__

#include "../base/ConfigProperty.hpp"

namespace SeaDrip
{
    class DebugConfig
    {
    public:
        DebugConfig( const bool isDebug = false );
        DebugConfig( const DebugConfig& copy ) = default;
        virtual ~DebugConfig() = default;
        const bool IsDebug( void ) const noexcept;
        DebugConfig& SetDebug( const bool flag = true, const EConfigSetFrom from = EConfigSetFrom::RUNTIME );
    protected:
        TConfigProperty<bool> m_b_debug;
    };
};

#endif
