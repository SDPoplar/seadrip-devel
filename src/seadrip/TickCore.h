#ifndef __SD_TICK_CORE_H__
#define __SD_TICK_CORE_H__

namespace SeaDrip
{
    class TickCore
    {
        public:
            bool IsRunning() const noexcept;
            int Run();
            void Quit();

        protected:
            TickCore();
            void Error( int code );

            virtual bool ReadyToRun();
            virtual void Release();
            virtual void Tick();
            virtual void PreQuit();

        private:
            bool m_b_run_switch;
            int m_n_runcode;
    };
}

#define _SDCORE_RET_FALSE( exp, dosth ) do { if( exp ) { dosth return false; } } while( false )
#define SDCORE_RET_FALSE( exp ) _SDCORE_RET_FALSE( exp, ; )
#define SDCORE_RET_FALSE_ERR( exp, code ) _SDCORE_RET_FALSE( exp, this->Error( code ); )

#endif
