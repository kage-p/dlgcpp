#pragma once

#include "control_impl.h"
#include "dlgcpp/controls/toolbar.h"
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
                const Position& p = Position());
            ~ToolBarImpl() override;
            void owner(IToolBar* toolBar);

        private:
            IToolBar* _toolBar = nullptr;

            // internals
            std::wstring _tooltipBuf;

            void build() override;
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
