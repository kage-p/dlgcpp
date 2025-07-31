#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class ILabel : public virtual IControl
        {
        public:
            virtual bool autoSize() const = 0;
            virtual void autoSize(bool value) = 0;
            virtual bool clickable() const = 0;
            virtual void clickable(bool value) = 0;
            virtual bool elipsis() const = 0;
            virtual void elipsis(bool value) = 0;
            virtual HorizontalAlign horizontalAlignment() const = 0;
            virtual void horizontalAlignment(HorizontalAlign value) = 0;
            virtual VerticalAlign verticalAlignment() const = 0;
            virtual void verticalAlignment(VerticalAlign value) = 0;
        };

        typedef std::shared_ptr<ILabel> ISharedLabel;

        class LabelImpl;

        class Label :
            public Control,
            public ILabel
        {
        public:
            explicit Label(const std::string& text = std::string(), const Position& p = Position());
            ~Label() override;

            // ILabel impl.
            bool autoSize() const override;
            void autoSize(bool value) override;
            bool clickable() const override;
            void clickable(bool value) override;
            bool elipsis() const override;
            void elipsis(bool value) override;
            HorizontalAlign horizontalAlignment() const override;
            void horizontalAlignment(HorizontalAlign value) override;
            VerticalAlign verticalAlignment() const override;
            void verticalAlignment(VerticalAlign value) override;

        private:
            Label(std::shared_ptr<LabelImpl> impl);

            std::shared_ptr<LabelImpl> _impl;
        };
    }
}
