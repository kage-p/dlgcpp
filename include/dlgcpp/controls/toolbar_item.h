#pragma once

#include "../defs.h"
#include "../event.h"
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
            virtual bool enabled() const = 0;
            virtual void enabled(bool value) = 0;

            virtual bool checked() const = 0;
            virtual void checked(bool value) = 0;

            virtual bool separator() const = 0;
            virtual void separator(bool value) = 0;

            virtual const std::string& text() const  = 0;
            virtual void text(const std::string& value) = 0;

            virtual const std::string& toolTipText() const  = 0;
            virtual void toolTipText(const std::string& value) = 0;

            virtual const ImageSource& image() const  = 0;
            virtual void image(ImageSource& value)  = 0;

            virtual IEvent<ISharedToolBarItem>& ClickEvent()  = 0;
        };

        class ToolBarItem : public IToolBarItem
        {
        public:
            ToolBarItem(const ImageSource& image = ImageSource(), const std::string& toolTipText = std::string());
            virtual ~ToolBarItem();

            bool enabled() const override;
            void enabled(bool value) override;

            bool checked() const override;
            void checked(bool value) override;

            bool separator() const override;
            void separator(bool value) override;

            const std::string& text() const override;
            void text(const std::string& value) override;

            virtual const std::string& toolTipText() const override;
            virtual void toolTipText(const std::string& value) override;

            const ImageSource& image() const override;
            void image(ImageSource& value) override;

            IEvent<ISharedToolBarItem>& ClickEvent() override;

        private:
            struct toolbar_item_priv* _priv;
        };
    }
}
