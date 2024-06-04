#pragma once

#include "dlgcpp/dialogs/splash.h"

namespace dlgcpp
{
    namespace dialogs
    {
        struct splash_props
        {
            std::shared_ptr<IDialog> parent;
            std::string logoBitmapId;
            std::string message;
            int timeout = 800;
            std::shared_ptr<Dialog> splashDialog;
        };
    }
}
