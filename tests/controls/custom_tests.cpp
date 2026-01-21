#include "custom_tests.h"
#include "dlgcpp/controls/custom.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(CustomTests, constructor_default)
{
    auto target = std::make_shared<Custom>();

    EXPECT_EQ(target->className(), std::string());
    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(CustomTests, constructor_with_params)
{
    auto target = std::make_shared<Custom>("class", Position{ 1,2,3,4 });

    EXPECT_EQ(target->className(), std::string("class"));
    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}
