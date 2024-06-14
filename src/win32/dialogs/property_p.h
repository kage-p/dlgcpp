#pragma once

#include "dlgcpp/dialogs/property.h"

#include <string>

namespace dlgcpp
{
    namespace dialogs
    {
        struct propdlg_props
        {
            ISharedDialog parent;
            std::string title;
            std::string message;
            int sectionWidth = 150;
        };

        struct propdlg_state
        {
            std::shared_ptr<dlgcpp::Dialog> propertyDialog;
            std::shared_ptr<dlgcpp::controls::ListBox> sectionListBox;
            std::shared_ptr<dlgcpp::Dialog> propertySheet;
        };
    }
}
