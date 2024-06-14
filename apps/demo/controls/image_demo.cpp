#include "image_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

void controls_image_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Application, parent);
    dlg->title("Image control demo");
    dlg->resize({250,175});
    dlg->center();

    auto icon = std::make_shared<Image>(Position{10, 10, 0, 0});
    icon->autoSize(true);
    icon->image(ImageSource{"#100", true});
    dlg->add(icon);

    auto bitmap = std::make_shared<Image>(Position{10, 40, 200, 120});
    bitmap->autoSize(false);
    bitmap->image(ImageSource{"#100", false});
    dlg->add(bitmap);

    dlg->exec();
}
