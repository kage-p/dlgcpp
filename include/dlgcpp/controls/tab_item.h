#pragma once

#include "../defs.h"
#include "../event.h"
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
            virtual bool highlight() const = 0;
            virtual void highlight(bool value) = 0;

            virtual const std::string& text() const  = 0;
            virtual void text(const std::string& value) = 0;

            virtual const std::string& toolTipText() const  = 0;
            virtual void toolTipText(const std::string& value) = 0;

            virtual const ImageSource& image() const  = 0;
            virtual void image(ImageSource& value)  = 0;

            virtual IEvent<ISharedTabItem>& ClickEvent()  = 0;
        };

        class TabItem : public ITabItem
        {
        public:
            TabItem(const std::string& text, const ImageSource& image = ImageSource(), const std::string& toolTipText = std::string());
            virtual ~TabItem();

            bool highlight() const override;
            void highlight(bool value) override;

            const std::string& text() const override;
            void text(const std::string& value) override;

            virtual const std::string& toolTipText() const override;
            virtual void toolTipText(const std::string& value) override;

            const ImageSource& image() const override;
            void image(ImageSource& value) override;

            IEvent<ISharedTabItem>& ClickEvent() override;

        private:
            struct tab_item_priv* _priv;
        };
    }
}
