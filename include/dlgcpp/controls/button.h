#pragma once
#include "../control.h"

namespace dlgcpp
{
    namespace controls
    {
        class IButton
        {
        public:
            virtual HorizontalAlign horizontalAlignment() const = 0;
            virtual void horizontalAlignment(HorizontalAlign value) = 0;
            virtual VerticalAlign verticalAlignment() const = 0;
            virtual void verticalAlignment(VerticalAlign value) = 0;
        };

        typedef std::shared_ptr<IButton> ISharedButton;

        class Button : public dlgcpp::Control,
                       public IButton
        {
        public:
            explicit Button(const std::string& text = std::string(), const Position& p = Position());
            ~Button() override;

            // IButton impl.
            HorizontalAlign horizontalAlignment() const override;
            void horizontalAlignment(HorizontalAlign value) override;
            VerticalAlign verticalAlignment() const override;
            void verticalAlignment(VerticalAlign value) override;

        private:
            struct button_props* _props;

            void rebuild() override;
            std::string className() const override;
            unsigned int styles() const override;
            void notify(dlg_message&) override;
        };
    }
}
