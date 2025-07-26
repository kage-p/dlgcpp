#pragma once

#include "control_p.h"
#include "dlgcpp/controls/listbox.h"
#include "utility/event.h"

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
            explicit ListBoxImpl(
                ListBox& listBox,
                const Position& p = Position());

            ~ListBoxImpl() override;

            int currentIndex() const;
            void currentIndex(int value);
            const std::vector<int>& currentIndexes() const;
            void currentIndexes(const std::vector<int>& indexes);
            bool highlight() const;
            void highlight(bool value);
            bool multiselect() const;
            void multiselect(bool value);
            bool sorted() const;
            void sorted(bool value);
            const std::vector<std::string>& items() const;
            void items(const std::vector<std::string>& items);

            IEvent<ISharedControl>& SelChangedEvent();
            IEvent<ISharedControl>& SelCancelEvent();

        private:
            ListBox& _listBox;
            bool _highlight = true;
            bool _multiselect = false;
            bool _sorted = true;
            std::vector<std::string> _items;
            int _currentIndex = -1;
            std::vector<int> _currentIndexes;
            Event<ISharedControl> _selChangedEvent;
            Event<ISharedControl> _selCancelEvent;

            void rebuild() override;
            std::string className() const override;
            void notify(DialogMessage&) override;
            unsigned int styles() const override;

            void readSelection();
            void updateSelection();
            void updateItems();
        };
    }
}
