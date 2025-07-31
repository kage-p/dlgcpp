#include "dlgcpp/position.h"
#include "position_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::tests;

TEST_F(PositionTests, test_constructor_default)
{
    Position target;

    EXPECT_EQ(target.x(), 0);
    EXPECT_EQ(target.y(), 0);
    EXPECT_EQ(target.width(), 0);
    EXPECT_EQ(target.height(), 0);
}

TEST_F(PositionTests, test_constructor_with_params)
{
    Position target(100, 200, 300, 400);

    EXPECT_EQ(target.x(), 100);
    EXPECT_EQ(target.y(), 200);
    EXPECT_EQ(target.width(), 300);
    EXPECT_EQ(target.height(), 400);
}

TEST_F(PositionTests, test_constructor_with_copy)
{
    Position source(100, 200, 300, 400);
    Position target(source);

    EXPECT_EQ(target.x(), 100);
    EXPECT_EQ(target.y(), 200);
    EXPECT_EQ(target.width(), 300);
    EXPECT_EQ(target.height(), 400);
}

TEST_F(PositionTests, test_assignment_op)
{
    Position source(100, 200, 300, 400);
    Position target;

    target = source;

    EXPECT_EQ(target.x(), 100);
    EXPECT_EQ(target.y(), 200);
    EXPECT_EQ(target.width(), 300);
    EXPECT_EQ(target.height(), 400);
}

TEST_F(PositionTests, test_equal_op)
{
    Position source(100, 200, 300, 400);
    Position target(100, 200, 300, 400);

    EXPECT_EQ(target, source);
}

TEST_F(PositionTests, test_nequal_op)
{
    Position source(100, 200, 300, 400);
    Position target(10, 20, 30, 40);

    EXPECT_NE(target, source);
}

TEST_F(PositionTests, test_width)
{
    Position target;

    EXPECT_EQ(target.width(), 0);

    target.width(-100);
    EXPECT_EQ(target.width(), -100);

    target.width(100);
    EXPECT_EQ(target.width(), 100);
}

TEST_F(PositionTests, test_height)
{
    Position target;

    EXPECT_EQ(target.height(), 0);

    target.height(-100);
    EXPECT_EQ(target.height(), -100);

    target.height(100);
    EXPECT_EQ(target.height(), 100);
}

TEST_F(PositionTests, test_empty)
{
    Position target;

    EXPECT_EQ(target.empty(), true);

    target.width(100);
    EXPECT_EQ(target.empty(), true);

    target.height(100);
    EXPECT_EQ(target.empty(), false);
}

TEST_F(PositionTests, test_point)
{
    Position target(100, 200, 0, 0);

    EXPECT_EQ(target.point(), Point(100, 200));
}

TEST_F(PositionTests, test_size)
{
    Position target(0, 0, 100, 200);

    EXPECT_EQ(target.size(), Size(100, 200));
}


