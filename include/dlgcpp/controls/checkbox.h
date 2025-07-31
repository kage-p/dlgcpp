#pragma once
#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class ICheckBox : public virtual IControl
        {
        public:
            virtual bool checked() const = 0;
            virtual void checked(bool value) = 0;
            virtual bool autoCheck() const = 0;
            virtual void autoCheck(bool value) = 0;
            virtual HorizontalAlign horizontalAlignment() const = 0;
            virtual void horizontalAlignment(HorizontalAlign value) = 0;
            virtual VerticalAlign verticalAlignment() const = 0;
            virtual void verticalAlignment(VerticalAlign value) = 0;
        };

        typedef std::shared_ptr<ICheckBox> ISharedCheckBox;

        class CheckBoxImpl;

        class CheckBox :
            public Control,
            public ICheckBox
        {
        public:
            explicit CheckBox(
                const std::string& text = std::string(),
                const Position& p = Position());

            ~CheckBox() override;

            // ICheckBox impl.
            bool checked() const override;
            void checked(bool value) override;
            bool autoCheck() const override;
            void autoCheck(bool value) override;
            HorizontalAlign horizontalAlignment() const override;
            void horizontalAlignment(HorizontalAlign value) override;
            VerticalAlign verticalAlignment() const override;
            void verticalAlignment(VerticalAlign value) override;

        private:
            CheckBox(std::shared_ptr<CheckBoxImpl> impl);

            std::shared_ptr<CheckBoxImpl> _impl;
        };
    }
}
