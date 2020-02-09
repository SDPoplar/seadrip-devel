#ifndef __SD_DAEMON_CORE_HPP__
#define __SD_DAEMON_CORE_HPP__

#include "SingletonCore.h"
#include "DaemonConfig.h"
#include "DaemonLog.h"

#if defined( linux ) or defined( __GNUC__ )
    #define LIMIT_TEMPLATE( tpltype, basetype ) template<typename tpltype, typename std::enable_if<std::is_base_of<basetype, tpltype>{}, int>::type = 0>
#else
    #define LIMIT_TEMPLATE( tpltype, basetype ) template<typename tpltype, typename std::enable_if <std::is_base_of<basetype, tpltype>::value, int>::type = 0 >
#endif

#define SDCORE_RET_FALSE_ERR_LOG( exp, code, log, log_level ) _SDCORE_RET_FALSE( exp, this->Error( code ); this->GetLog()->log_level( log ); )

namespace SeaDrip
{
    LIMIT_TEMPLATE( Conf, DaemonConfig )
    class DaemonCore : public SingletonCore
    {
    public:
        Conf* GetConfig( void ) { return &this->m_o_conf; }
        DaemonLog* GetLog( void ) { return &this->m_o_log; }

    protected:
        virtual bool ReadyToRun() override
        {
            SDCORE_RET_FALSE( !SingletonCore::ReadyToRun() );
            // daemon( 1, 1 )
            return true;
        }

    private:
        Conf m_o_conf;
        DaemonLog m_o_log;
    };
};

#endif
