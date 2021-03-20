#include "../seadrip/config/atom/WorkProcessConfig.h"
using namespace SeaDrip;

WorkProcessConfig::WorkProcessConfig( const int workProcessNum, const std::string withAccount, const std::string withAccountGroup )
    : m_n_work_process_num( workProcessNum ), m_s_with_account( withAccount ), m_s_with_account_group( withAccountGroup )
{}

const int WorkProcessConfig::GetWorkProcessNum() const noexcept
{
    return this->m_n_work_process_num.Get();
}

WorkProcessConfig& WorkProcessConfig::SetWorkProcessNum( const int num, const EConfigSetFrom from )
{
    this->m_n_work_process_num.Set( from, num );
    return *this;
}

const std::string WorkProcessConfig::GetWorkAccount() const noexcept
{
    return this->m_s_with_account.Get();
}

WorkProcessConfig& WorkProcessConfig::SetWorkAccount( const std::string withAccount, const EConfigSetFrom from )
{
    this->m_s_with_account.Set( from, withAccount );
    return *this;
}

const std::string WorkProcessConfig::GetWorkAccountGroup() const noexcept
{
    return this->m_s_with_account_group.Get();
}

WorkProcessConfig& WorkProcessConfig::SetWorkAccountGroup( const std::string accountGroup, const EConfigSetFrom from )
{
    this->m_s_with_account_group.Set( from, accountGroup );
    return *this;
}
