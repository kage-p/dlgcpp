#pragma once

#include "dlgcpp/defs.h"
#include "dlgcpp/property.h"

#include <string>

namespace dlgcpp
{
    namespace dialogs
    {
        class IAboutDialog;
        class AboutDialogImpl;
        typedef std::shared_ptr<IAboutDialog> ISharedAboutDialog;

        class IAboutDialog
        {
        public:
            virtual IReadOnlyProperty<ISharedDialog, ISharedAboutDialog>& parent() = 0;
            virtual IProperty<std::string, ISharedAboutDialog>& title() = 0;
            virtual IProperty<std::string, ISharedAboutDialog>& appDetails() = 0;
            virtual IProperty<std::string, ISharedAboutDialog>& description() = 0;
            virtual IProperty<std::string, ISharedAboutDialog>& homePageLink() = 0;
            virtual IProperty<std::string, ISharedAboutDialog>& releaseDate() = 0;
            virtual IProperty<ImageSource, ISharedAboutDialog>& logoImage() = 0;

            virtual void show() = 0;
        };

        class AboutDialog :
            public IAboutDialog,
            public std::enable_shared_from_this<AboutDialog>
        {
        public:
            explicit AboutDialog(ISharedDialog parent = nullptr);
            virtual ~AboutDialog() = default;

            // IAboutDialog impl.
            IReadOnlyProperty<ISharedDialog, ISharedAboutDialog>& parent() override;
            IProperty<std::string, ISharedAboutDialog>& title() override;
            IProperty<std::string, ISharedAboutDialog>& appDetails() override;
            IProperty<std::string, ISharedAboutDialog>& description() override;
            IProperty<std::string, ISharedAboutDialog>& homePageLink() override;
            IProperty<std::string, ISharedAboutDialog>& releaseDate() override;
            IProperty<ImageSource, ISharedAboutDialog>& logoImage() override;

            void show() override;

        private:
            AboutDialog(std::shared_ptr<AboutDialogImpl> impl, ISharedDialog parent);

            std::shared_ptr<AboutDialogImpl> _impl;
            Property<ISharedDialog, ISharedAboutDialog> _parent;
            Property<std::string, ISharedAboutDialog> _title;
            Property<std::string, ISharedAboutDialog> _appDetails;
            Property<std::string, ISharedAboutDialog> _description;
            Property<std::string, ISharedAboutDialog> _homePageLink;
            Property<std::string, ISharedAboutDialog> _releaseDate;
            Property<ImageSource, ISharedAboutDialog> _logoImage;
        };
    }
}
