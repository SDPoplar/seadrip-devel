#include "seadrip/GlobalCore.hpp"
using namespace SeaDrip;

bool SingletonCore<T>::IsRunning() const noexcept
{
    return this->m_b_run_switch;
}
