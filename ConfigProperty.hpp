#ifndef     __SD_CONFIG_PROPERTY_HPP__
#define     __SD_CONFIG_PROPERTY_HPP__

#include <string>
#include <map>
#include <fstream>
#include <functional>
#if defined( linux ) or defined( __GNUC__ )
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#endif
#include <seadrip/KvFileReader.hpp>
#include <seadrip/LinuxSigMap.h>

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
#if defined( linux ) or defined( __GNUC__ )
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
                    if( val.empty() )
                    {
                        continue;
                    }
                    if( !this->CfgFileOverride( key, val ) )
                    {
                        std::cout << "Unknown config item: " << key << std::endl;
                    }
                }
#endif
            }
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
        DaemonConfig( std::string def_cfg_path ) : BaseConfig( def_cfg_path ), m_s_pid_path( "" ), m_n_exit_sig( SIGUSR2 ) {}
        std::string GetPidPath( void ) const noexcept { return this->m_s_pid_path.Get(); }
        int GetExitSig( void ) const noexcept { return this->m_n_exit_sig.Get(); }

    protected:
        virtual bool CfgFileOverride( std::string key, std::string val ) override
        {
            if( key == "pid" )
            {
                this->m_s_pid_path.Set( EConfigSetFrom::CFGFILE, val );
                return true;
            }
            if( key == "exit_sig" )
            {
                std::string upperval = boost::to_upper_copy( val );
                auto sigfound = linux_sig_map.find( upperval );
                int nsig = ( sigfound != linux_sig_map.end() ) ? sigfound->second : atoi( val.c_str() );
                this->m_n_exit_sig.Set( EConfigSetFrom::CFGFILE, nsig );
                return true;
            }
            return false;
        }

    private:
        TConfigProperty<std::string> m_s_pid_path;
        TConfigProperty<int> m_n_exit_sig;
    };

    #ifndef DEF_LISTEN_PORT
        #define DEF_LISTEN_PORT 0
        #error 'DEF_LISTEN_PORT' not defined
    #endif
    class SocketDaemonConfig : public DaemonConfig
    {
    public:
        SocketDaemonConfig( std::string def_cfg_path ) : DaemonConfig( def_cfg_path ),
            m_n_listen_port( DEF_LISTEN_PORT ), m_n_sock_addr( INADDR_ANY ) {}
        int GetListenPort() const noexcept { return this->m_n_listen_port.Get(); }
        in_addr_t GetSockAddr() const noexcept { return this->m_n_sock_addr.Get(); }

    protected:
        virtual bool CfgFileOverride( std::string key, std::string val ) override
        {
            if( DaemonConfig::CfgFileOverride( key, val ) )
            {
                return true;
            }
            if( key == "sock_addr" )
            {
                //  translate val to in_addr_t
                this->m_n_sock_addr.Set( EConfigSetFrom::CFGFILE, INADDR_ANY );
                return true;
            }
            if( key == "listen" )
            {
                this->m_n_listen_port.Set( EConfigSetFrom::CFGFILE, atoi( val.c_str() ) );
                return true;
            }
            return false;
        }

    private:
        TConfigProperty<int> m_n_listen_port;
        TConfigProperty<in_addr_t> m_n_sock_addr;
    };
};

#endif
