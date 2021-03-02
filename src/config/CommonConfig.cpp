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
