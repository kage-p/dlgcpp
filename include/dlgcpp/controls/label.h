#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class LabelImpl;

        class Label : public Control
        {
        public:
            explicit Label(const std::string& text = std::string(), const Position& p = Position());
            ~Label() override;

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
            Label(std::shared_ptr<LabelImpl> impl);

            std::shared_ptr<LabelImpl> _impl;
        };
    }
}
