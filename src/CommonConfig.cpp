#include "seadrip/config/BaseConfig.h"
using namespace SeaDrip;

//  ==================  BaseConfig  ============================================

BaseConfig::BaseConfig( std::string def_cfg_path ) : m_s_config_file( def_cfg_path )
{}

bool BaseConfig::Init( int argc, char** argv )
{
    return false;
}
