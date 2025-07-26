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
            explicit PropertyDialogImpl(PropertyDialog& propertyDialog, ISharedDialog parent = nullptr);
            virtual ~PropertyDialogImpl() = default;

            const std::string& title() const;
            void title(const std::string& value);
            const std::string& message() const;
            void message(const std::string& value);
            int sectionWidth() const;
            void sectionWidth(int value);
            bool show();

        private:
            PropertyDialog& _propertyDialog;
            ISharedDialog _parent;
            std::string _title;
            std::string _message;
            int _sectionWidth = 150;

            // state properties
            std::shared_ptr<dlgcpp::dialogs::Dialog> _dialog;
            std::shared_ptr<dlgcpp::controls::ListBox> _sectionListBox;
            std::shared_ptr<dlgcpp::dialogs::Dialog> _propertySheet;
        };
    }
}
