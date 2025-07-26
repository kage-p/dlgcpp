#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class ImageImpl;

        class Image : public Control
        {
        public:
            explicit Image(const Position& p = Position());
            ~Image() override;

            bool autoSize() const;
            void autoSize(bool value);
            bool centered() const;
            void centered(bool value);
            const ImageSource& image() const;
            void image(const ImageSource& image);

        private:
            Image(std::shared_ptr<ImageImpl> impl);

            std::shared_ptr<ImageImpl> _impl;
        };
    }
}
