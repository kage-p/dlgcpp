#pragma once

#include "control_impl.h"
#include "dlgcpp/controls/textbox.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        class TextBoxImpl : public ControlImpl
        {
        public:
            explicit TextBoxImpl();
            ~TextBoxImpl() override;
            void owner(ITextBox* textBox);

        private:
            ITextBox* _textBox = nullptr;

            void build() override;
            std::string className() const override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;

            void readInput();
            void onMaxCharsChanged();
            void onReadOnlyChanged();
        };
    }
}
