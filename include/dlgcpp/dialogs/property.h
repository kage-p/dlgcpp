#pragma once

#include "dlgcpp/defs.h"
#include <string>

namespace dlgcpp
{
    namespace dialogs
    {
        enum class PropertyDialogOption
        {
            Check = 0,
            List,
            Text,
        };

        class IPropertyDialog
        {
        public:
            virtual const std::string& title() const = 0;
            virtual void title(const std::string& value) = 0;
            virtual const std::string& message() const = 0;
            virtual void message(const std::string& value) = 0;
            virtual int sectionWidth() const = 0;
            virtual void sectionWidth(int value) = 0;
            virtual bool show() = 0;
        };

        class PropertyDialogImpl;

        class PropertyDialog : public IPropertyDialog
        {
        public:
            explicit PropertyDialog(ISharedDialog parent = nullptr);
            virtual ~PropertyDialog() = default;

            // IPropertyDialog impl.
            const std::string& title() const override;
            void title(const std::string& value) override;
            const std::string& message() const override;
            void message(const std::string& value) override;
            int sectionWidth() const override;
            void sectionWidth(int value) override;
            bool show() override;

        private:
            PropertyDialog(std::shared_ptr<PropertyDialogImpl> impl);

            std::shared_ptr<PropertyDialogImpl> _impl;
        };
    }
}
