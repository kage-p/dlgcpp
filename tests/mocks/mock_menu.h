#pragma once

#include "dlgcpp/menus/menu.h"
#include "support/unit_test.h"

namespace dlgcpp
{
    namespace tests
    {
        class MockMenu : public dlgcpp::menus::IMenu
        {
        public:
            MOCK_METHOD(void, add, (ISharedMenuItem), (override));
            MOCK_METHOD(void, remove, (ISharedMenuItem), (override));
            MOCK_METHOD(void, clear, (), (override));
            MOCK_METHOD(const std::vector<ISharedMenuItem>&, items, (), (const, override));

            MOCK_METHOD(void, popup, (ISharedControl, const Point&), (override));
            MOCK_METHOD(void, popup, (ISharedDialog, const Point&), (override));
        };
    }
}
