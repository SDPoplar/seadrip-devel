#ifndef     __SD_DAEMON_LOG_H__
#define     __SD_DAEMON_LOG_H__

#include <set>
#include <string>
#include <fstream>

namespace SeaDrip
{
    enum class ELogLevel : int
    {
        None        = 0,
        Error       = 1,
        Warn        = 2,
        Info        = 3,
        Debug       = 4,
    };

    class DaemonLog
    {
    public:
        DaemonLog( std::string path = "" );
        virtual ~DaemonLog();

        bool SetLogPath( std::string path );
        bool SetLogLevel( std::string conf );
        bool SetLogLevel( ELogLevel level, const std::set<ELogLevel> force = {},
                const std::set<ELogLevel> ignore = {} );

        bool Error( std::string content );
        bool Warn( std::string content );
        bool Info( std::string content );
        bool Debug( std::string content );

    protected:
        bool Log( ELogLevel, std::string content );

        bool InForceSave( ELogLevel level ) const noexcept;
        bool InUnsave( ELogLevel level ) const noexcept;
        bool LevelShouldSave( ELogLevel level ) const noexcept;
        bool IsLogPathCanWrite( void ) const noexcept;

        std::string m_s_path;
        std::ofstream m_file_log;
        ELogLevel m_e_log_level;
        std::set<ELogLevel> m_set_force_save;
        std::set<ELogLevel> m_set_unsave;
    };
};

#endif

