#ifndef     __SD_CONFIG_PROPERTY_HPP__
#define     __SD_CONFIG_PROPERTY_HPP__

#include <string>
#include <map>
#include <fstream>
#include <unistd.h>
#include <seadrip/KvFileReader.hpp>

namespace SeaDrip
{
    enum class EConfigSetFrom : int
    {
        DEF = 0,
        CFGFILE =  1,
        SHELL = 2,
        RUNTIME = 3
    };

    template <typename T> class TConfigProperty
    {            
        protected:
            SeaDrip::EConfigSetFrom m_e_from;
            T m_o_val;
        public:
            TConfigProperty( T def ) : m_e_from( SeaDrip::EConfigSetFrom::DEF ), m_o_val( def ) {}
            virtual ~TConfigProperty() {}
            bool Set( SeaDrip::EConfigSetFrom from, T val )
            {
                bool canset = static_cast<int>( from ) > static_cast<int>( this->m_e_from );
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
            BaseConfig( std::string def_cfg_path ) : m_s_config_file( def_cfg_path ) {}
            void Init( int argc, char** argv )
            {
                char ch;
                while( ( ch = getopt( argc, argv, this->m_s_shell_options.c_str() ) ) != -1 )
                {
                    if( ch == 'c' )
                    {
                        this->m_s_config_file.Set( EConfigSetFrom::SHELL, optarg );
                        continue;
                    }
                    auto prop = this->m_map_bool_props.find( ch );
                    if( prop != this->m_map_bool_props.end() )
                    {
                        prop->second->Set( EConfigSetFrom::SHELL, true );
                    }
                    else
                    {
                        this->ShellOverride( ch, optarg );
                    }
                }
                KvFileReader cfg( this->m_s_config_file.Get(), '#' );
                if( !cfg )
                {
                    return;
                }
                std::string key, val;
                while( cfg.Next( key, val ) )
                {
                    auto method = this->m_map_cfgfile_override.find( key );
                    if( method != this->m_map_cfgfile_override.end() )
                    {
                        method->second( val );
                    }                
                }
            }
        protected:
            virtual void ShellOverride( char shell_flag, std::string val ) = 0;

            TConfigProperty<std::string> m_s_config_file;
            std::string m_s_shell_options;
            std::map<char, TConfigProperty<bool>*> m_map_bool_props;
            std::map<std::string, void(*)( const std::string& )> m_map_cfgfile_override;
    };
};

#endif
