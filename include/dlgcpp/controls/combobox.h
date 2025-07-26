#pragma once
#include "dlgcpp/controls/control.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class ComboBoxImpl;

        class ComboBox : public Control
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
            ComboBox(std::shared_ptr<ComboBoxImpl> impl);

            std::shared_ptr<ComboBoxImpl> _impl;
        };
    }
}
