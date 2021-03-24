#ifndef __SD_WORKPROC_MASTER_H__
#define __SD_WORKPROC_MASTER_H__

#include <map>
#include <vector>

#include "Type.h"

namespace SeaDrip
{
    namespace WorkProcess
    {
        class Worker : public SockPair
        {
        public:
            Worker( const int in_sock = 0, const int out_sock = 0 );
            Worker( const Worker& copy );
            Worker& operator = ( const ClientStatus status );
            Worker& operator << ( const Task task );
        private:
            ClientStatus m_e_status;
        };

        class Master
        {
        public:
            const bool Fork( const char* worker, const char* useAccount, const int workerNum );
        protected:
        private:
            std::map<int, Worker> m_map_clients;
            std::vector<int> m_queue_free;
        };
    }
}

#endif
