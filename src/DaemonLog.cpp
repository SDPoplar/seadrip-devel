#include "seadrip/DaemonLog.h"
using namespace SeaDrip;

DaemonLog::DaemonLog( std::string path = "" ) : m_s_path( path ), m_file_log( nullptr ),
    m_e_log_level( ELogLevel::Error )
{}

DaemonLog::~DaemonLog()
{
    if( this->m_file_log )
    {
        if( this->m_file_log->is_open() )
        {
            this->m_file_log->close();
        }
        delete this->m_file_log;
        this->m_file_log = nullptr;
    }
}

bool DaemonLog::Init( const DaemonConfig& cfg )
{
    std::string cfg_log_path = cfg.GetLogPath();
    if( !cfg_log_path.empty() )
    {
        this->m_s_path = cfg_log_path;
    }
    if( this->m_file_log || this->m_s_path.empty() )
    {
        return false;
    }
    this->m_file_log = new std::ofstream( this->m_s_path, std::ios::out | std::ios::app );
    if( !this->m_file_log->is_open() )
    {
        return false;
    }
    //  load log level and force save
    return true;
}

bool DaemonLog::IsLogOpened() const noexcept
{
    return this->m_file_log && this->m_file_log->is_open();
}

bool DaemonLog::InForceSave( ELogLevel level ) const noexcept
{
    return this->m_set_force_save.find( level ) != this->m_set_force_save.end();
}

bool DaemonLog::Log( ELogLevel level, std::string content )
{
    if( !this->IsLogOpened() )
    {
        return false;
    }
    if( (static_cast<int>(level) < static_cast<int>(this->m_e_log_level)) && !this->InForceSave( level ) )
    {
        return false;
    }
        
    *(this->m_file_log) << content;
    return true;
}

bool DaemonLog::Error( std::string content )
{
    return this->Log( ELogLevel::Error, "[Error]" + content );
}

bool DaemonLog::Warn( std::string content )
{
    return this->Log( ELogLevel::Warn, "[Warn]" + content );
}

bool DaemonLog::Info( std::string content )
{
    return this->Log( ELogLevel::Info, "[Info]" + content );
}

bool DaemonLog::Debug( std::string content )
{
    return this->Log( ELogLevel::Debug, "[Debug]" + content );
}
