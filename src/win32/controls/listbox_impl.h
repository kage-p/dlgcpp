#pragma once

#include "control_impl.h"
#include "dlgcpp/controls/listbox.h"

#include <vector>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        class ListBoxImpl : public ControlImpl
        {
        public:
            explicit ListBoxImpl();
            ~ListBoxImpl() override;
            void owner(IListBox* listBox);

        private:
            IListBox* _listBox = nullptr;

            void build() override;
            std::string className() const override;
            void notify(DialogMessage&) override;
            unsigned int styles() const override;

            void readSelection();
            void updateSelection();
            void updateItems();

            void onItemsChanged();
        };
    }
}
