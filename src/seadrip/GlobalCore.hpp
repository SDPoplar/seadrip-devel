#ifndef __SD_GLOBAL_CORE_HPP__
#define __SD_GLOBAL_CORE_HPP__

#if defined( linux ) or defined( __GNUC__ )
#include <signal.h>
#include <sys/socket.h>
#endif
#include <fstream>
#include <type_traits>
#include "ConfigProperty.hpp"
#include "RunCode.h"
#include "DaemonLog.h"

#include <iostream>

#if defined( linux ) or defined( __GNUC__ )
    #define LIMIT_TEMPLATE( tpltype, basetype ) template<typename tpltype, typename std::enable_if<std::is_base_of<basetype, tpltype>{}, int>::type = 0>
#else
    #define LIMIT_TEMPLATE( tpltype, basetype ) template<typename tpltype, typename std::enable_if <std::is_base_of<basetype, tpltype>::value, int>::type = 0 >
#endif

#define _SDCORE_RET_FALSE( exp, dosth ) do { if( exp ) { dosth return false; } } while( false )
#define SDCORE_RET_FALSE( exp ) _SDCORE_RET_FALSE( exp, ; )
#define SDCORE_RET_FALSE_ERR( exp, code ) _SDCORE_RET_FALSE( exp, this->Error( SD_RUNCODE( code ) ); )
#define SDCORE_RET_FALSE_ERR_LOG( exp, code, log, log_level ) _SDCORE_RET_FALSE( exp, this->Error( SD_RUNCODE( code ) ); this->GetLog()->log_level( log ); )

namespace SeaDrip
{
    template<typename T> class SingletonCore
    {
    public:
        static T* Get()
        {
            return SingletonCore<T>::g_p_core ? SingletonCore<T>::g_p_core : new T();
        }

        bool IsRunning() const noexcept
        {
            return this->m_b_run_switch;
        }


        int Run()
        {
            this->m_b_run_switch = this->ReadyToRun();
            while( this->m_b_run_switch )
            {
                this->Tick();
            }
            this->Release();
            return this->m_n_runcode;
        }

        void Quit()
        {
            this->m_b_run_switch = false;
            this->PreQuit();
        }

    protected:
        SingletonCore() : m_b_run_switch( false ), m_n_runcode( SD_RUNCODE( ECoreRunCode::OK ) ) { g_p_core = static_cast<T *>( this ); }
        SingletonCore( const SingletonCore& obj )  = delete;
        SingletonCore& operator = ( const SingletonCore& obj ) = delete;
        //  void Error( ECoreRunCode code ) { this->Error( SD_RUNCODE( code ) ); }
        void Error( int code ) { this->m_n_runcode = code; }

        virtual bool ReadyToRun() { return true; }
        virtual void Release() {}
        virtual void Tick() {}
        virtual void PreQuit() {}

    private:
        static T* g_p_core;
        bool m_b_run_switch;
        int m_n_runcode;
    };

    template<typename T>
    T* SingletonCore<T>::g_p_core = nullptr;

    LIMIT_TEMPLATE( Conf, SeaDrip::BaseConfig )
    class DaemonCore : public SingletonCore<DaemonCore<Conf>>
    {
        friend SingletonCore<DaemonCore>;
    protected:
        DaemonCore() : SingletonCore<DaemonCore>(), m_o_conf(), m_b_run_switch( false ), m_b_pid_saved( false ) {}
        int SavePid()
        {
            std::ofstream fpid( this->m_o_conf.GetPidPath(), std::ios::out );
            if( !fpid )
            {
                return 0;
            }
            int pid = getpid();
            fpid << pid;
            fpid.close();
            return pid;
        }

    private:
        Conf m_o_conf;
        bool m_b_run_switch;
        bool m_b_pid_saved;
        DaemonLog m_o_log;

    public:
        Conf* GetConfig()
        {
            return &this->m_o_conf;
        }

