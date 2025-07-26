#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class TextBoxImpl;

        class TextBox : public Control
        {
        public:
            explicit TextBox(
                const std::string& text = std::string(),
                const Position& p = Position());

            ~TextBox() override;

            int maxChars() const;
            void maxChars(int value);
            bool password() const;
            void password(bool value);
            bool readOnly() const;
            void readOnly(bool value);
            bool multiline() const;
            void multiline(bool value);
            bool wrapText() const;
            void wrapText(bool value);
            HorizontalAlign horizontalAlignment() const;
            void horizontalAlignment(HorizontalAlign value);

            IEvent<ISharedControl>& ChangedEvent();

        private:
            TextBox(std::shared_ptr<TextBoxImpl> impl);

            std::shared_ptr<TextBoxImpl> _impl;
        };
    }
}
