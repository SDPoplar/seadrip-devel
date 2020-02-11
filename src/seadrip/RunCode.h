#ifndef __SD_RUN_CODE_H__
#define __SD_RUN_CODE_H__

#define SDRUNCODE( name, val )  public: static const int name = val

namespace SeaDrip
{
    class CoreRunCode
    {
        SDRUNCODE( OK, 0 );
    };

    class DaemonCoreRunCode : public CoreRunCode
    {
        SDRUNCODE( EMPTY_PID_PATH, 1 );
        SDRUNCODE( PID_FILE_CANNOT_SAVE, 2 );

        SDRUNCODE( CREATE_DAEMON_FAILED, 3 );
        SDRUNCODE( SET_LOG_PATH_FAILED, 4 );
        SDRUNCODE( SET_LOG_LEVEL_FAILED, 5 );
        SDRUNCODE( NO_QUIT_METHOD_SETTED, 6 );
    };

    class SocketDaemonCoreRunCode : public DaemonCoreRunCode
    {
        SDRUNCODE( CREATE_SOCK_FAILED, 10 );
        SDRUNCODE( NO_PORT_SETTED, 11 );
        SDRUNCODE( BIND_PORT_FAILED, 12 );
        SDRUNCODE( PORT_CONFLICT, 13 );
    };

};

#undef SDRUNCODE

#endif
