#ifndef     __SD_CONFIG_BASE_CONFIG_H__
#define     __SD_CONFIG_BASE_CONFIG_H__

#include "ConfigProperty.hpp"

namespace SeaDrip
{
    class BaseConfig
    {
        public:
            BaseConfig( const std::string& def_cfg_path );
            const std::string GetConfigPath() const noexcept;
            BaseConfig& SetConfigPath( const std::string& cfg_path, EConfigSetFrom from = EConfigSetFrom::RUNTIME );

        protected:
            TConfigProperty<std::string> m_s_config_file;
    };
};

#endif
