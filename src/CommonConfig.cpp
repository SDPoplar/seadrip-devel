#include "seadrip/config/BaseConfig.h"
#include "seadrip/shell/ShellInput.h"
using namespace SeaDrip;

//  ==================  BaseConfig  ============================================

BaseConfig::BaseConfig( std::string def_cfg_path ) : m_s_config_file( def_cfg_path )
{}

bool BaseConfig::Init( int argc, char** argv )
{
    auto in = ( new ShellInput() )
        ->AppendInput( 'c', "config_file" )
        ->AppendBool( 'd', "daemon", true );
    for( auto item : in->Parse( argc, argv ) )
    {}
    return false;
}
