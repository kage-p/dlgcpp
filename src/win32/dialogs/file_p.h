#pragma once

#include "dlgcpp/dialogs/file.h"

namespace dlgcpp
{
    namespace dialogs
    {
        struct file_props
        {
            ISharedDialog parent;
            std::string fileName;
            std::string filters;
            std::string title;
            int filterIndex = 0;
            int extErr = 0;
        };
    }
}
