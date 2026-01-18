#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class ITextBox : public virtual IControl
        {
        public:
            virtual IProperty<int, ISharedControl>& maxChars() = 0;
            virtual IProperty<bool, ISharedControl>& password() = 0;
            virtual IProperty<bool, ISharedControl>& readOnly() = 0;
            virtual IProperty<bool, ISharedControl>& multiline() = 0;
            virtual IProperty<bool, ISharedControl>& wrapText() = 0;
            virtual IProperty<HorizontalAlign, ISharedControl>& horizontalAlignment() = 0;
            virtual IProperty<std::string, ISharedControl>& text() = 0;

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
            IProperty<int, ISharedControl>& maxChars() override;
            IProperty<bool, ISharedControl>& password() override;
            IProperty<bool, ISharedControl>& readOnly() override;
            IProperty<bool, ISharedControl>& multiline() override;
            IProperty<bool, ISharedControl>& wrapText() override;
            IProperty<HorizontalAlign, ISharedControl>& horizontalAlignment() override;
            IProperty<std::string, ISharedControl>& text() override;

            IEvent<ISharedControl>& ChangedEvent() override;

            // compatibility setters
            void maxChars(int value);
            void password(bool value);
            void readOnly(bool value);
            void multiline(bool value);
            void wrapText(bool value);
            void horizontalAlignment(HorizontalAlign value);
            void text(const std::string& value);

        private:
            TextBox(std::shared_ptr<TextBoxImpl> impl, const Position& p);

            std::shared_ptr<TextBoxImpl> _impl;

            Property<int, ISharedControl> _maxChars;
            Property<bool, ISharedControl> _password;
            Property<bool, ISharedControl> _readOnly;
            Property<bool, ISharedControl> _multiline;
            Property<bool, ISharedControl> _wrapText;
            Property<HorizontalAlign, ISharedControl> _horizontalAlignment;
            Property<std::string, ISharedControl> _text;
            Event<ISharedControl> _changedEvent;
        };
    }
}
