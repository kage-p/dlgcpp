#pragma once

#include "dlgcpp/defs.h"

namespace dlgcpp
{
    class KeyMapper
    {
    public:
        static Key ToKey(unsigned int vk);
    };
}