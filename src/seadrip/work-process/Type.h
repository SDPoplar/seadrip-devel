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
        {};

        // event is from client to master
        class Event
        {};

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
