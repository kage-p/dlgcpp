#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class ITextBox : public virtual IControl
        {
        public:
            virtual int maxChars() const = 0;
            virtual void maxChars(int value) = 0;
            virtual bool password() const = 0;
            virtual void password(bool value) = 0;
            virtual bool readOnly() const = 0;
            virtual void readOnly(bool value) = 0;
            virtual bool multiline() const = 0;
            virtual void multiline(bool value) = 0;
            virtual bool wrapText() const = 0;
            virtual void wrapText(bool value) = 0;
            virtual HorizontalAlign horizontalAlignment() const = 0;
            virtual void horizontalAlignment(HorizontalAlign value) = 0;

            virtual IEvent<ISharedControl>& ChangedEvent() = 0;
        };

        typedef std::shared_ptr<ITextBox> ISharedTextBox;

        class TextBoxImpl;

        class TextBox :
            public Control,
            public ITextBox
        {
        public:
            explicit TextBox(
                const std::string& text = std::string(),
                const Position& p = Position());

            ~TextBox() override;

            // ITextBox impl.
            int maxChars() const override;
            void maxChars(int value) override;
            bool password() const override;
            void password(bool value) override;
            bool readOnly() const override;
            void readOnly(bool value) override;
            bool multiline() const override;
            void multiline(bool value) override;
            bool wrapText() const override;
            void wrapText(bool value) override;
            HorizontalAlign horizontalAlignment() const override;
            void horizontalAlignment(HorizontalAlign value) override;

            IEvent<ISharedControl>& ChangedEvent() override;

        private:
            TextBox(std::shared_ptr<TextBoxImpl> impl);

            std::shared_ptr<TextBoxImpl> _impl;
        };
    }
}
