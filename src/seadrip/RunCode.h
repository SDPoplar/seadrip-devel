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
    };
};

#define SD_RUNCODE( code ) static_cast<int>( code )

#endif
