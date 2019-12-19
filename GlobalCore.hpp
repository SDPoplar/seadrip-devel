#ifndef __SD_GLOBAL_CORE_HPP__
#define __SD_GLOBAL_CORE_HPP__

#if defined( linux ) or defined( __GNUC__ )
#include <signal.h>
#include <sys/socket.h>
#endif
#include <fstream>
#include <type_traits>
#include <err.h>
#include "ConfigProperty.hpp"


#if defined( linux ) or defined( __GNUC__ )
    #define LIMIT_TEMPLATE( tpltype, basetype ) template<typename tpltype, typename std::enable_if<std::is_base_of<basetype, tpltype>{}, int>::type = 0>
#else
    #define LIMIT_TEMPLATE( tpltype, basetype ) template<typename tpltype, typename std::enable_if <std::is_base_of<basetype, tpltype>::value, int>::type = 0 >
#endif

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
        }

        void Quit()
        {
            this->m_b_run_switch = false;
            this->PreQuit();
        }

    protected:
        SingletonCore() : m_b_run_switch( false ) { g_p_core = static_cast<T *>( this ); }
        SingletonCore( const SingletonCore& obj ) {}
        SingletonCore& operator = ( const SingletonCore& obj ) = default;

        virtual bool ReadyToRun() { return true; }
        virtual void Release() {}
        virtual void Tick() {}
        virtual void PreQuit() {}

    private:
        static T* g_p_core;
        bool m_b_run_switch;
    };

    template<typename T>
    T* SingletonCore<T>::g_p_core = nullptr;

    LIMIT_TEMPLATE( Conf, SeaDrip::BaseConfig )
    class DaemonCore : public SingletonCore<DaemonCore<Conf>>
    {
        friend SingletonCore<DaemonCore>;
    protected:
    #ifndef DEF_CFG_PATH
        #define DEF_CFG_PATH ""
        #error 'DEF_CFG_PATH' not defined
    #endif
        DaemonCore() : SingletonCore<DaemonCore>(), m_o_conf( DEF_CFG_PATH ), m_b_run_switch( false ), m_b_pid_saved( false ) {}
        bool SavePid( int pid = 0 )
        {
            std::string pidpath = this->m_o_conf.GetPidPath();
            if( pidpath.empty() )
            {
                return false;
            }
            std::ofstream fpid( pidpath, std::ios::out );
            if( !fpid )
            {
                return false;
            }
            if( pid > 0 )
            {
                fpid << pid;
            }
            fpid.close();
            return true;
        }

        Conf m_o_conf;
        bool m_b_run_switch;
        bool m_b_pid_saved;

    public:
        Conf* GetConfig()
        {
            return &this->m_o_conf;
        }

    protected:
        virtual bool ReadyToRun() override
        {
            if( !SingletonCore<DaemonCore<Conf>>::ReadyToRun() )
            {
                return false;
            }
            std::string pidpath = this->m_o_conf.GetPidPath();
            int npid = getpid();
            
            if( !this->SavePid() || daemon( 1, 1 ) )
            {
                return false;
            }
            
            int dpid = getpid();
            if( npid == dpid )
            {
                return false;
            }
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
        virtual bool ReadyToRun() override
        {
            if( !DaemonCore<Conf>::ReadyToRun() )
            {
                fprintf( stderr, "Daemon init failed\n" );
                return false;
            }
            this->m_n_socket = socket( AF_INET, SOCK_STREAM, 0 );
            fprintf( stderr, "Tcmd get socket %d\n", this->m_n_socket );
            if( this->m_n_socket == -1 )
            {
                fprintf( stderr, "Socket create failed\n" );
                return false;
            }
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = htonl( this->m_o_conf.GetSockAddr() );
            addr.sin_port = htons( this->m_o_conf.GetListenPort() );
            if( bind( this->m_n_socket, &addr, sizeof( addr ) ) == -1 )
            {
                fprintf( stderr, "Bind domain failed, with port %d\n", this->m_o_conf.GetListenPort() );
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
