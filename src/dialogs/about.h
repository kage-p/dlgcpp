#pragma once

#include "dlgcpp.h"

#include <string>

namespace dlgcpp
{
    namespace dialogs
    {
        class IAboutDialog
        {
        public:
            virtual const std::string& title() const = 0;
            virtual void title(const std::string& value) = 0;
            virtual const std::string& appDetails() const = 0;
            virtual void appDetails(const std::string& value) = 0;
            virtual const std::string& description() const = 0;
            virtual void description(const std::string& value) = 0;
            virtual const std::string& homePageLink() const = 0;
            virtual void homePageLink(const std::string& value) = 0;
            virtual const std::string& releaseDate() const = 0;
            virtual void releaseDate(const std::string& value) = 0;
            virtual const std::string& logoBitmapId() const = 0;
            virtual void logoBitmapId(const std::string& value) = 0;
            virtual void show() = 0;
        };

        class AboutDialog : public IAboutDialog
        {
        public:
            explicit AboutDialog(std::shared_ptr<IDialog> parent = nullptr);

            // IAboutDialog impl.
            const std::string& title() const override;
            void title(const std::string& value) override;
            const std::string& appDetails() const override;
            void appDetails(const std::string& value) override;
            const std::string& description() const override;
            void description(const std::string& value) override;
            const std::string& homePageLink() const override;
            void homePageLink(const std::string& value) override;
            const std::string& releaseDate() const override;
            void releaseDate(const std::string& value) override;
            const std::string& logoBitmapId() const override;
            void logoBitmapId(const std::string& value) override;
            void show() override;

        private:
            std::shared_ptr<IDialog> _parent;

            std::shared_ptr<dlgcpp::controls::Image> _logoImage;
            std::shared_ptr<dlgcpp::controls::Label> _appDetails;
            std::shared_ptr<dlgcpp::controls::TextBox> _description;
            std::shared_ptr<dlgcpp::controls::Label> _releaseInfo;
            std::shared_ptr<dlgcpp::controls::Label> _webLink;
            std::shared_ptr<dlgcpp::controls::Button> _closeButton;
            std::shared_ptr<dlgcpp::controls::Button> _sysInfoButton;
            std::shared_ptr<dlgcpp::controls::Label> _copyrightLabel;

            std::string _titleText;
            std::string _appDetailsText;
            std::string _descriptionText;
            std::string _homePageLink;
            std::string _releaseDateText;
            std::string _logoBitmapId;
        };
    }
}
