#include "dlgcpp/controls/image.h"
#include "image_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(ImageTests, constructor_default)
{
    auto target = std::make_shared<Image>();

    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(ImageTests, constructor_with_params)
{
    auto target = std::make_shared<Image>(Position{ 1,2,3,4 });

    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}

TEST_F(ImageTests, autoSize)
{
    auto target = std::make_shared<Image>();

    EXPECT_EQ(target->autoSize(), false);

    target->autoSize(true);
    EXPECT_EQ(target->autoSize(), true);

    target->autoSize(false);
    EXPECT_EQ(target->autoSize(), false);

    // TODO: also need test to check control size after image load
}

TEST_F(ImageTests, centered)
{
    auto target = std::make_shared<Image>();

    EXPECT_EQ(target->centered(), false);

    target->centered(true);
    EXPECT_EQ(target->centered(), true);

    target->centered(false);
    EXPECT_EQ(target->centered(), false);
}

TEST_F(ImageTests, source)
{
    auto target = std::make_shared<Image>();

    EXPECT_EQ(target->source(), ImageSource{});

    auto testImage = ImageSource{ "img_id" };
    target->source(testImage);
    EXPECT_EQ(target->source(), testImage);

    target->source({});
    EXPECT_EQ(target->source(), ImageSource{});
}
