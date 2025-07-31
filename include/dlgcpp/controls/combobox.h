#pragma once
#include "dlgcpp/controls/control.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class IComboBox : public virtual IControl
        {
        public:
            virtual int currentIndex() const = 0;
            virtual void currentIndex(int value) = 0;
            virtual bool dropDown() const = 0;
            virtual void dropDown(bool value) = 0;
            virtual bool editable() const = 0;
            virtual void editable(bool value) = 0;
            virtual bool sorted() const = 0;
            virtual void sorted(bool value) = 0;
            virtual const std::vector<std::string>& items() const = 0;
            virtual void items(const std::vector<std::string>& items) = 0;

            virtual IEvent<ISharedControl>& SelChangedEvent() = 0;
            virtual IEvent<ISharedControl>& SelCancelEvent() = 0;
            virtual IEvent<ISharedControl>& TextChangedEvent() = 0;
            virtual IEvent<ISharedControl>& ListOpenEvent() = 0;
            virtual IEvent<ISharedControl>& ListCloseEvent() = 0;
        };

        typedef std::shared_ptr<IComboBox> ISharedComboBox;

        class ComboBoxImpl;

        class ComboBox :
            public Control,
            public IComboBox
        {
        public:
            explicit ComboBox(const Position& p = Position());
            ~ComboBox() override;

            // IComboBox impl;
            int currentIndex() const override;
            void currentIndex(int value) override;
            bool dropDown() const override;
            void dropDown(bool value) override;
            bool editable() const override;
            void editable(bool value) override;
            bool sorted() const override;
            void sorted(bool value) override;
            const std::vector<std::string>& items() const override;
            void items(const std::vector<std::string>& items) override;

            IEvent<ISharedControl>& SelChangedEvent() override;
            IEvent<ISharedControl>& SelCancelEvent() override;
            IEvent<ISharedControl>& TextChangedEvent() override;
            IEvent<ISharedControl>& ListOpenEvent() override;
            IEvent<ISharedControl>& ListCloseEvent() override;

        private:
            ComboBox(std::shared_ptr<ComboBoxImpl> impl);

            std::shared_ptr<ComboBoxImpl> _impl;
        };
    }
}
