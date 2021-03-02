#include "../seadrip/config/atom/DebugConfig.h"
using namespace SeaDrip;

DebugConfig::DebugConfig( const bool isDebug ) : m_b_debug( isDebug )
{}

const bool DebugConfig::IsDebug( void ) const noexcept
{
    return this->m_b_debug.Get();
}

DebugConfig& DebugConfig::SetDebug( const bool flag, const EConfigSetFrom from )
{
    this->m_b_debug.Set( from, flag );
    return *this;
}
