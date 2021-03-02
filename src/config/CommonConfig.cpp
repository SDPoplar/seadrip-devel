#include "../seadrip/config/base/BaseConfig.h"
#include "../seadrip/file/KvFileReader.h"
//  #include "seadrip/shell/ShellInput.h"
//  #include "seadrip/file/IniFileReader.h"
#include <boost/algorithm/string/case_conv.hpp>
using namespace SeaDrip;

InvalidShellOption::InvalidShellOption( const bool stop_if_invalid ) : stopIfInvalid( stop_if_invalid )
{}

InvalidConfigOption::InvalidConfigOption( const bool stop_if_invalid ) : stopIfInvalid( stop_if_invalid )
{}

//  ==================  BaseConfig  ============================================

BaseConfig::BaseConfig( const std::string& def_cfg_path ) : m_s_config_file( def_cfg_path )
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

const bool BaseConfig::InitWithShell( int argc, char** argv, InvalidShellOption& eholder )
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
    return "c:";
}

const bool BaseConfig::SetShellOption( const char item, const char* val )
{
    switch( item )
    {
        /*
        case 'd':
            this->SetDebug();
            return true;
        */
        case 'c':
            this->SetConfigPath( val, EConfigSetFrom::SHELL );
            return true;
        default:
            return false;
    }
}

const bool BaseConfig::InitWithIniReader( SeaDrip::KvFileReader& ini, InvalidConfigOption& ico )
{
    std::string key, val;
    while( ini.Next( key, val ) )
    {
        boost::to_lower( key );
        if( this->SetConfigOption( key, val ) )
        {
            continue;
        }
        if( &ico && ico.stopIfInvalid )
        {
            ico.configItem = key;
            ico.invalidValue = val;
            ico.optionAtLine = 0; //ini.GetLineIndex();
            return false;
        }
    }
    return true;
}

const bool BaseConfig::SetConfigOption( const std::string key, const std::string value )
{
    /*
    if( ( key == "debug" ) )
    {
        this->SetDebug( boost::to_lower_copy( value ) == "true", EConfigSetFrom::CFGFILE );
        return true;
    }
    */
    return false;
}
