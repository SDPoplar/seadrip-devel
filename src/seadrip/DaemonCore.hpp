#ifndef __SD_DAEMON_CORE_HPP__
#define __SD_DAEMON_CORE_HPP__

#include "TickCore.h"
#include "DaemonConfig.h"
#include "DaemonLog.h"
#include "RunCode.h"

#if defined( linux ) or defined( __GNUC__ )
    #define LIMIT_TEMPLATE( tpltype, basetype ) template<typename tpltype, typename std::enable_if<std::is_base_of<basetype, tpltype>{}, int>::type = 0>
#else
    #define LIMIT_TEMPLATE( tpltype, basetype ) template<typename tpltype, typename std::enable_if <std::is_base_of<basetype, tpltype>::value, int>::type = 0 >
#endif

#define SDCORE_RET_FALSE_ERR_LOG( exp, code, log, log_level ) _SDCORE_RET_FALSE( exp, this->Error( code ); this->GetLog()->log_level( log ); )

typedef void( *LINUX_SIG_METHOD )( int );
bool SaveDaemonPid( std::string pid_path );

namespace SeaDrip
{
    LIMIT_TEMPLATE( Conf, DaemonConfig )
    class DaemonCore : public TickCore
    {
    public:
        Conf* GetConfig( void ) { return &this->m_o_conf; }
        DaemonLog* GetLog( void ) { return &this->m_o_log; }

    protected:
        DaemonCore() : m_p_quit_method( nullptr ) {}

        virtual bool ReadyToRun() override
        {
            SDCORE_RET_FALSE( !SeaDrip::TickCore::ReadyToRun() );
            SDCORE_RET_FALSE_ERR( !this->m_p_quit_method, DaemonCoreRunCode::NO_QUIT_METHOD_SETTED );
            auto conf = this->GetConfig();
            SDCORE_RET_FALSE_ERR( this->GetLog()->SetLogPath( conf->GetLogPath() ), DaemonCoreRunCode::SET_LOG_PATH_FAILED );
            SDCORE_RET_FALSE_ERR( this->GetLog()->SetLogLevel( conf->GetLogLevel() ), DaemonCoreRunCode::SET_LOG_LEVEL_FAILED );
            SDCORE_RET_FALSE_ERR( !SaveDaemonPid( conf->GetPidPath() ), DaemonCoreRunCode::PID_FILE_CANNOT_SAVE );
            SDCORE_RET_FALSE_ERR( daemon( 1, 1 ) != 0, DaemonCoreRunCode::CREATE_DAEMON_FAILED );

            SaveDaemonPid( conf->GetPidPath() );
            signal( conf->GetExitSig(), this->m_p_quit_method );
            return true;
        }

        LINUX_SIG_METHOD m_p_quit_method;

    private:
        Conf m_o_conf;
        DaemonLog m_o_log;
    };
};

#endif
