#pragma once  

#include "dlgcpp/event.h"  
#include "unit_test.h"

namespace dlgcpp
{
    template <class... Args>
    class MockEvent : public IEvent<Args...>
    {
    public:
        MOCK_METHOD(EventBinding, bind, (std::function<void(Args... args)> fn), (override));
        MOCK_METHOD(size_t, count, (), (override));
        MOCK_METHOD(void, clear, (), (override));
        MOCK_METHOD(void, invoke, (Args... args), (override));
        MOCK_METHOD(void, invoke, (), (override));
        MOCK_METHOD(EventBinding, operator+=, (std::function<void(Args... args)> fn), (override));
        MOCK_METHOD(void, operator-=, (EventBinding ref), (override));
    };
}
