#ifndef     __SD_LINUX_SIGMAP_H__
#define     __SD_LINUX_SIGMAP_H__

#include <map>
#include <string>
#include <signal.h>

namespace SeaDrip
{
    const std::map<std::string, int> linux_sig_map = {
        std::make_pair( "SIGHUP", SIGHUP ),         //  1   Hangup (POSIX)
        std::make_pair( "SIGINT", SIGINT ),         //  2   Interrupt (ANSI)
        std::make_pair( "SIGQUIT", SIGQUIT ),       //  3   Quit (POSIX)
        std::make_pair( "SIGILL", SIGILL ),         //  4   Illegal instruction (ANSI)
        std::make_pair( "SIGTRAP", SIGTRAP ),       //  5   Trace trap (POSIX)
        std::make_pair( "SIGABRT",SIGABRT ),        //  6   Abort (ANSI)
        std::make_pair( "SIGIOT", SIGIOT ),         //  6   IOT trap (4.2 BSD)
        std::make_pair( "SIGBUS", SIGBUS ),         //  7   BUS error (4.2 BSD)
        std::make_pair( "SIGFPE", SIGFPE ),         //  8   Floating-point exception (ANSI)
        std::make_pair( "SIGKILL", SIGKILL ),       //  9   Kill, unblockable (POSIX)
        std::make_pair( "SIGUSR1", SIGUSR1 ),       //  10  User-defined signal 1 (POSIX)
        std::make_pair( "SIGSEGV", SIGSEGV ),       //  11  Segmentation violation (ANSI)
        std::make_pair( "SIGUSR2", SIGUSR2 ),       //  12  User-defined signal 2 (POSIX)
        std::make_pair( "SIGPIPE", SIGPIPE ),       //  13  Broken pipe (POSIX)
        //  std::make_pair( "SIGALARM", SIGALARM ),     //  14  Alarm clock (POSIX)
        std::make_pair( "SIGTERM", SIGTERM ),       //  15  Termination (ANSI)
        std::make_pair( "SIGSTKFLT", SIGSTKFLT ),   //  16  Stack fault
        std::make_pair( "SIGCLD", SIGCLD ),         //  SIGCHLD    Same as SIGCHLD (System V)
        std::make_pair( "SIGCHLD", SIGCHLD ),       //  17  Child status has changed (POSIX)
        std::make_pair( "SIGCONT", SIGCONT ),       //  18  Continue (POSIX)
        std::make_pair( "SIGSTOP", SIGSTOP ),       //  19  Stop, unblockable (POSIX)
        std::make_pair( "SIGTSTP", SIGTSTP ),       //  20  Keyboard stop (POSIX)
        std::make_pair( "SIGTTIN", SIGTTIN ),       //  21  Background read from tty (POSIX)
        std::make_pair( "SIGTTOU", SIGTTOU ),       //  22  Background write to tty (POSIX)
        std::make_pair( "SIGURG", SIGURG ),         //  23  Urgent condition on socket (4.2 BSD)
        std::make_pair( "SIGXCPU", SIGXCPU ),       //  24  CPU limit exceeded (4.2 BSD)
        std::make_pair( "SIGXFSZ", SIGXFSZ ),       //  25  File size limit exceeded (4.2 BSD)
        std::make_pair( "SIGVTALRM", SIGVTALRM ),   //  26  Virtual alarm clock (4.2 BSD)
        std::make_pair( "SIGPROF", SIGPROF ),       //  27  Profiling alarm clock (4.2 BSD)
        std::make_pair( "SIGWINCH", SIGWINCH ),     //  28  Window size change (4.3 BSD, Sun)
        std::make_pair( "SIGPOLL", SIGPOLL ),       //  SIGIO  Pollable event occurred (System V)
        std::make_pair( "SIGIO", SIGIO ),           //  29  I/O now possible (4.2 BSD)
        std::make_pair( "SIGPWR", SIGPWR ),         //  30  Power failure restart (System V)
        std::make_pair( "SIGSYS", SIGSYS ),         //  31  Bad system call
        std::make_pair( "SIGUNUSED", SIGUNUSED )    //  31
    };

    //  int TransLinuxSignal( std::string );
};

#endif

