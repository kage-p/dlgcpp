#pragma once

#include "dlgcpp/defs.h"
#include "dlgcpp/event.h"
#include "dlgcpp/property.h"
#include <memory>

namespace dlgcpp
{
    namespace controls
    {
        class ITabItem;
        typedef std::shared_ptr<ITabItem> ISharedTabItem;

        class ITabItem
        {
        public:
            virtual IProperty<bool, ISharedTabItem>& highlight() = 0;
            virtual IProperty<std::string, ISharedTabItem>& text() = 0;
            virtual IProperty<std::string, ISharedTabItem>& toolTipText() = 0;
            virtual IProperty<ImageSource, ISharedTabItem>& image() = 0;

            virtual IEvent<ISharedTabItem>& ClickEvent() = 0;
        };

        class TabItemImpl;

        class TabItem :
            public ITabItem,
            public std::enable_shared_from_this<TabItem>
        {
        public:
            TabItem(
                const std::string& text = std::string(),
                const ImageSource& image = ImageSource(),
                const std::string& toolTipText = std::string());
            virtual ~TabItem() = default;

            TabItem& operator=(const TabItem& other);
            bool operator==(const TabItem& other) const;
            bool operator!=(const TabItem& other) const;

            // ITabItem impl.
            IProperty<bool, ISharedTabItem>& highlight() override;
            IProperty<std::string, ISharedTabItem>& text() override;
            IProperty<std::string, ISharedTabItem>& toolTipText() override;
            IProperty<ImageSource, ISharedTabItem>& image() override;

            IEvent<ISharedTabItem>& ClickEvent() override;

        private:
            Property<bool, ISharedTabItem> _highlight;
            Property<std::string, ISharedTabItem> _text;
            Property<std::string, ISharedTabItem> _toolTipText;
            Property<ImageSource, ISharedTabItem> _image;
            Event<ISharedTabItem> _clickEvent;
        };
    }
}
