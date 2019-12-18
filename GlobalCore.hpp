#ifndef __SD_GLOBAL_CORE_HPP__
#define __SD_GLOBAL_CORE_HPP__

#if defined( linux ) or defined( __GNUC__ )
#include <signal.h>
#endif
#include <fstream>
#include <type_traits>
#include "ConfigProperty.hpp"

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
        }

    protected:
        SingletonCore() : m_b_run_switch( false ) { g_p_core = static_cast<T *>( this ); }
        SingletonCore( const SingletonCore& obj ) {}
        SingletonCore& operator = ( const SingletonCore& obj ) = default;

        virtual bool ReadyToRun() { return true; }
        virtual void Release() {}
        virtual void Tick() {}

    private:
        static T* g_p_core;
        bool m_b_run_switch;
    };

    template<typename T>
    T* SingletonCore<T>::g_p_core = nullptr;
    
#if defined( linux ) or defined( __GNUC__ )
    template<typename Conf, typename std::enable_if<std::is_base_of<SeaDrip::DaemonConfig, Conf>{}, int>::type = 0>
#else
    template<typename Conf, typename std::enable_if < std::is_base_of<SeaDrip::BaseConfig, Conf>::value, int > ::type = 0 >
#endif
    class DaemonCore : public SingletonCore<DaemonCore<Conf>>
    {
        friend SingletonCore<DaemonCore>;
    protected:
    #ifndef DEF_CFG_PATH
        #define DEF_CFG_PATH ""
        #error 'DEF_CFG_PATH' not defined
    #endif
        DaemonCore() : SingletonCore<DaemonCore>(), m_o_conf( DEF_CFG_PATH ), m_b_run_switch( false ) {}

        Conf m_o_conf;
        bool m_b_run_switch;

    public:
        Conf* GetConfig()
        {
            return &this->m_o_conf;
        }

        bool ReadyToRun() override
        {
            std::string pidpath = this->m_o_conf.GetPidPath();
            if( !pidpath.empty() )
            {
                std::ofstream pid( pidpath, std::ios::out );
                if( !pid )
                {
                    return false;
                }
                pid << getpid();
                pid.close();
            }
            signal( SIGUSR2, []( int sig )->void {
                auto core = DaemonCore::Get();
                if( core->IsRunning() )
                {
                    core->Quit();
                }
            } );
            return true;
        }

        void Release() override
        {
            //  this->m_b_run_switch = this->valid();
            std::string pidpath = this->m_o_conf.GetPidPath();
            if( !pidpath.empty() )
            {
                remove( pidpath.c_str() );
            }
        }
    };
};

#endif
