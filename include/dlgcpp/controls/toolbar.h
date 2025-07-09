#pragma once
#include "../control.h"
#include "toolbar_item.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class ToolBar : public dlgcpp::Control
        {
        public:
            explicit ToolBar(const Position& p = Position());
            ~ToolBar() override;

            const Size& buttonSize() const;
            void buttonSize(const Size& value);
            const Size& imageSize() const;
            void imageSize(const Size& value);
            const std::vector<ISharedToolBarItem>& items() const;
            void items(const std::vector<ISharedToolBarItem>& items);

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
