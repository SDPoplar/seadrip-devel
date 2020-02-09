#include "seadrip/GlobalCore.h"
#include "seadrip/RunCode.h"
using namespace SeaDrip;

SingletonCore::SingletonCore() : m_b_run_switch( false ), m_n_runcode( CoreRunCode::OK )
{}

bool SingletonCore::IsRunning() const noexcept
{
    return this->m_b_run_switch;
}

int SingletonCore::Run()
{
    this->m_b_run_switch = this->ReadyToRun();
    while( this->m_b_run_switch )
    {
        this->Tick();
    }
    this->Release();
    return this->m_n_runcode;
}

void SingletonCore::Quit()
{
    this->m_b_run_switch = false;
    this->PreQuit();
}

bool SingletonCore::ReadyToRun()
{
    return true;
}

void SingletonCore::PreQuit()
{}

void SingletonCore::Release()
{}

void SingletonCore::Tick()
{}
