#pragma once

#include "dlgcpp/controls/control.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class ListBoxImpl;

        class ListBox : public Control
        {
        public:
            explicit ListBox(const Position& p = Position());
            ~ListBox() override;

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
            ListBox(std::shared_ptr<ListBoxImpl> impl);

            std::shared_ptr<ListBoxImpl> _impl;
        };
    }
}
