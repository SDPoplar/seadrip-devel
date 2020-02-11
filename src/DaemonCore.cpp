#include "seadrip/DaemonCore.hpp"
using namespace SeaDrip;

bool SaveDaemonPid( std::string pid_path )
{
    std::ofstream pid( pid_path, std::ios::out );
    if( !pid )
    {
        return false;
    }
    pid << getpid();
    pid.close();
    return true;
}
