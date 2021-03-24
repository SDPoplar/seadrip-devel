#ifndef __SD_WORKPROC_CLIENT_H__
#define __SD_WORKPROC_CLIENT_H__

#include "Type.h"

namespace SeaDrip
{
    namespace WorkProcess
    {
        class Client : public SockPair
        {
        public:
            Client( const int parent, const int in_sock, const int out_sock );
            const bool IsClientRunning() const noexcept;
            int Listen();

        protected:
            const bool WellLoaded() const noexcept;
            
        private:
            int m_n_parent_pid;
            bool m_b_running;
        };
    }
}

#endif
