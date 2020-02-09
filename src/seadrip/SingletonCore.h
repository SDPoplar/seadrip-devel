#ifndef __SD_SINGLETON_CORE_H__
#define __SD_SINGLETON_CORE_H__

#include "Singleton.hpp"
#include "TickCore.h"

namespace SeaDrip
{
    class SingletonCore : public TickCore
    {
        AS_SD_SINGLETON;

    private:
        SingletonCore();
    };

};

#endif
