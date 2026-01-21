#pragma once

#include "dlgcpp/defs.h"
#include "dlgcpp/property.h"
#include <string>

namespace dlgcpp
{
    namespace dialogs
    {
        class ISplashDialog;
        class SplashDialogImpl;
        typedef std::shared_ptr<ISplashDialog> ISharedSplashDialog;

        class ISplashDialog
        {
        public:
            virtual IReadOnlyProperty<ISharedDialog, ISharedSplashDialog>& parent() = 0;
            virtual IProperty<std::string, ISharedSplashDialog>& message() = 0;
            virtual IProperty<ImageSource, ISharedSplashDialog>& logoImage() = 0;
            virtual IProperty<int, ISharedSplashDialog>& timeout() = 0;

            virtual void show() = 0;
            virtual void close() = 0;
        };

        class SplashDialog :
            public ISplashDialog,
            public std::enable_shared_from_this<SplashDialog>
        {
        public:
            explicit SplashDialog(ISharedDialog parent = nullptr);
            virtual ~SplashDialog() = default;

            // ISplashDialog impl.
            IReadOnlyProperty<ISharedDialog, ISharedSplashDialog>& parent() override;
            IProperty<std::string, ISharedSplashDialog>& message() override;
            IProperty<ImageSource, ISharedSplashDialog>& logoImage() override;
            IProperty<int, ISharedSplashDialog>& timeout() override;
            void show() override;
            void close() override;

        private:
            SplashDialog(std::shared_ptr<SplashDialogImpl> impl, ISharedDialog parent);

            std::shared_ptr<SplashDialogImpl> _impl;
            Property<ISharedDialog, ISharedSplashDialog> _parent;
            Property<std::string, ISharedSplashDialog> _message;
            Property<ImageSource, ISharedSplashDialog> _logoImage;
            Property<int, ISharedSplashDialog> _timeout;
        };
    }
}
