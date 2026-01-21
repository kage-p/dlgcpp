#pragma once

#include "dlgcpp/controls/listbox.h"
#include "dlgcpp/dialogs/dialog.h"
#include "dlgcpp/dialogs/property.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace dialogs
    {
        class PropertyDialogImpl
        {
        public:
            explicit PropertyDialogImpl(PropertyDialog* propertyDialog);
            virtual ~PropertyDialogImpl() = default;

            bool show();

        private:
            PropertyDialog* _propertyDialog = nullptr;

            // state properties
            std::shared_ptr<dlgcpp::dialogs::Dialog> _dialog;
            std::shared_ptr<dlgcpp::controls::ListBox> _sectionListBox;
            std::shared_ptr<dlgcpp::dialogs::Dialog> _propertySheet;
        };
    }
}
