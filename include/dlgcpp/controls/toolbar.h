#pragma once

#include "dlgcpp/controls/control.h"
#include "dlgcpp/controls/toolbar_item.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class IToolBar
        {
        public:
            virtual const Size& buttonSize() const = 0;
            virtual void buttonSize(const Size& value) = 0;
            virtual const Size& imageSize() const = 0;
            virtual void imageSize(const Size& value) = 0;
            virtual const std::vector<ISharedToolBarItem>& items() const = 0;
            virtual void items(const std::vector<ISharedToolBarItem>& items) = 0;
        };

        class ToolBarImpl;

        class ToolBar :
            public Control,
            public IToolBar
        {
        public:
            explicit ToolBar(const Position& p = Position());
            ~ToolBar() override;

            const Size& buttonSize() const override;
            void buttonSize(const Size& value) override;
            const Size& imageSize() const override;
            void imageSize(const Size& value) override;
            const std::vector<ISharedToolBarItem>& items() const override;
            void items(const std::vector<ISharedToolBarItem>& items) override;

        private:
            ToolBar(std::shared_ptr<ToolBarImpl> impl);

            std::shared_ptr<ToolBarImpl> _impl;
        };
    }
}
