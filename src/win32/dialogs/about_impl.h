#pragma once

#include "dlgcpp/dialogs/about.h"

#include <string>

namespace dlgcpp
{
    namespace dialogs
    {
        class AboutDialogImpl
        {
        public:
            AboutDialogImpl(AboutDialog* aboutDialog);
            ~AboutDialogImpl() = default;

            void openUrl(ISharedDialog parent, const std::string& url) const;
            void openSystemInfo(ISharedDialog parent);

        private:
            AboutDialog* _aboutDialog = nullptr;
        };
    }
}
