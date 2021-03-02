#include "seadrip/config/SocketDaemonConfig.h"
using namespace SeaDrip;

//  ========================    SocketConfig    ============================================
SocketConfig::SocketConfig( int port, std::string addr )
    : m_s_listen_addr( addr ), m_n_listen_port( port ), m_n_work_process_num( 1 )
{}

const int SocketConfig::GetListenPort() const noexcept
{
    return this->m_n_listen_port.Get();
}

SocketConfig &SocketConfig::SetListenPort( int port, EConfigSetFrom from )
{
    this->m_n_listen_port.Set( from, port );
    return *this;
}

const std::string SocketConfig::GetListenAddr( void ) const noexcept
{
    return this->m_s_listen_addr.Get();
}

SocketConfig &SocketConfig::SetListenAddr( const std::string &listen_addr, EConfigSetFrom from )
{
    this->m_s_listen_addr.Set( from, listen_addr );
    return *this;
}

const int SocketConfig::GetWorkProcessNum( void ) const noexcept
{
    return this->m_n_work_process_num.Get();
}

SocketConfig &SocketConfig::SetWorkProcessNum( const int num, EConfigSetFrom from )
{
    this->m_n_work_process_num.Set( from, num );
    return *this;
}

//  =====================   SocketDaemonConfig  ============================================

SocketDaemonConfig::SocketDaemonConfig( std::string def_cfg_path, std::string def_pid_path, int def_listen_port, std::string def_listen_addr )
    : DaemonConfig( def_cfg_path, def_pid_path ), SocketConfig( def_listen_port, def_listen_addr )
{}

const std::string SocketDaemonConfig::GetShellOptions( void )
{
    return std::string( DaemonConfig::GetShellOptions() ).append( "h:p:" );
}

const bool SocketDaemonConfig::SetShellOption( const char item, const char* val )
{
    switch( item )
    {
        case 'h':
            this->SetListenAddr( val,  EConfigSetFrom::SHELL );
            return true;
        case 'p':
            this->SetListenPort( atoi( val ), EConfigSetFrom::SHELL );
            return true;
        default:
            return DaemonConfig::SetShellOption( item, val );
    }
}

const bool SocketDaemonConfig::SetConfigOption( const std::string key, const std::string value )
{
    if( key == "listen_addr" )
    {
        this->SetListenAddr( value, EConfigSetFrom::CFGFILE );
        return true;
    }
    if( key == "listen_port" )
    {
        this->SetListenPort( atoi( value.c_str() ), EConfigSetFrom::CFGFILE );
        return true;
    }
    if( key == "work_process_num" )
    {
        this->SetWorkProcessNum( atoi( value.c_str() ), EConfigSetFrom::CFGFILE );
        return true;
    }
    return DaemonConfig::SetConfigOption( key, value );
}