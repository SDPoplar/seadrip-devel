#include "seadrip/DaemonLog.h"
#include <map>
#include <iostream>
#include <unistd.h>
using namespace SeaDrip;

const std::map<std::string, ELogLevel> levelstrmap = {
    std::make_pair( "NONE", ELogLevel::None ),
    std::make_pair( "ERROR", ELogLevel::Error ),
    std::make_pair( "WARN", ELogLevel::Warn ),
    std::make_pair( "INFO", ELogLevel::Info ),
    std::make_pair( "DEBUG", ELogLevel::Debug ),
};

DaemonLog::DaemonLog( std::string path ) : m_s_path( path ), m_file_log(),
    m_e_log_level( ELogLevel::Error ), m_set_force_save( {} ), m_set_unsave( {} )
{}

DaemonLog::~DaemonLog()
{
    if( this->m_file_log.is_open() )
    {
        this->m_file_log.close();
    }
}

bool DaemonLog::SetLogPath( std::string path )
{
    if( this->m_s_path == path )
    {
        return !path.empty();
    }
    if( this->m_file_log.is_open() )
    {
        this->m_file_log.close();
    }
    this->m_s_path = path;
    return this->IsLogPathCanWrite();
}

bool DaemonLog::SetLogLevel( std::string conf )
{
    if( conf.empty() )
    {
        return true;
    }
    auto match = levelstrmap.find( conf );
    return ( match != levelstrmap.end() ) &&  this->SetLogLevel( match->second );
}

bool DaemonLog::SetLogLevel( ELogLevel level, const std::set<ELogLevel> force,
    const std::set<ELogLevel> ignore )
{
    std::cout << "Setting log level to " << static_cast<int>( level ) << std::endl;
    this->m_e_log_level = level;
    if( !force.empty() )
    {
        this->m_set_force_save = force;
    }
    if( !ignore.empty() )
    {
        this->m_set_unsave = ignore;
    }
    return true;
}

bool DaemonLog::InForceSave( ELogLevel level ) const noexcept
{
    return this->m_set_force_save.find( level ) != this->m_set_force_save.end();
}

bool DaemonLog::InUnsave( ELogLevel level ) const noexcept
{
    return this->m_set_unsave.find( level ) != this->m_set_unsave.end();
}

bool DaemonLog::LevelShouldSave( ELogLevel level ) const noexcept
{
    if( this->InForceSave( level ) )
    {
        return true;
    }
    if( this->InUnsave( level ) )
    {
        return false;
    }
    return static_cast<int>( level ) <= static_cast<int>( this->m_e_log_level );
}

bool DaemonLog::IsLogPathCanWrite( void ) const noexcept
{
    std::string path = this->m_s_path.substr( 0, this->m_s_path.find_last_of( '/' ) );
    return !access( path.c_str(), F_OK | W_OK );
}

bool DaemonLog::Log( ELogLevel level, std::string content )
{
    if( !this->LevelShouldSave( level ) )
    {
        return false;
    }
    if( !this->m_file_log.is_open() && !this->m_s_path.empty() )
    {
        this->m_file_log.open( this->m_s_path, std::ios::out | std::ios::app );
    }
    if( !this->m_file_log.is_open() )
    {
        return false;
    }
        
    this->m_file_log << content << std::endl;
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
