#pragma once

#include "control_p.h"
#include "dlgcpp/controls/label.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        class LabelImpl : public ControlImpl
        {
        public:
            explicit LabelImpl(
                Label& label,
                const std::string& text = std::string(),
                const Position& p = Position());

            ~LabelImpl() override;

            void p(const Position& p) override;
            void text(const std::string& value) override;
            void font(const Font& value) override;
            bool autoSize() const;
            void autoSize(bool value);
            bool clickable() const;
            void clickable(bool value);
            bool elipsis() const;
            void elipsis(bool value);
            HorizontalAlign horizontalAlignment() const;
            void horizontalAlignment(HorizontalAlign value);
            VerticalAlign verticalAlignment() const;
            void verticalAlignment(VerticalAlign value);

        private:
            Label& _label;
            bool _autoSize = false;
            bool _elipsis = false;
            bool _clickable = true;
            HorizontalAlign _horzAlign = HorizontalAlign::Left;
            VerticalAlign _vertAlign = VerticalAlign::Top;

            void rebuild() override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;

            void updateAutoSize();
        };
    }
}
