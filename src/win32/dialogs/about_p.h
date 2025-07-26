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
            AboutDialogImpl(AboutDialog& aboutDialog, ISharedDialog parent);
            ~AboutDialogImpl() = default;

            const std::string& title() const;
            void title(const std::string& value);
            const std::string& appDetails() const;
            void appDetails(const std::string& value);
            const std::string& description() const;
            void description(const std::string& value);
            const std::string& homePageLink() const;
            void homePageLink(const std::string& value);
            const std::string& releaseDate() const;
            void releaseDate(const std::string& value);
            const std::string& logoBitmapId() const;
            void logoBitmapId(const std::string& value);
            const std::string& logoIconId() const;
            void logoIconId(const std::string& value);

            void show();

        private:
            AboutDialog& _aboutDialog;
            ISharedDialog _parent;
            std::string _title;
            std::string _appDetails;
            std::string _description;
            std::string _homePageLink;
            std::string _releaseDate;
            std::string _logoBitmapId;
            std::string _logoIconId;
        };
    }
}
