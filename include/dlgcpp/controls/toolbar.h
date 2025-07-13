#pragma once
#include "../control.h"
#include "toolbar_item.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class IToolBar
        {
        public:
            virtual const Size& buttonSize() const = 0;
            virtual void buttonSize(const Size& value) = 0;
            virtual const Size& imageSize() const = 0;
            virtual void imageSize(const Size& value) = 0;
            virtual const std::vector<ISharedToolBarItem>& items() const = 0;
            virtual void items(const std::vector<ISharedToolBarItem>& items) = 0;
        };

        typedef std::shared_ptr<IToolBar> ISharedToolBar;

        class ToolBar :
            public dlgcpp::Control,
            public IToolBar
        {
        public:
            explicit ToolBar(const Position& p = Position());
            ~ToolBar() override;

            const Size& buttonSize() const override;
            void buttonSize(const Size& value) override;
            const Size& imageSize() const override;
            void imageSize(const Size& value) override;
            const std::vector<ISharedToolBarItem>& items() const override;
            void items(const std::vector<ISharedToolBarItem>& items) override;

        private:
            struct toolbar_props* _props;
            void rebuild() override;
            std::string className() const override;
            int idRange() const override;
            void notify(dlg_message&) override;
            unsigned int styles() const override;
            bool isHandleEqual(void* otherHandle) const override;

            ISharedToolBarItem findItemById(int ctlId) const;
            void updateItems();

            static constexpr int ToolbarIdRange = 100;
        };
    }
}
