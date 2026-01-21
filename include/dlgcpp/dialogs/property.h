#pragma once

#include "dlgcpp/defs.h"
#include "dlgcpp/property.h"
#include <ostream>
#include <string>

namespace dlgcpp
{
    namespace dialogs
    {
        enum class PropertyDialogOption
        {
            Check = 0,
            List,
            Text,
        };

        inline std::ostream& operator<<(std::ostream& os, PropertyDialogOption e)
        {
            switch (e) {
            case PropertyDialogOption::Check:   return os << "Check";
            case PropertyDialogOption::List:  return os << "List";
            case PropertyDialogOption::Text: return os << "Text";
            }
            return os << static_cast<int>(e); // fallback
        }

        class IPropertyDialog;
        class PropertyDialogImpl;
        typedef std::shared_ptr<IPropertyDialog> ISharedPropertyDialog;

        class IPropertyDialog
        {
        public:
            virtual IReadOnlyProperty<ISharedDialog, ISharedPropertyDialog>& parent() = 0;
            virtual IProperty<std::string, ISharedPropertyDialog>& title() = 0;
            virtual IProperty<std::string, ISharedPropertyDialog>& message() = 0;
            virtual IProperty<int, ISharedPropertyDialog>& sectionWidth() = 0;

            virtual bool show() = 0;
        };

        class PropertyDialog :
            public IPropertyDialog,
            public std::enable_shared_from_this<PropertyDialog>
        {
        public:
            explicit PropertyDialog(ISharedDialog parent = nullptr);
            virtual ~PropertyDialog() = default;

            // IPropertyDialog impl.
            IReadOnlyProperty<ISharedDialog, ISharedPropertyDialog>& parent() override;
            IProperty<std::string, ISharedPropertyDialog>& title() override;
            IProperty<std::string, ISharedPropertyDialog>& message() override;
            IProperty<int, ISharedPropertyDialog>& sectionWidth() override;
            bool show() override;

        private:
            PropertyDialog(std::shared_ptr<PropertyDialogImpl> impl, ISharedDialog parent);

            std::shared_ptr<PropertyDialogImpl> _impl;
            Property<ISharedDialog, ISharedPropertyDialog> _parent;
            Property<std::string, ISharedPropertyDialog> _title;
            Property<std::string, ISharedPropertyDialog> _message;
            Property<int, ISharedPropertyDialog> _sectionWidth;
        };
    }
}
