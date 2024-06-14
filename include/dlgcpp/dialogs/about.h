#pragma once

#include "dlgcpp/dlgcpp.h"

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
            virtual const std::string& logoIconId() const = 0;
            virtual void logoIconId(const std::string& value) = 0;
            virtual void show() = 0;
        };

        class AboutDialog : public IAboutDialog
        {
        public:
            explicit AboutDialog(ISharedDialog parent = nullptr);
            virtual ~AboutDialog();

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
            const std::string& logoIconId() const override;
            void logoIconId(const std::string& value) override;
            void show() override;

        private:
            struct about_props* _props;
        };
    }
}
