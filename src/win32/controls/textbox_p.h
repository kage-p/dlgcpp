#pragma once

#include "control_p.h"
#include "dlgcpp/controls/textbox.h"
#include "utility/event.h"

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
            explicit TextBoxImpl(
                const std::string& text = std::string(),
                const Position& p = Position());

            ~TextBoxImpl() override;

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
            int _maxChars = 0;
            bool _password = false;
            bool _readOnly = false;
            bool _multiline = false;
            bool _wrapText = false;
            HorizontalAlign _horzAlign = HorizontalAlign::Left;

            Event<ISharedControl> _changedEvent;

            void rebuild() override;
            std::string className() const override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;

            void readInput();
        };
    }
}
