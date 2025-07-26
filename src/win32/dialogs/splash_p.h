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
            explicit SplashDialogImpl(SplashDialog& splashDialog, ISharedDialog parent = nullptr);
            virtual ~SplashDialogImpl() = default;

            const std::string& logoBitmapId() const;
            void logoBitmapId(const std::string& value);
            const std::string& message() const;
            void message(const std::string& value);
            int timeout() const;
            void timeout(int value);
            void show();
            void close();

        private:
            SplashDialog& _splashDialog;
            ISharedDialog _parent;
            std::string _logoBitmapId;
            std::string _message;
            int _timeout = 800;
            std::shared_ptr<Dialog> _dialog;
        };
    }
}
