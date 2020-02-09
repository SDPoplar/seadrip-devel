#include "seadrip/SingletonCore.h"
#include "seadrip/RunCode.h"
using namespace SeaDrip;

BaseCore::BaseCore() : m_b_run_switch( false ), m_n_runcode( CoreRunCode::OK )
{}

bool BaseCore::IsRunning() const noexcept
{
    return this->m_b_run_switch;
}

int BaseCore::Run()
{
    this->m_b_run_switch = this->ReadyToRun();
    while( this->m_b_run_switch )
    {
        this->Tick();
    }
    this->Release();
    return this->m_n_runcode;
}

void BaseCore::Quit()
{
    this->m_b_run_switch = false;
    this->PreQuit();
}

bool BaseCore::ReadyToRun()
{
    return true;
}

void BaseCore::PreQuit()
{}

void BaseCore::Release()
{}

void BaseCore::Tick()
{}

SingletonCore::SingletonCore()
{}
