#ifndef     __SD_CONFIG_PROPERTY_HPP__
#define     __SD_CONFIG_PROPERTY_HPP__

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
            virtual ~TConfigProperty() {}
            bool Set( EConfigSetFrom from, T val )
            {
                bool canset = static_cast<int>( from ) >= static_cast<int>( this->m_e_from );
                if( !canset )
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

    };

    class BaseConfig
    {
        public:
            BaseConfig( std::string def_cfg_path = "" );
            void Init( int argc, char** argv );

        protected:
            virtual void ShellOverride( char shell_flag, std::string val ) = 0;
            virtual bool CfgFileOverride( std::string key, std::string val ) { return false; };

            TConfigProperty<std::string> m_s_config_file;
            std::string m_s_shell_options;
            std::map<char, TConfigProperty<bool>*> m_map_bool_props;
    };

};

#endif

