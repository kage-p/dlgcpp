#pragma once

#include "dlgcpp/dialogs/dialog.h"
#include "dlgcpp/dialogs/splash.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace dialogs
    {
        class SplashDialogImpl
        {
        public:
            explicit SplashDialogImpl(SplashDialog* splashDialog);
            virtual ~SplashDialogImpl() = default;

            void show();
            void close();

        private:
            SplashDialog* _splashDialog = nullptr;
            std::shared_ptr<Dialog> _dialog;
        };
    }
}
