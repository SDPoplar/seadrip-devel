#ifndef     __SD_CONFIG_CONFIG_PROPERTY_HPP__
#define     __SD_CONFIG_CONFIG_PROPERTY_HPP__

#include <string>
#include <map>
#if defined( linux ) or defined( __GNUC__ )
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#endif
//  #include "DaemonLog.h"

namespace SeaDrip
{
    enum class EConfigSetFrom : int
    {
        NOTSET = 0,
        DEF = 1,
        CFGFILE =  2,
        SHELL = 3,
        RUNTIME = 4
    };

    template <typename T> class TConfigProperty
    {            
        protected:
            SeaDrip::EConfigSetFrom m_e_from;
            T m_o_val;
        public:
            TConfigProperty() : m_e_from( EConfigSetFrom::NOTSET ) {}
            TConfigProperty( T def ) : m_e_from( EConfigSetFrom::DEF ), m_o_val( def ) {}
            TConfigProperty( const TConfigProperty &obj ) : m_e_from( obj.m_e_from ), m_o_val( obj.m_o_val ) {}
            virtual ~TConfigProperty() = default;
            bool Set( EConfigSetFrom from, T val )
            {
                if( static_cast<int>( from ) < static_cast<int>( this->m_e_from ) )
                {
                    return false;
                }
                this->m_e_from = from;
                this->m_o_val = val;
                return true;
            }

            T Get() const
            {
                return this->m_o_val;
            }

            virtual bool IsBoolProperty() const noexcept { return false; }
    };

    template<> inline bool TConfigProperty<bool>::IsBoolProperty() const noexcept { return true; }
};

#endif

