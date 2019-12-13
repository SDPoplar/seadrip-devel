#ifndef __SD_GLOBAL_CORE_HPP__
#define __SD_GLOBAL_CORE_HPP__

#include <type_traits>
#include "ConfigProperty.hpp"

namespace SeaDrip
{
    template<typename T>
    class SingletonCore
    {
    public:
        static T* Get()
        {
            return T::g_p_core ? T::g_p_core : new T();
        }

    protected:
        SingletonCore()
        {
            T::g_p_core = this;
        }


    private:
        static T* g_p_core;
    };

    template<typename T>
    T* SingletonCore<T>::g_p_core = nullptr;

    template<typename Conf, typename std::enable_if<std::is_base_of<SeaDrip::BaseConfig, Conf>{}, int>::type = 0>
    class DaemonCore : public SingletonCore<DaemonCore>
    {
        friend SingletonCore<DaemonCore>;
    protected:
    #ifndef DEF_CFG_PATH
        #define DEF_CFG_PATH ""
        #error 'DEF_CFG_PATH' not defined
    #endif
        DaemonCore() : SingletonCore(), m_o_conf( DEF_CFG_PATH ) {}

        Conf m_o_conf;

    public:
        Conf* GetConfig() const
        {
            return &this->m_o_conf;
        }
    };
};

#endif
