#pragma once

#include "dlgcpp/property.h"
#include "utfw/unit_test.h"

namespace dlgcpp
{
    template <class TValue, class TOwner>
    class MockProperty : public IProperty<TValue, TOwner>
    {
    public:
        MOCK_METHOD(bool, operator==, (const TValue& other), (const, override));
        MOCK_METHOD(bool, operator!=, (const TValue& other), (const, override));
        MOCK_METHOD(IProperty<TValue, TOwner>&, operator=, (const IProperty<TValue, TOwner>& other), (override));
        MOCK_METHOD(IProperty<TValue, TOwner>&, operator=, (const TValue& newValue), (override));
        MOCK_METHOD(const TValue*, operator->, (), (const, override));
        MOCK_METHOD(const TValue&, operator*, (), (const, override));
        MOCK_METHOD(const TValue&, value, (), (const, override));
        MOCK_METHOD(const std::string&, name, (), (const, override));
        MOCK_METHOD(bool, readOnly, (), (const, override));
        MOCK_METHOD(void, clear, (), (override));
        MOCK_METHOD(IEvent<TOwner>&, event, (), (override));
        MOCK_METHOD(operator TValue, (), (const, override));
    };
}