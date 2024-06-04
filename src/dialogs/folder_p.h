#pragma once

#include "folder.h"

namespace dlgcpp
{
    namespace dialogs
    {
        struct folder_props
        {
            std::shared_ptr<IDialog> parent;
            std::string title;
            std::string message;
            std::string folderName;
        };
    }
}
