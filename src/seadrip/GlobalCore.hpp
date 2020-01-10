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


#if defined( linux ) or defined( __GNUC__ )
    #define LIMIT_TEMPLATE( tpltype, basetype ) template<typename tpltype, typename std::enable_if<std::is_base_of<basetype, tpltype>{}, int>::type = 0>
#else
    #define LIMIT_TEMPLATE( tpltype, basetype ) template<typename tpltype, typename std::enable_if <std::is_base_of<basetype, tpltype>::value, int>::type = 0 >
#endif

#define SDCORE_RET_FALSE( exp ) do { if( exp ) { return false; } } while( false )
#define SDCORE_RET_FALSE_ERR( exp, code ) do { if( exp ) { this->Error( SD_RUNCODE( code ) ); return false; } } while( false )

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
        SingletonCore( const SingletonCore& obj ) {}
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
        bool SavePid( int pid = 0 )
        {
            std::string pidpath = this->m_o_conf.GetPidPath();
            SDCORE_RET_FALSE_ERR( pidpath.empty(), EDaemonCoreRunCode::EMPTY_PID_PATH );
            std::ofstream fpid( pidpath, std::ios::out );
            SDCORE_RET_FALSE_ERR( !fpid, EDaemonCoreRunCode::PID_FILE_CANNOT_SAVE );
            if( pid > 0 )
            {
                fpid << pid;
            }
            fpid.close();
            return true;
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
            std::string pidpath = this->m_o_conf.GetPidPath();
            int npid = getpid();

            SDCORE_RET_FALSE( !this->SavePid() );            
            SDCORE_RET_FALSE_ERR( daemon( 1, 1 ), EDaemonCoreRunCode::CREATE_DAEMON_FAILED );
            
            int dpid = getpid();
            SDCORE_RET_FALSE( npid == dpid );
            this->SavePid( dpid );
            //  bind linux signals
            signal( this->m_o_conf.GetExitSig(), []( int sig )->void {
                auto core = DaemonCore::Get();
                if( core->IsRunning() )
                {
                    core->Quit();
                }
            } );
            this->m_b_pid_saved = true;
            return this->m_o_log.Init( this->m_o_conf );
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
    public:
        SocketDaemonCore() : DaemonCore<Conf>() {}
    protected:
        virtual bool ReadyToRun() override
        {
            if( !DaemonCore<Conf>::ReadyToRun() )
            {
                this->GetLog()->Error( "Daemon init failed" );
                return false;
            }
            int port = this->GetConfig()->GetListenPort();
            if( !port )
            {
                this->GetLog()->Error( "No listen port setted" );
                return false;
            }
            this->m_n_socket = socket( AF_INET, SOCK_STREAM, 0 );
            this->GetLog()->Debug( "Tcmd get socket: " + this->m_n_socket );
            if( this->m_n_socket == -1 )
            {
                this->GetLog()->Error( "Socket create failed" );
                return false;
            }
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = htonl( this->m_o_conf.GetSockAddr() );
            addr.sin_port = htons( port );
            if( bind( this->m_n_socket, &addr, sizeof( addr ) ) == -1 )
            {
                this->GetLog()->Error( "Bind domain failed, with port: " + port );
                return -1;
            }
            return listen( this->m_n_socket, SI_QUEUE ) != -1;
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
