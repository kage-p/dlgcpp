#pragma once

#include "control_impl.h"
#include "dlgcpp/controls/tabs.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <CommCtrl.h>

namespace dlgcpp
{
    namespace controls
    {
        class TabsImpl : public ControlImpl
        {
        public:
            explicit TabsImpl();
            ~TabsImpl() override;
            void owner(ITabs* tabs);

        private:
            ITabs* _tabs = nullptr;

            std::string className() const override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;
            void build() override;

            void readSelection();
            void updateSelection();
            void updateItems();
            void updateClientArea();

            void onItemsChanged();
        };
    }
}
