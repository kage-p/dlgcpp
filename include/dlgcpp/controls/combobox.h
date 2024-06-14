#pragma once
#include "../control.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class ComboBox : public dlgcpp::Control
        {
        public:
            explicit ComboBox(const Position& p = Position());
            ~ComboBox() override;

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
            struct combobox_props* _props;
            void rebuild() override;
            std::string className() const override;
            void notify(dlg_message&) override;
            unsigned int styles() const override;

            void readInput();
            void readSelection();
            void updateSelection();
            void updateItems();
        };
    }
}
