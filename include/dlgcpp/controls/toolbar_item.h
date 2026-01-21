#pragma once

#include "dlgcpp/defs.h"
#include "dlgcpp/event.h"
#include "dlgcpp/property.h"
#include <memory>

namespace dlgcpp
{
    namespace controls
    {
        class IToolBarItem;
        typedef std::shared_ptr<IToolBarItem> ISharedToolBarItem;

        class IToolBarItem
        {
        public:
            virtual IProperty<bool, ISharedToolBarItem>& enabled() = 0;
            virtual IProperty<bool, ISharedToolBarItem>& checked() = 0;
            virtual IProperty<bool, ISharedToolBarItem>& separator() = 0;
            virtual IProperty<std::string, ISharedToolBarItem>& text() = 0;
            virtual IProperty<std::string, ISharedToolBarItem>& toolTipText() = 0;
            virtual IProperty<ImageSource, ISharedToolBarItem>& image() = 0;

            virtual IEvent<ISharedToolBarItem>& ClickEvent() = 0;
        };

        class ToolBarItemImpl;

        class ToolBarItem :
            public IToolBarItem,
            public std::enable_shared_from_this<ToolBarItem>
        {
        public:
            ToolBarItem(
                const ImageSource& image = ImageSource(),
                const std::string& text = {},
                const std::string& toolTipText = {});
            virtual ~ToolBarItem() = default;

            ToolBarItem& operator=(const ToolBarItem& other);
            bool operator==(const ToolBarItem& other) const;
            bool operator!=(const ToolBarItem& other) const;

            // IToolBarItem impl.
            IProperty<bool, ISharedToolBarItem>& enabled() override;
            IProperty<bool, ISharedToolBarItem>& checked() override;
            IProperty<bool, ISharedToolBarItem>& separator() override;
            IProperty<std::string, ISharedToolBarItem>& text() override;
            IProperty<std::string, ISharedToolBarItem>& toolTipText() override;
            IProperty<ImageSource, ISharedToolBarItem>& image() override;

            IEvent<ISharedToolBarItem>& ClickEvent() override;

        private:
            Property<bool, ISharedToolBarItem> _enabled;
            Property<bool, ISharedToolBarItem> _checked;
            Property<bool, ISharedToolBarItem> _separator;
            Property<std::string, ISharedToolBarItem> _text;
            Property<std::string, ISharedToolBarItem> _toolTipText;
            Property<ImageSource, ISharedToolBarItem> _image;
            Event<ISharedToolBarItem> _clickEvent;
        };
    }
}
