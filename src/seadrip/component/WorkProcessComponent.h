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
        ERROR,
    };

    enum class WorkProcessEvents : int
    {
        READY,
        ERROR,
    };

    enum class WorkProcessOptions : int
    {
        STOP,

        USER_OP_1,
        USER_OP_2,
    };

    class WorkProcessNoticePack
    {
    public:
        WorkProcessNoticePack( const int op, const sigval data );

        const int GetOption() const noexcept;
        const sigval GetData() const noexcept;
        const int GetDataAsInt() const noexcept;
        const void* GetDataAsPtr() const noexcept;
    protected:
        int m_n_op;
        sigval m_union_data;
    };

    class WorkProcessEventPack : public WorkProcessNoticePack
    {
    public:
        WorkProcessEventPack( const int event, const sigval data );
        const int FromProcess() const noexcept;

    protected:
        int m_n_pid;
    };

    typedef void(*WorkProcessClientEventProc)(class WorkProcessClient*, sigval);
    class WorkProcessClient
    {
    public:
        WorkProcessClient( const int parentPid );

        const bool Init( void(*handler)(int, siginfo_t*, void*), const std::map<int, WorkProcessClientEventProc> *workmap );
        const bool ReportReady();
        const bool ReportError( const char* message );
        const bool ClientRunning() const noexcept;
        void Work( WorkProcessNoticePack* event );
        void Stop();

    protected:
        const int GetParentPid() const noexcept;
        const bool ReportEvent( const int op, const sigval data );

    private:
        int m_n_parent_pid;
        int m_b_client_running;
        const std::map<int, WorkProcessClientEventProc>* m_p_method_dict;
    };

    class WorkProcessComponent
    {
    public:
        ~WorkProcessComponent();

        bool Fork( const char* worker, const char* withUser, const int workerNum );
        int PopFreeWorker();
        bool NoticeFreeWorker( const int op, const sigval data );
        void OnEvent( WorkProcessEventPack* event );
        void OnWorkProcessReady( const int processPid );

    protected:
        bool NoticeWorker( const int workerPid, const int op, const sigval data );

    private:
        std::map<int, WorkProcessStatus> m_map_worker;
        std::vector<int> m_queue_free;
    };
};

#endif
