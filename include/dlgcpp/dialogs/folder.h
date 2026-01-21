#pragma once

#include "dlgcpp/defs.h"
#include "dlgcpp/property.h"
#include <string>

namespace dlgcpp
{
    namespace dialogs
    {
        class IFolderDialog;
        class FolderDialogImpl;
        typedef std::shared_ptr<IFolderDialog> ISharedFolderDialog;

        class IFolderDialog
        {
        public:
            virtual IReadOnlyProperty<ISharedDialog, ISharedFolderDialog>& parent() = 0;
            virtual IProperty<std::string, ISharedFolderDialog>& title() = 0;
            virtual IProperty<std::string, ISharedFolderDialog>& message() = 0;
            virtual IProperty<std::string, ISharedFolderDialog>& folderName() = 0;

            virtual bool create() = 0;
            virtual bool open() = 0;
        };

        class FolderDialog :
            public IFolderDialog,
            public std::enable_shared_from_this<FolderDialog>
        {
        public:
            explicit FolderDialog(ISharedDialog parent = nullptr);
            virtual ~FolderDialog() = default;

            // IFolderDialog impl.
            IReadOnlyProperty<ISharedDialog, ISharedFolderDialog>& parent() override;
            IProperty<std::string, ISharedFolderDialog>& title() override;
            IProperty<std::string, ISharedFolderDialog>& message() override;
            IProperty<std::string, ISharedFolderDialog>& folderName() override;
            bool create() override;
            bool open() override;

        private:
            FolderDialog(std::shared_ptr<FolderDialogImpl> impl, ISharedDialog parent);

            std::shared_ptr<FolderDialogImpl> _impl;
            Property<ISharedDialog, ISharedFolderDialog> _parent;
            Property<std::string, ISharedFolderDialog> _title;
            Property<std::string, ISharedFolderDialog> _message;
            Property<std::string, ISharedFolderDialog> _folderName;
        };
    }
}
