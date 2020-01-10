#ifndef __SD_RUN_CODE_H__
#define __SD_RUN_CODE_H__

namespace SeaDrip
{
    enum class ECoreRunCode : int
    {
        OK = 0,
    };

    enum class EDaemonCoreRunCode : int
    {
        EMPTY_PID_PATH = 1,
        PID_FILE_CANNOT_SAVE = 2,

        CREATE_DAEMON_FAILED = 3,
        SET_LOG_PATH_FAILED = 4,
        SET_LOG_LEVEL_FAILED = 5,
    };

    enum class ESocketDaemonCoreRunCode : int
    {
        CREATE_SOCK_FAILED = 10,
        NO_PORT_SETTED = 11,
        BIND_PORT_FAILED = 12,
        PORT_CONFLICT = 13,
    };
};

#define SD_RUNCODE( code ) static_cast<int>( code )

#endif
