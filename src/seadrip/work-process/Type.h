#ifndef __SD_WORKPROC_TYPE_H__
#define __SD_WORKPROC_TYPE_H__

namespace SeaDrip
{
    namespace WorkProcess
    {
        enum class ClientStatus : int
        {
            CREATE,
            READY,
            ERROR,
            STOPED,
        };

        // task is from master to client
        class Task
        {
        public:
            const static int OPTION_QUIT = 1;
            const static Task QuitTask;

            Task( const int option );
            const int GetOption() const noexcept;

        private:
            int m_n_option;
        };

        // event is from client to master
        class Event
        {
        public:
            const static int EVENT_ERROR = 1;
            const static Event ErrorEvent;

            Event( const int event );
            const int GetClient() const noexcept;
            const int GetEvent() const noexcept;
        
        private:
            int m_n_event;
            int m_n_client_pid;
        };

        class SockPair
        {
        public:
            SockPair( const int in_sock = 0, const int out_sock = 0 );
            SockPair( const SockPair& copy );
        
        protected:
            const int GetInSock() const noexcept;
            const int GetOutSock() const noexcept;

        private:
            int m_sock_in;
            int m_sock_out;
        };
    }
}

#endif
