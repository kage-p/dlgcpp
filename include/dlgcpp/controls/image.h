#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class IImage : public virtual IControl
        {
        public:
            virtual bool autoSize() const = 0;
            virtual void autoSize(bool value) = 0;
            virtual bool centered() const = 0;
            virtual void centered(bool value) = 0;
            virtual const ImageSource& image() const = 0;
            virtual void image(const ImageSource& image) = 0;
        };

        typedef std::shared_ptr<IImage> ISharedImage;

        class ImageImpl;

        class Image :
            public Control,
            public IImage
        {
        public:
            explicit Image(const Position& p = Position());
            ~Image() override;

            // IImage impl.
            bool autoSize() const override;
            void autoSize(bool value) override;
            bool centered() const override;
            void centered(bool value) override;
            const ImageSource& image() const override;
            void image(const ImageSource& image) override;

        private:
            Image(std::shared_ptr<ImageImpl> impl);

            std::shared_ptr<ImageImpl> _impl;
        };
    }
}
