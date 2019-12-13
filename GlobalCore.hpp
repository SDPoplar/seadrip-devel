#ifndef __SD_GLOBAL_CORE_HPP__
#define __SD_GLOBAL_CORE_HPP__

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

    protected:
        SingletonCore() { g_p_core = static_cast<T *>( this ); }
        SingletonCore( const SingletonCore& obj ) {}
        SingletonCore& operator = ( const SingletonCore& obj ) = default;

    private:
        static typename T* g_p_core;
    };

    template<typename T>
    T* SingletonCore<T>::g_p_core = nullptr;

#if defined( linux ) or defined( _GNUC_ )
    template<typename Conf, typename std::enable_if<std::is_base_of<SeaDrip::BaseConfig, Conf>{}, int>::type = 0>
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
        DaemonCore() : SingletonCore<DaemonCore>(), m_o_conf( DEF_CFG_PATH ) {}

        Conf m_o_conf;

    public:
        Conf* GetConfig()
        {
            return &this->m_o_conf;
        }
    };
};

#endif
