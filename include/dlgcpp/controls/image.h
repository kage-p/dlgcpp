#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class IImage : public virtual IControl
        {
        public:
            // properties
            virtual IProperty<bool, ISharedControl>& autoSize() = 0;
            virtual IProperty<bool, ISharedControl>& centered() = 0;
            virtual IProperty<ImageSource, ISharedControl>& source() = 0;

            // events
            virtual IEvent<ISharedControl>& ClickEvent() = 0;
            virtual IEvent<ISharedControl>& DoubleClickEvent() = 0;
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
            IProperty<bool, ISharedControl>& autoSize() override;
            IProperty<bool, ISharedControl>& centered() override;
            IProperty<ImageSource, ISharedControl>& source() override;
            IEvent<ISharedControl>& ClickEvent() override;
            IEvent<ISharedControl>& DoubleClickEvent() override;

            // compatibility setters
            void autoSize(bool value);
            void centered(bool value);
            void source(const ImageSource& source);
            void image(const ImageSource& source);

        private:
            Image(std::shared_ptr<ImageImpl> impl, const Position& p);

            std::shared_ptr<ImageImpl> _impl;

            Property<bool, ISharedControl> _autoSize;
            Property<bool, ISharedControl> _centered;
            Property<ImageSource, ISharedControl> _source;
            Event<ISharedControl> _clickEvent;
            Event<ISharedControl> _dblClickEvent;
        };
    }
}
