#include "dlgcpp/point.h"
#include "point_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::tests;

TEST_F(PointTests, constructor_default)
{
    Point target;

    EXPECT_EQ(target.x(), 0);
    EXPECT_EQ(target.y(), 0);
}

TEST_F(PointTests, constructor_with_params)
{
    Point target(100, 200);

    EXPECT_EQ(target.x(), 100);
    EXPECT_EQ(target.y(), 200);
}

TEST_F(PointTests, constructor_with_copy)
{
    Point source(100, 200);
    Point target(source);

    EXPECT_EQ(target.x(), 100);
    EXPECT_EQ(target.y(), 200);
}

TEST_F(PointTests, op_assignment)
{
    Point source(100, 200);
    Point target;

    target = source;
    EXPECT_EQ(target.x(), 100);
    EXPECT_EQ(target.y(), 200);
}

TEST_F(PointTests, op_equal)
{
    Point source(100, 200);
    Point target(100, 200);

    EXPECT_EQ(target, source);
}

TEST_F(PointTests, op_nequal)
{
    Point source(100, 200);
    Point target(10, 20);

    EXPECT_NE(target, source);
}

TEST_F(PointTests, x)
{
    Point target;

    target.x(-100);
    EXPECT_EQ(target.x(), -100);

    target.x(100);
    EXPECT_EQ(target.x(), 100);
}

TEST_F(PointTests, y)
{
    Point target;

    target.y(-100);
    EXPECT_EQ(target.y(), -100);

    target.y(100);
    EXPECT_EQ(target.y(), 100);
}

