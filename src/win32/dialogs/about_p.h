#pragma once

#include "dlgcpp/dialogs/about.h"

#include <string>

namespace dlgcpp
{
    namespace dialogs
    {
        struct about_props
        {
            ISharedDialog parent;
            std::string title;
            std::string appDetails;
            std::string description;
            std::string homePageLink;
            std::string releaseDate;
            std::string logoBitmapId;
            std::string logoIconId;
        };

        struct about_state
        {

        };
    }
}
