#include "seadrip/TickCore.h"
#include "seadrip/RunCode.h"
using namespace SeaDrip;

TickCore::TickCore() : m_b_run_switch( false ), m_n_runcode( CoreRunCode::OK )
{}

bool TickCore::IsRunning() const noexcept
{
    return this->m_b_run_switch;
}

int TickCore::Run()
{
    this->m_b_run_switch = this->ReadyToRun();
    while( this->m_b_run_switch )
    {
        this->Tick();
    }
    this->Release();
    return this->m_n_runcode;
}

void TickCore::Quit()
{
    this->m_b_run_switch = false;
    this->PreQuit();
}

bool TickCore::ReadyToRun()
{
    return true;
}

void TickCore::PreQuit()
{}

void TickCore::Release()
{}

void TickCore::Tick()
{}

void TickCore::Error( int code )
{
    this->m_n_runcode = code;
}