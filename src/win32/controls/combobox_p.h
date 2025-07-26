#pragma once

#include "control_p.h"
#include "dlgcpp/controls/combobox.h"
#include "utility/event.h"
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
            explicit ComboBoxImpl(
                ComboBox& comboBox,
                const Position& p = Position());

            ~ComboBoxImpl() override;

            int currentIndex() const;
            void currentIndex(int value);
            bool dropDown() const;
            void dropDown(bool value);
            bool editable() const;
            void editable(bool value);
            bool sorted() const;
            void sorted(bool value);
            const std::vector<std::string>& items() const;
            void items(const std::vector<std::string>& items);

            IEvent<ISharedControl>& SelChangedEvent();
            IEvent<ISharedControl>& SelCancelEvent();
            IEvent<ISharedControl>& TextChangedEvent();
            IEvent<ISharedControl>& ListOpenEvent();
            IEvent<ISharedControl>& ListCloseEvent();

        private:
            ComboBox& _comboBox;
            bool _editable = true;
            bool _dropDown = false;
            bool _sorted = true;
            std::vector<std::string> _items;
            int _currentIndex = -1;
            Event<ISharedControl> _selChangedEvent;
            Event<ISharedControl> _selCancelEvent;
            Event<ISharedControl> _listCloseEvent;
            Event<ISharedControl> _listOpenEvent;
            Event<ISharedControl> _textChangedEvent;

            void rebuild() override;
            std::string className() const override;
            void notify(DialogMessage&) override;
            unsigned int styles() const override;
            bool isHandleEqual(void* otherHandle) const override;

            void readInput();
            void readSelection();
            void updateSelection();
            void updateItems();
        };
    }
}
