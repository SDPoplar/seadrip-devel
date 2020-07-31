#ifndef     __SD_SHELL_DAEMON_H__
#define     __SD_SHELL_DAEMON_H__

#include <string>
namespace SeaDrip
{
    class DaemonUtil
    {
    public:
        static int SavePid( std::string pidpath );
    };
}

#endif