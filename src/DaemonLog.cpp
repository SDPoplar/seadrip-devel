#include "seadrip/DaemonLog.h"
using namespace SeaDrip;

bool DaemonLog::~DaemonLog()
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
    this->m_s_path = cfg.GetLogPath();
    if( this->m_file_log || this->m_s_path.empty() )
    {
        return false;
    }
    this->m_file_log = new std::ofstream( this->m_s_path, std::ios::out | std::ios::app );
    return this->m_file_log->is_open();
}

bool DaemonLog::IsLogOpened() const noexcept
{
    return this->m_file_log && this->m_file_log->is_open();
}

bool DaemonLog::InForceSave( ELogLevel level )
{
    return std::find( this->m_arr_force_save.begin(), this->m_arr_force_save.end(), level )
        != this->m_arr_force_save.end();
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

bool DaemonLog::Info( std::string content )
{
    return this->Log( ELogLevel::Debug, "[Debug]" + content );
}
