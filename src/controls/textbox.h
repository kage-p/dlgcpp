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

            bool readOnly() const;
            void readOnly(bool value);
            bool multiline() const;
            void multiline(bool value);
            bool wrapText() const;
            void wrapText(bool value);

        private:
            struct tb_props* _props;

            std::string className() const override;
            unsigned int styles() const override;
            unsigned int exStyles() const override;
        };
    }
}
