#ifndef __SD_GLOBAL_CORE_H__
#define __SD_GLOBAL_CORE_H__

#include "Singleton.hpp"

namespace SeaDrip
{
    class SingletonCore
    {
        AS_SD_SINGLETON;

        public:
            bool IsRunning() const noexcept;
            int Run();
            void Quit();

        protected:
            void Error( int code );

            virtual bool ReadyToRun();
            virtual void Release();
            virtual void Tick();
            virtual void PreQuit();

        private:
            SingletonCore();
            bool m_b_run_switch;
            int m_n_runcode;
    };
};

#endif
