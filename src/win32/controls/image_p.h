#pragma once

#include "control_p.h"
#include "dlgcpp/controls/image.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        class ImageImpl : public ControlImpl
        {
        public:
            explicit ImageImpl(
                Image& image,
                const Position& p = Position());

            ~ImageImpl() override;

            bool autoSize() const;
            void autoSize(bool value);
            bool centered() const;
            void centered(bool value);
            const ImageSource& image() const;
            void image(const ImageSource& image);

        private:
            Image& _image;
            ImageSource _imageSource;
            bool _autoSize = false;
            bool _centered = false;

            // state
            HGDIOBJ _hImage = NULL;

            void rebuild() override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;

            void updateImage();
        };
    }
}
