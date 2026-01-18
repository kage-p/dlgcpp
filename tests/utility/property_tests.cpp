#include "dlgcpp/property.h"
#include "property_tests.h"
#include <memory>

using namespace dlgcpp;
using namespace dlgcpp::tests;

class PropertyOwnerClass
{
public:
    PropertyOwnerClass() : _text("", nullptr, [this]() {return this; }, "text")
    {
    }

    IProperty<std::string, PropertyOwnerClass*>& text()
    {
        return _text;
    }

private:
    Property<std::string, PropertyOwnerClass*> _text;

};

TEST_F(PropertyTests, constructor_default)
{
    Property<std::string, int> target;

    EXPECT_EQ(target.value(), std::string());
}

TEST_F(PropertyTests, constructor_with_params)
{
    Property<std::string, int> target(
        "10",
        [](auto) { return true; },
        [] {return 99; },
        "name");

    EXPECT_EQ(target.value(), std::string("10"));
    EXPECT_EQ(target.name(), std::string("name"));
}

TEST_F(PropertyTests, constructor_with_copy)
{
    Property<std::string, int> source(
        "10",
        [](auto) { return true; },
        [] {return 99; },
        "name");

    Property<std::string, int> target(source);

    EXPECT_EQ(target.value(), std::string("10"));
}

TEST_F(PropertyTests, value_with_int_type)
{
    Property<int, int> target(42);

    EXPECT_EQ(target.value(), 42);

    target = 100;
    EXPECT_EQ(target.value(), 100);
}

TEST_F(PropertyTests, value_with_double_type)
{
    Property<double, int> target(3.14);

    EXPECT_DOUBLE_EQ(target.value(), 3.14);

    target = 2.71;
    EXPECT_DOUBLE_EQ(target.value(), 2.71);
}

TEST_F(PropertyTests, value_with_string_type)
{
    Property<std::pair<std::string, std::string>, int> target(std::make_pair("a", "b"));

    EXPECT_EQ(target.value(), std::make_pair("a", "b"));

    target = std::make_pair("x", "y");
    EXPECT_EQ(target.value(), std::make_pair("x", "y"));
}

TEST_F(PropertyTests, value_with_shared_ptr_type)
{
    auto var = std::make_shared<std::string>("123");
    Property<std::shared_ptr<std::string>, int> target(var);

    EXPECT_EQ(target.value(), var);

    var->assign("abc");

    EXPECT_EQ(target.value()->c_str(), std::string("abc"));

    EXPECT_EQ(target.value()->substr(0, 2), "ab");
}

TEST_F(PropertyTests, clear_with_int_type)
{
    Property<int, int> target(42);

    target.clear();
    EXPECT_EQ(target.value(), 0);
}

TEST_F(PropertyTests, clear_with_string_type)
{
    Property<std::pair<std::string, std::string>, int> target(std::make_pair("a", "b"));

    target.clear();
    EXPECT_EQ(target.value(), std::make_pair("", ""));
}

TEST_F(PropertyTests, op_conversion)
{
    Property<int, int> target(42);
    int val = target; // should invoke operator TValue()
    EXPECT_EQ(val, 42);
}

TEST_F(PropertyTests, op_assignment)
{
    Property<std::string, int> source("10");

    Property<std::string, int> target;

    target = source;

    EXPECT_EQ(target.value(), std::string("10"));
}

TEST_F(PropertyTests, op_assignment_validate_false)
{
    Property<std::string, int> source("10");

    Property<std::string, int> target(
        "10",
        [](auto) { return false; },
        [] {return 99; },
        "name");

    target = source;

    EXPECT_EQ(target.value(), std::string("10"));
}

TEST_F(PropertyTests, op_assignment_by_value)
{
    Property<std::string, int> source(
        "10",
        [](auto) { return true; },
        [] {return 99; },
        "name");

    Property<std::string, int> target;

    target = source.value();

    EXPECT_EQ(target.value(), std::string("10"));
}

TEST_F(PropertyTests, op_assignment_by_value_validate_false)
{
    Property<std::string, int> target(
        "10",
        [](auto) { return false; },
        [] {return 99; },
        "name");

    target = "20";

    EXPECT_EQ(target.value(), std::string("10"));
}

TEST_F(PropertyTests, op_equal)
{
    Property<std::string, int> target("10");
    Property<std::string, int> compare("10");

    EXPECT_EQ(target, compare);
}

TEST_F(PropertyTests, op_nequal)
{
    Property<std::string, int> target("10");
    Property<std::string, int> compare("20");

    EXPECT_NE(target, compare);
}

TEST_F(PropertyTests, event_with_int_owner)
{
    Property<std::string, int> target(
        "10",
        nullptr,
        [] {return 99; },
        "name");

    auto& event = target.event();

    int eventCalled = 0;
    event += [&eventCalled](int owner)
        {
            if (owner == 99)
                eventCalled++;
        };

    // set some values
    target = "1";
    target = "2";
    target = "3";
    target = "3"; // same; no event

    EXPECT_EQ(eventCalled, 3);
}

TEST_F(PropertyTests, event_with_raw_pointer_owner)
{
    int ownerValue = 42;
    Property<std::string, int*> target(
        "initial",
        nullptr,
        [&] {return &ownerValue; },
        "name");

    int eventCalled = 0;
    target.event() += [&](int* owner) {
        if (owner == &ownerValue)
            eventCalled++;
        };

    target = "changed";
    EXPECT_EQ(eventCalled, 1);

    // Assign same value, event should NOT fire
    target = "changed";
    EXPECT_EQ(eventCalled, 1);
}

TEST_F(PropertyTests, event_with_shared_ptr_owner)
{
    auto ownerPtr = std::make_shared<int>(99);
    Property<std::string, std::shared_ptr<int>> target(
        "initial",
        nullptr,
        [&] {return ownerPtr; },
        "name");

    int eventCalled = 0;
    target.event() += [&](const std::shared_ptr<int>& owner) {
        if (owner == ownerPtr)
            eventCalled++;
        };

    target = "changed";
    EXPECT_EQ(eventCalled, 1);

    // Assign same value, event should NOT fire
    target = "changed";
    EXPECT_EQ(eventCalled, 1);
}

TEST_F(PropertyTests, event_with_null_raw_pointer_owner)
{
    int id = 1;
    int* ptr_owner = &id;

    Property<std::string, int*> target(
        "initial",
        nullptr,
        [&]() { return ptr_owner; },
        "name");

    int eventCalled = 0;
    target.event() += [&](int* owner) {
        if (owner == ptr_owner)
            eventCalled++;
        };

    target = "changed";
    EXPECT_EQ(eventCalled, 1);
}

TEST_F(PropertyTests, event_with_null_shared_ptr_owner)
{
    std::shared_ptr<int> nullOwner;
    Property<std::string, std::shared_ptr<int>> target(
        "initial",
        nullptr,
        [&] {return nullOwner; },
        "name");

    int eventCalled = 0;
    target.event() += [&](const std::shared_ptr<int>& owner) {
        if (!owner)
            eventCalled++;
        };

    target = "changed";
    EXPECT_EQ(eventCalled, 1);
}

TEST_F(PropertyTests, text_with_owner)
{
    PropertyOwnerClass owner;

    int eventCalled = 0;
    owner.text().event() += [&](PropertyOwnerClass* pOwner) {
        if (pOwner == &owner)
        {
            logText(owner.text());
            eventCalled++;
        }
        };

    owner.text() = "abc";
    owner.text() = "123";
    owner.text() = "end";

    EXPECT_EQ(owner.text(), std::string("end"));
    EXPECT_EQ(eventCalled, 3);
}
