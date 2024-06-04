#pragma once

#include "about.h"

#include <string>

namespace dlgcpp
{
    namespace dialogs
    {
        struct about_props
        {
            std::shared_ptr<IDialog> parent;
            std::string titleText;
            std::string appDetailsText;
            std::string descriptionText;
            std::string homePageLink;
            std::string releaseDateText;
            std::string logoBitmapId;
            std::string logoIconId;
        };

        struct about_state
        {
            std::shared_ptr<dlgcpp::controls::Image> logoImage;
            std::shared_ptr<dlgcpp::controls::Label> appDetails;
            std::shared_ptr<dlgcpp::controls::TextBox> description;
            std::shared_ptr<dlgcpp::controls::Label> releaseInfo;
            std::shared_ptr<dlgcpp::controls::Label> webLink;
            std::shared_ptr<dlgcpp::controls::Button> closeButton;
            std::shared_ptr<dlgcpp::controls::Button> sysInfoButton;
            std::shared_ptr<dlgcpp::controls::Label> copyrightLabel;
        };
    }
}
