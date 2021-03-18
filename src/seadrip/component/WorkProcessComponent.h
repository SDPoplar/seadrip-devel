#ifndef __SD_COMPONENT_ATOM_WORK_PROCESS_H__
#define __SD_COMPONENT_ATOM_WORK_PROCESS_H__

#include <vector>
#include <map>
#include <signal.h>

namespace SeaDrip
{
    enum class WorkProcessStatus : int
    {
        CREATE,
        READY,
        ZOMBIE,
    };

    enum class WorkProcessEvents : int
    {
        READY,
    };

    enum class WorkProcessOptions : int
    {
        STOP,
    };

    class WorkProcessNoticePack
    {
    public:
        WorkProcessNoticePack( const int op, const sigval data );

        const int GetDataAsInt() const noexcept;
        const void* GetDataAsPtr() const noexcept;
    protected:
        int op;
        sigval data;
    };

    class WorkProcessComponent
    {
    public:
        ~WorkProcessComponent();

        bool Fork( const char* worker, const char* withUser, const int workerNum );
        int PopFreeWorker();
        bool NoticeFreeWorker( const int op, const sigval data );

    protected:
        bool NoticeWorker( const int workerPid, const int op, const sigval data );

    private:
        std::map<int, WorkProcessStatus> m_map_worker;
        std::vector<int> m_queue_free;
    };
};

#endif
