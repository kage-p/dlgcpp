#pragma once
#include "../control.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class ListBox : public dlgcpp::Control
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
            struct listbox_props* _props;
            void rebuild() override;
            std::string className() const override;
            void notify(dlg_message&) override;
            unsigned int styles() const override;

            void readSelection();
            void updateSelection();
            void updateItems();
        };
    }
}
