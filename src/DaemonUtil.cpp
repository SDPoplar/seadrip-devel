#include "seadrip/shell/Daemon.h"
#include <fstream>
#include <unistd.h>
using namespace SeaDrip;

int DaemonUtil::SavePid( std::string pidpath )
{
    std::ofstream pid_file( pidpath, std::ios::out );
    if( !pid_file )
    {
        return 0;
    }
    int pid = getpid();
    pid_file << pid;
    pid_file.close();
    return pid;
}
