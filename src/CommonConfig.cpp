#include "seadrip/config/BaseConfig.h"
#include "seadrip/shell/ShellInput.h"
#include "seadrip/file/KvFileReader.h"
using namespace SeaDrip;

//  ==================  BaseConfig  ============================================

BaseConfig::BaseConfig( std::string def_cfg_path, int argc, char** argv ) : m_s_config_file( def_cfg_path )
{
    auto shell_catcher = this->RegistShellItem( new ShellInput() );
    for( auto item : shell_catcher->Parse( argc, argv ) )
    {
        this->ShellOverride( item.first, item.second );
    }
    std::string cfg = this->m_s_config_file.Get(), key, val;
    KvFileReader* cfgfile = cfg.empty() ? nullptr : new KvFileReader( cfg, '#' );
    if( cfgfile && cfgfile->is_open() )
    {
        while( cfgfile->Next( key, val ) )
        {
            this->CfgFileOverride( key, val );
        }
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

bool BaseConfig::Valid( void ) const noexcept
{
    return !this->m_s_config_file.Get().empty();
}
