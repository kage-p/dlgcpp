#pragma once
#include "../control.h"

namespace dlgcpp
{
    namespace controls
    {
        class Image : public dlgcpp::Control
        {
        public:
            explicit Image(std::shared_ptr<IDialog> parent, const Position& p = Position());
            ~Image() override;

            bool autoSize() const;
            void autoSize(bool value);
            const ImageSource& image() const;
            void image(const ImageSource& image);

        private:
            struct img_props* _props;
            struct img_state* _state;

            void rebuild() override;
            unsigned int styles() const override;
            unsigned int exStyles() const override;
            void notify(struct dlg_message&) override;

            void updateImage();
        };
    }
}
