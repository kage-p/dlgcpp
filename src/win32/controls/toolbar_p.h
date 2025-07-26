#pragma once

#include "control_p.h"
#include "dlgcpp/controls/toolbar.h"
#include "utility/event.h"
#include <vector>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <commctrl.h>

namespace dlgcpp
{
    namespace controls
    {
        class ToolBarImpl : public ControlImpl
        {
        public:
            explicit ToolBarImpl(
                ToolBar& toolBar,
                const Position& p = Position());

            ~ToolBarImpl() override;

            const Size& buttonSize() const;
            void buttonSize(const Size& value);
            const Size& imageSize() const;
            void imageSize(const Size& value);
            const std::vector<ISharedToolBarItem>& items() const;
            void items(const std::vector<ISharedToolBarItem>& items);

        private:
            ToolBar& _toolBar;
            Size _buttonSize = Size(24, 24);
            Size _imageSize = Size(20, 20);
            std::vector<ISharedToolBarItem> _items;
            std::wstring _tooltipBuf;

            void rebuild() override;
            std::string className() const override;
            int idRange() const override;
            void notify(DialogMessage&) override;
            unsigned int styles() const override;
            bool isHandleEqual(void* otherHandle) const override;

            ISharedToolBarItem findItemById(int ctlId) const;
            void updateItems();

            static constexpr int ToolbarIdRange = 100;
        };
    }
}
