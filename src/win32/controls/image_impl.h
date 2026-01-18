#pragma once

#include "control_impl.h"
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
            explicit ImageImpl();
            ~ImageImpl() override;
            void owner(IImage* image);

        private:
            IImage* _image = nullptr;
            HGDIOBJ _hImage = NULL;

            void build() override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;

            void updateImage();
        };
    }
}
