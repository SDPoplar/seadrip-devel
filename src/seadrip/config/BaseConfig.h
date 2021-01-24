#ifndef     __SD_CONFIG_BASE_CONFIG_H__
#define     __SD_CONFIG_BASE_CONFIG_H__

#include "ConfigProperty.hpp"

namespace SeaDrip
{
    typedef struct
    {
        bool stopIfInvalid;
        char optionItem;
        std::string invalidValue;
    } InvalidShellOption;

    class BaseConfig
    {
        public:
            BaseConfig( const std::string& def_cfg_path );
            const std::string GetConfigPath() const noexcept;
            BaseConfig& SetConfigPath( const std::string& cfg_path, EConfigSetFrom from = EConfigSetFrom::RUNTIME );
            const bool IsDebug( void ) const noexcept;
            BaseConfig& SetDebug( const bool flag = true, EConfigSetFrom from = EConfigSetFrom::RUNTIME );

        protected:
            bool InitWithShell( int argc, char** argv, InvalidShellOption& = *((InvalidShellOption*)nullptr) );
            virtual const std::string GetShellOptions( void );
            virtual const bool SetShellOption( const char, const char* );

            TConfigProperty<std::string> m_s_config_file;
            TConfigProperty<bool> m_b_debug;

        private:
            void _initDebug( char* );
            void _initConfigPath( char* );
    };
};

#endif
