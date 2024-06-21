#include "size_tests.h"
#include "dlgcpp/dlgcpp.h"

using namespace dlgcpp;
using namespace dlgcpp::tests;

TEST(SizeTests, test_constructor_default)
{
    Size target;

    EXPECT_EQ(target.width(), 0);
    EXPECT_EQ(target.height(), 0);
}

TEST(SizeTests, test_constructor_with_params)
{
    Size target(100,200);

    EXPECT_EQ(target.width(), 100);
    EXPECT_EQ(target.height(), 200);
}

TEST(SizeTests, test_constructor_with_copy)
{
    Size source(100,200);
    Size target(source);

    EXPECT_EQ(target.width(), 100);
    EXPECT_EQ(target.height(), 200);
}

TEST(SizeTests, test_assignment_op)
{
    Size source(100,200);
    Size target;

    target = source;
    EXPECT_EQ(target.width(), 100);
    EXPECT_EQ(target.height(), 200);
}

TEST(SizeTests, test_equal_op)
{
    Size source(100,200);
    Size target(100,200);

    EXPECT_EQ(target, source);
}

TEST(SizeTests, test_nequal_op)
{
    Size source(100,200);
    Size target(10,10);

    EXPECT_NE(target, source);
}

TEST(SizeTests, test_width)
{
    Size target;

    EXPECT_EQ(target.width(), 0);

    target.width(-100);
    EXPECT_EQ(target.width(), -100);

    target.width(100);
    EXPECT_EQ(target.width(), 100);
}

TEST(SizeTests, test_height)
{
    Size target;

    EXPECT_EQ(target.height(), 0);

    target.height(-100);
    EXPECT_EQ(target.height(), -100);

    target.height(100);
    EXPECT_EQ(target.height(), 100);
}

TEST(SizeTests, test_empty)
{
    Size target;

    EXPECT_EQ(target.empty(), true);

    target.width(100);
    EXPECT_EQ(target.empty(), true);

    target.height(100);
    EXPECT_EQ(target.empty(), false);
}

