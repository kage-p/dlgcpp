#pragma once

#include "dlgcpp/dlgcpp.h"

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

        class PropertyDialog : public IPropertyDialog
        {
        public:
            explicit PropertyDialog(ISharedDialog parent = nullptr);
            virtual ~PropertyDialog();

            // IPropertyDialog impl.
            const std::string& title() const override;
            void title(const std::string& value) override;
            const std::string& message() const override;
            void message(const std::string& value) override;
            int sectionWidth() const override;
            void sectionWidth(int value) override;
            bool show() override;

        private:
            struct propdlg_props* _props;
            struct propdlg_state* _state;
        };
    }
}
