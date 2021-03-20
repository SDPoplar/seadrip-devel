#ifndef  __SD_CONF_ATOM_WORK_PROCESS_H__
#define  __SD_CONF_ATOM_WORK_PROCESS_H__

#include "../base/ConfigProperty.hpp"

namespace SeaDrip
{
    class WorkProcessConfig
    {
    public:
        WorkProcessConfig( const int workProcessNum, const std::string withAccount, const std::string withAccountGroup );
        WorkProcessConfig( const WorkProcessConfig& copy ) = default;
        virtual ~WorkProcessConfig() = default;

        const int GetWorkProcessNum() const noexcept;
        WorkProcessConfig& SetWorkProcessNum( const int num, const EConfigSetFrom from = EConfigSetFrom::RUNTIME );

        const std::string GetWorkAccount() const noexcept;
        WorkProcessConfig& SetWorkAccount( const std::string withAccount, const EConfigSetFrom from = EConfigSetFrom::RUNTIME );

        const std::string GetWorkAccountGroup() const noexcept;
        WorkProcessConfig& SetWorkAccountGroup( const std::string accountGroup, EConfigSetFrom from = EConfigSetFrom::RUNTIME );

    protected:
        SeaDrip::TConfigProperty<std::string> m_s_with_account_group;
        SeaDrip::TConfigProperty<std::string> m_s_with_account;
        SeaDrip::TConfigProperty<int> m_n_work_process_num;
    };
};

#endif
