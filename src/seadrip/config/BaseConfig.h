#ifndef     __SD_CONFIG_BASE_CONFIG_H__
#define     __SD_CONFIG_BASE_CONFIG_H__

#include "ConfigProperty.hpp"

#define DECLARE_SDCFG_ITEM( name, type, var )                                                                           \
public:                                                                                                                 \
    type Get##name() const { return this->##var.Get(); }                                                                \
    void Set##name( type _##var, EConfigSetFrom from = EConfigSetFrom::RUNTIME ) { this->##var.Set( from, _##var ); }   \
private:                                                                                                                \
    TConfigProperty<type> var

namespace SeaDrip
{
    class BaseConfig
    {
        public:
            BaseConfig( std::string def_cfg_path = "" );
            virtual bool Init( int argc, char** argv );

        protected:
            TConfigProperty<std::string> m_s_config_file;
    };
};

#endif
