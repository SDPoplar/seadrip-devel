#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "ConfigProperty.hpp"

namespace SeaDrip
{
    enum class ELogLevel : uint8_t
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
        DaemonLog( std::string path ) : m_s_path( "" ), m_file_log( nullptr ) = default;
        virtual ~DaemonLog();

        bool Init( const DaemonConfig& cfg );

        bool Error( std::string content );
        bool Warn( std::string content );
        bool Info( std::string content );
        bool Debug( std::string content );

    protected:
        bool Log( ELogLevel, std::string content );

        bool IsLogOpened() const noexcept;
        bool InForceSave( ELogLevel level ) const noexcept;

        std::string m_s_path;
        std::ofstream* m_file_log;
        ELogLevel m_e_log_level;
        std::vector<ELogLevel> m_arr_force_save;
    };
};