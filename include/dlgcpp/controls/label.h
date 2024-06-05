#pragma once
#include "../control.h"
#include "../dialog.h"

namespace dlgcpp
{
    namespace controls
    {
        class Label : public dlgcpp::Control
        {
        public:
            explicit Label(std::shared_ptr<IDialog> parent, const std::string& text = std::string(), const Position& p = Position());
            ~Label() override;

            void p(const Position& p) override;
            void text(const std::string& value) override;
            void font(const Font& value) override;
            bool autoSize() const;
            void autoSize(bool value);
            bool elipsis() const;
            void elipsis(bool value);
            HorizontalAlign horizontalAlignment() const;
            void horizontalAlignment(HorizontalAlign value);
            VerticalAlign verticalAlignment() const;
            void verticalAlignment(VerticalAlign value);

        private:
            struct lbl_props* _props;

            unsigned int styles() const override;
            void notify(struct dlg_message&) override;

            void updateAutoSize();
        };
    }
}
