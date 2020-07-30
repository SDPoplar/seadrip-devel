#include "seadrip/config/BaseConfig.h"
#include "seadrip/shell/ShellInput.h"
using namespace SeaDrip;

//  ==================  BaseConfig  ============================================

BaseConfig::BaseConfig( std::string def_cfg_path, int argc, char** argv ) : m_s_config_file( def_cfg_path )
{
    auto shell_catcher = this->RegistShellItem( new ShellInput() );
    for( auto item : shell_catcher->Parse( argc, argv ) )
    {
        this->ShellOverride( item.first, item.second );
    }
}

ShellInput* BaseConfig::RegistShellItem( ShellInput* catcher )
{
    return catcher->AppendInput( 'c', "config_file" );
        //  ->AppendBool( 'd', "daemon", true );
}

bool BaseConfig::ShellOverride( std::string key, std::string val )
{
    CONFIG_OVERRIDE_FROM_SHELL( "c", this->m_s_config_file );
    return false;
}

bool BaseConfig::CfgFileOverride( std::string key, std::string val )
{
    return false;
}
