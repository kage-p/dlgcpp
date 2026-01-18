#include "dlgcpp/size.h"
#include "size_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::tests;

TEST_F(SizeTests, constructor)
{
    Size target;

    EXPECT_EQ(target.width(), 0);
    EXPECT_EQ(target.height(), 0);
}

TEST_F(SizeTests, constructor_with_params)
{
    Size target(100, 200);

    EXPECT_EQ(target.width(), 100);
    EXPECT_EQ(target.height(), 200);
}

TEST_F(SizeTests, constructor_with_copy)
{
    Size source(100, 200);
    Size target(source);

    EXPECT_EQ(target.width(), 100);
    EXPECT_EQ(target.height(), 200);
}

TEST_F(SizeTests, op_assignment)
{
    Size source(100, 200);
    Size target;

    target = source;
    EXPECT_EQ(target.width(), 100);
    EXPECT_EQ(target.height(), 200);
}

TEST_F(SizeTests, op_equal)
{
    Size source(100, 200);
    Size target(100, 200);

    EXPECT_EQ(target, source);
}

TEST_F(SizeTests, op_nequal)
{
    Size source(100, 200);
    Size target(10, 10);

    EXPECT_NE(target, source);
}

TEST_F(SizeTests, width)
{
    Size target;

    EXPECT_EQ(target.width(), 0);

    target.width(-100);
    EXPECT_EQ(target.width(), -100);

    target.width(100);
    EXPECT_EQ(target.width(), 100);
}

TEST_F(SizeTests, height)
{
    Size target;

    EXPECT_EQ(target.height(), 0);

    target.height(-100);
    EXPECT_EQ(target.height(), -100);

    target.height(100);
    EXPECT_EQ(target.height(), 100);
}

TEST_F(SizeTests, empty)
{
    Size target;

    EXPECT_EQ(target.empty(), true);

    target.width(100);
    EXPECT_EQ(target.empty(), true);

    target.height(100);
    EXPECT_EQ(target.empty(), false);
}

