#pragma once

#include "dlgcpp/defs.h"
#include "dlgcpp/property.h"
#include <string>

namespace dlgcpp
{
    namespace dialogs
    {
        class IFileDialog;
        class FileDialogImpl;
        typedef std::shared_ptr<IFileDialog> ISharedFileDialog;

        class IFileDialog
        {
        public:
            virtual IReadOnlyProperty<ISharedDialog, ISharedFileDialog>& parent() = 0;
            virtual IProperty<std::string, ISharedFileDialog>& title() = 0;
            virtual IProperty<std::string, ISharedFileDialog>& fileName() = 0;
            virtual IProperty<std::string, ISharedFileDialog>& filters() = 0;

            virtual bool create() = 0;
            virtual bool open() = 0;
            virtual bool save() = 0;
        };

        class FileDialog :
            public IFileDialog,
            public std::enable_shared_from_this<FileDialog>
        {
        public:
            explicit FileDialog(ISharedDialog parent = nullptr);
            virtual ~FileDialog() = default;

            // IFileDialog impl.
            IReadOnlyProperty<ISharedDialog, ISharedFileDialog>& parent() override;
            IProperty<std::string, ISharedFileDialog>& title() override;
            IProperty<std::string, ISharedFileDialog>& fileName() override;
            IProperty<std::string, ISharedFileDialog>& filters() override;
            bool create() override;
            bool open() override;
            bool save() override;

        private:
            FileDialog(std::shared_ptr<FileDialogImpl> impl, ISharedDialog parent);

            std::shared_ptr<FileDialogImpl> _impl;
            Property<ISharedDialog, ISharedFileDialog> _parent;
            Property<std::string, ISharedFileDialog> _title;
            Property<std::string, ISharedFileDialog> _fileName;
            Property<std::string, ISharedFileDialog> _filters;
        };
    }
}
