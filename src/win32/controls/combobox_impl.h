#pragma once

#include "control_impl.h"
#include "dlgcpp/controls/combobox.h"
#include <vector>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        class ComboBoxImpl : public ControlImpl
        {
        public:
            explicit ComboBoxImpl();
            ~ComboBoxImpl() override;
            void owner(IComboBox* comboBox);

        private:
            IComboBox* _comboBox = nullptr;

            void build() override;
            std::string className() const override;
            void notify(DialogMessage&) override;
            unsigned int styles() const override;
            bool isHandleEqual(void* otherHandle) const override;

            void readInput();
            void readSelection();
            void updateSelection();
            void updateItems();

            void onEditableChanged();
            void onItemsChanged();
            void onTextChanged();
        };
    }
}