        DaemonLog* GetLog() const noexcept
        {
            return &this->m_o_log;
        }

    protected:
        virtual bool ReadyToRun() override
        {
            SDCORE_RET_FALSE( !SingletonCore<DaemonCore<Conf>>::ReadyToRun() );
            int npid = this->SavePid();

            SDCORE_RET_FALSE_ERR( !this->SavePid(), EDaemonCoreRunCode::PID_FILE_CANNOT_SAVE );
            SDCORE_RET_FALSE_ERR( daemon( 1, 1 ) == -1, EDaemonCoreRunCode::CREATE_DAEMON_FAILED );
            
            int dpid = this->SavePid();
            //  SDCORE_RET_FALSE( npid == dpid );
            //  bind linux signals
            signal( this->m_o_conf.GetExitSig(), []( int sig )->void {
                auto core = DaemonCore::Get();
                if( core->IsRunning() )
                {
                    core->Quit();
                }
            } );
            this->m_b_pid_saved = true;

            SDCORE_RET_FALSE_ERR( !this->m_o_log.SetLogPath( this->m_o_conf.GetLogPath() ), EDaemonCoreRunCode::SET_LOG_PATH_FAILED );
            SDCORE_RET_FALSE_ERR( !this->m_o_log.SetLogLevel( this->m_o_conf.GetLogLevel() ), EDaemonCoreRunCode::SET_LOG_LEVEL_FAILED );

            return true;
        }

        void Release() override
        {
            std::string pidpath = this->m_o_conf.GetPidPath();
            if( !pidpath.empty() && this->m_b_pid_saved )
            {
                remove( pidpath.c_str() );
            }
        }
    };

    LIMIT_TEMPLATE( Conf, SeaDrip::SocketDaemonConfig )
    class SocketDaemonCore : public DaemonCore<Conf>
    {
    protected:
        SocketDaemonCore() : DaemonCore<Conf>(), m_n_socket( 0 ) {}
        virtual bool ReadyToRun() override
        {
            std::cout << "SocketDaemonCore::ReadyToRun()" << std::endl;
            SDCORE_RET_FALSE( !DaemonCore<Conf>::ReadyToRun() );
            std::cout << "DaemonCore::ReadyToRun success" << std::endl;

            int port = this->GetConfig()->GetListenPort();
            std::cout << "Setting port" << std::endl;
            SDCORE_RET_FALSE_ERR_LOG( !port, ESocketDaemonCoreRunCode::NO_PORT_SETTED, "No listen port setted", Error );
            this->m_n_socket = socket( AF_INET, SOCK_STREAM, 0 );
            if( this->GetLog()->Debug( "Tcmd get socket: " + this->m_n_socket ) ) {
                std::cout << "Debug info write to log failed" << std::endl;
            }
            SDCORE_RET_FALSE_ERR_LOG( this->m_n_socket == -1, ESocketDaemonCoreRunCode::CREATE_SOCK_FAILED, "Socket create failed", Error );
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = htonl( this->m_o_conf.GetSockAddr() );
            addr.sin_port = htons( port );
            SDCORE_RET_FALSE_ERR_LOG( bind( this->m_n_socket, &addr, sizeof( addr ) ) == -1, ESocketDaemonCoreRunCode::PORT_CONFLICT,
                "Bind domain failed, with port: " + port, Error );
            SDCORE_RET_FALSE_ERR_LOG( listen( this->m_n_socket, SI_QUEUE ) == -1, ESocketDaemonCoreRunCode::PORT_CONFLICT,
                "Port conflict at: " + port, Error );
            return true;
        }

        virtual void PreQuit() override
        {
            if( this->m_n_socket != -1 )
            {
                close( this->m_n_socket );
            }
        }

        virtual void Tick() override
        {
            if( this->m_n_socket != -1 )
            {
                //  accept( this->m_n_socket );
            }
        }

        int m_n_socket;
    };
};

#endif
