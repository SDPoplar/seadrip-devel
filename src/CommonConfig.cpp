#include "seadrip/config/BaseConfig.h"
#include "seadrip/shell/ShellInput.h"
#include "seadrip/file/KvFileReader.h"
using namespace SeaDrip;

//  ==================  BaseConfig  ============================================

BaseConfig::BaseConfig( const std::string& def_cfg_path )
    : m_s_config_file( def_cfg_path ), m_b_debug( false )
{}

const std::string BaseConfig::GetConfigPath( void ) const noexcept
{
    return this->m_s_config_file.Get();
}

BaseConfig& BaseConfig::SetConfigPath( const std::string& cfg_path, EConfigSetFrom from )
{
    this->m_s_config_file.Set( from, cfg_path );
    return *this;
}

const bool BaseConfig::IsDebug( void ) const noexcept
{
    return this->m_b_debug.Get();
}

BaseConfig& BaseConfig::SetDebug( const bool flag, EConfigSetFrom from )
{
    this->m_b_debug.Set( from, flag );
    return *this;
}

bool BaseConfig::InitWithShell( int argc, char** argv, InvalidShellOption& eholder )
{
    char opt;
    while( ( opt = getopt( argc, argv, this->GetShellOptions().c_str() ) ) != -1 )
    {
        if( this->SetShellOption( opt, optarg ) )
        {
            continue;
        }
        if( &eholder && eholder.stopIfInvalid )
        {
            eholder.optionItem = opt;
            eholder.invalidValue = optarg;
            return false;
        }
    }
    return true;
}

const std::string BaseConfig::GetShellOptions( void )
{
    return "dc:";
}

const bool BaseConfig::SetShellOption( const char item, const char* val )
{
    switch( item )
    {
        case 'd':
            this->SetDebug();
            return true;
        case 'c':
            this->SetConfigPath( val, EConfigSetFrom::SHELL );
            return true;
        default:
            return false;
    }
}
