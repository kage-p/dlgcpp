#pragma once
#include "../control.h"

namespace dlgcpp
{
    namespace controls
    {
        class TextBox : public dlgcpp::Control
        {
        public:
            explicit TextBox(std::shared_ptr<IDialog> parent, const std::string& text = std::string(), const Position& p = Position());
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

            IEvent<>& ChangedEvent();

        private:
            struct textbox_props* _props;

            void rebuild() override;
            std::string className() const override;
            unsigned int styles() const override;
            void notify(struct dlg_message&) override;

            void readInput();
        };
    }
}
