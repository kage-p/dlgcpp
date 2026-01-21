#include "dlgcpp/event.h"
#include "event_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::tests;

TEST_F(EventTests, reset)
{
    Event<int> target;
    int counter = 0;

    target.reset([]() { return 99; }, "name");

    target +=
        [&](int owner)
        {
            if (owner == 99)
                counter++;
        };
    target.invoke();

    EXPECT_EQ(target.name(), std::string("name"));
    EXPECT_EQ(counter, 1);
}

TEST_F(EventTests, op_plus)
{
    Event<int> target;

    auto binding = target += [](auto) {};

    EXPECT_EQ(binding, 1);
}

TEST_F(EventTests, op_minus)
{
    Event<int> target;

    auto binding = target += [](auto) {};

    target -= binding;

    EXPECT_EQ(target.count(), 0);
}

TEST_F(EventTests, bind)
{
    Event<int> target;

    auto binding = target.bind([](auto) {});

    EXPECT_EQ(binding, 1);
}

TEST_F(EventTests, count)
{
    Event<int> target;

    EXPECT_EQ(target.count(), 0);

    auto binding = target += [](auto) {};
    EXPECT_EQ(target.count(), 1);
}

TEST_F(EventTests, clear)
{
    Event<int> target;

    target += [](auto) {};
    target += [](auto) {};

    target.clear();
    EXPECT_EQ(target.count(), 0);
}

TEST_F(EventTests, invoke_with_owner)
{
    Event<int, int> target;

    target.reset([]() { return 1; }, "name");

    int counter = 0;
    target += [&](int owner, int param) { counter++; };
    target += [&](int owner, int param) { counter++; };
    target += [&](int owner, int param) { counter++; };

    target.invoke(1);
    target.invoke(2);

    EXPECT_EQ(counter, 6);
}

TEST_F(EventTests, invoke_with_no_owner)
{
    Event<int, int> target;

    int counter = 0;
    target += [&](int owner, int param) { counter++; };
    target += [&](int owner, int param) { counter++; };
    target += [&](int owner, int param) { counter++; };

    target.invoke(1);
    target.invoke(2);

    EXPECT_EQ(counter, 0);
}
