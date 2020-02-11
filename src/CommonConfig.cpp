#include "seadrip/ConfigProperty.hpp"
#include <boost/algorithm/string.hpp>
#if defined( linux ) or defined( __GNUC__ )
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#endif
#include "seadrip/KvFileReader.h"
#include <iostream>
using namespace SeaDrip;

//  ==================  BaseConfig  ============================================

BaseConfig::BaseConfig( std::string def_cfg_path ) : m_s_config_file( def_cfg_path )
{}

void BaseConfig::Init( int argc, char** argv )
{
#if defined( linux ) or defined( __GNUC__ )
    char ch;
    while( (ch = getopt( argc, argv, this->m_s_shell_options.c_str() )) != -1 )
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
#endif
    if( this->m_s_config_file.Get().empty() )
    {
        std::cout << "No config file given" << std::endl;
        return;
    }
    KvFileReader cfg( this->m_s_config_file.Get(), '#' );
    if( !cfg )
    {
        std::cout << "Cannot open config file: " << this->m_s_config_file.Get() << std::endl;
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
}
