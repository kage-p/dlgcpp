#include "point_tests.h"
#include "dlgcpp/dlgcpp.h"

using namespace dlgcpp;
using namespace dlgcpp::tests;

TEST(PointTests, test_constructor_default)
{
    Point target;

    EXPECT_EQ(target.x(), 0);
    EXPECT_EQ(target.y(), 0);
}

TEST(PointTests, test_constructor_with_params)
{
    Point target(100,200);

    EXPECT_EQ(target.x(), 100);
    EXPECT_EQ(target.y(), 200);
}

TEST(PointTests, test_constructor_with_copy)
{
    Point source(100,200);
    Point target(source);

    EXPECT_EQ(target.x(), 100);
    EXPECT_EQ(target.y(), 200);
}

TEST(PointTests, test_assignment_op)
{
    Point source(100,200);
    Point target;

    target = source;
    EXPECT_EQ(target.x(), 100);
    EXPECT_EQ(target.y(), 200);
}

TEST(PointTests, test_equal_op)
{
    Point source(100,200);
    Point target(100,200);

    EXPECT_EQ(target, source);
}

TEST(PointTests, test_nequal_op)
{
    Point source(100,200);
    Point target(10,20);

    EXPECT_NE(target, source);
}

TEST(PointTests, test_x)
{
    Point target;

    target.x(-100);
    EXPECT_EQ(target.x(), -100);

    target.x(100);
    EXPECT_EQ(target.x(), 100);
}

TEST(PointTests, test_y)
{
    Point target;

    target.y(-100);
    EXPECT_EQ(target.y(), -100);

    target.y(100);
    EXPECT_EQ(target.y(), 100);
}

