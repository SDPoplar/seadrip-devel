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
        INHERIT = 2,
        CFGFILE =  3,
        SHELL = 4,
        RUNTIME = 5
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
            BaseConfig( std::string def_cfg_path = "" );
            void Init( int argc, char** argv );

        protected:
            virtual void ShellOverride( char shell_flag, std::string val ) = 0;
            virtual bool CfgFileOverride( std::string key, std::string val ) { return false; };

            TConfigProperty<std::string> m_s_config_file;
            std::string m_s_shell_options;
            std::map<char, TConfigProperty<bool>*> m_map_bool_props;
    };

    class DaemonConfig : public BaseConfig
    {
    public:
        DaemonConfig( std::string def_cfg_path = "" );
        std::string GetPidPath( void ) const noexcept { return this->m_s_pid_path.Get(); }
        int GetExitSig( void ) const noexcept { return this->m_n_exit_sig.Get(); }
        std::string GetLogPath( void ) const noexcept { return this->m_s_log_path.Get(); }
        std::string GetLogLevel( void ) const noexcept { return this->m_s_log_level.Get(); }
        //  const std::set<ELogLevel>& GetLogForceSave() const noexcept { return this->m_set_force_save; }

    protected:
        virtual bool CfgFileOverride( std::string key, std::string val ) override;

        TConfigProperty<std::string> m_s_pid_path;
        TConfigProperty<int> m_n_exit_sig;
        TConfigProperty<std::string> m_s_log_path;
        TConfigProperty<std::string> m_s_log_level;
        //  std::set<ELogLevel> m_set_force_save;
    };

    class SocketDaemonConfig : public DaemonConfig
    {
    public:
        SocketDaemonConfig( std::string def_cfg_path = "", int def_listen_port = 0 );
        int GetListenPort() const noexcept { return this->m_n_listen_port.Get(); }
        in_addr_t GetSockAddr() const noexcept { return this->m_n_sock_addr.Get(); }

    protected:
        virtual bool CfgFileOverride( std::string key, std::string val ) override;

        TConfigProperty<int> m_n_listen_port;
        TConfigProperty<in_addr_t> m_n_sock_addr;
    };
};

#endif

