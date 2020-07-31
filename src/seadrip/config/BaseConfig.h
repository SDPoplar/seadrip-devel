#ifndef     __SD_CONFIG_BASE_CONFIG_H__
#define     __SD_CONFIG_BASE_CONFIG_H__

#include "ConfigProperty.hpp"

#define DECLARE_SDCFG_ITEM( name, type, var )                                                                           \
public:                                                                                                                 \
    type Get##name() const { return this->##var.Get(); }                                                                \
    void Set##name( type _##var, EConfigSetFrom from = EConfigSetFrom::RUNTIME ) { this->##var.Set( from, _##var ); }   \
private:                                                                                                                \
    TConfigProperty<type> var

#define CONFIG_OVERRIDE( item, from, var ) do {                                                                         \
if( key == item )                                                                                                       \
{                                                                                                                       \
    var.Set( from, val );                                                                                               \
    return true;                                                                                                        \
} } while( false )

#define CONFIG_OVERRIDE_FROM_SHELL( item, var ) CONFIG_OVERRIDE( item, EConfigSetFrom::SHELL, var )
#define CONFIG_OVERRIDE_FROM_CFGFILE( item, var ) CONFIG_OVERRIDE( item, EConfigSetFrom::CFGFILE, var )

namespace SeaDrip
{
    class BaseConfig
    {
        public:
            BaseConfig( std::string def_cfg_path, int argc, char** argv );
            virtual bool Valid( void ) const noexcept;

        protected:
            virtual class ShellInput* RegistShellItem( class ShellInput* catcher );
            virtual bool ShellOverride( std::string key, std::string val );
            virtual bool CfgFileOverride( std::string key, std::string val );

            TConfigProperty<std::string> m_s_config_file;
    };
};

#endif
