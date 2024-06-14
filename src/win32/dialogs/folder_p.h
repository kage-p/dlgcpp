#pragma once

#include "dlgcpp/dialogs/folder.h"

namespace dlgcpp
{
    namespace dialogs
    {
        struct folder_props
        {
            ISharedDialog parent;
            std::string title;
            std::string message;
            std::string folderName;
        };
    }
}
