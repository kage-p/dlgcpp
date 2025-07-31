#pragma once

#include "dlgcpp/controls/control.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class IListBox : public virtual IControl
        {
        public:
            virtual int currentIndex() const = 0;
            virtual void currentIndex(int value) = 0;
            virtual const std::vector<int>& currentIndexes() const = 0;
            virtual void currentIndexes(const std::vector<int>& indexes) = 0;
            virtual bool highlight() const = 0;
            virtual void highlight(bool value) = 0;
            virtual bool multiselect() const = 0;
            virtual void multiselect(bool value) = 0;
            virtual bool sorted() const = 0;
            virtual void sorted(bool value) = 0;
            virtual const std::vector<std::string>& items() const = 0;
            virtual void items(const std::vector<std::string>& items) = 0;

            virtual IEvent<ISharedControl>& SelChangedEvent() = 0;
            virtual IEvent<ISharedControl>& SelCancelEvent() = 0;
        };

        typedef std::shared_ptr<IListBox> ISharedListBox;

        class ListBoxImpl;

        class ListBox :
            public Control,
            public IListBox
        {
        public:
            explicit ListBox(const Position& p = Position());
            ~ListBox() override;

            // IListBox impl.
            int currentIndex() const override;
            void currentIndex(int value) override;
            const std::vector<int>& currentIndexes() const override;
            void currentIndexes(const std::vector<int>& indexes) override;
            bool highlight() const override;
            void highlight(bool value) override;
            bool multiselect() const override;
            void multiselect(bool value) override;
            bool sorted() const override;
            void sorted(bool value) override;
            const std::vector<std::string>& items() const override;
            void items(const std::vector<std::string>& items) override;

            IEvent<ISharedControl>& SelChangedEvent() override;
            IEvent<ISharedControl>& SelCancelEvent() override;

        private:
            ListBox(std::shared_ptr<ListBoxImpl> impl);

            std::shared_ptr<ListBoxImpl> _impl;
        };
    }
}
