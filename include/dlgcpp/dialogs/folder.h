#pragma once

#include "dlgcpp/defs.h"
#include <string>

namespace dlgcpp
{
    namespace dialogs
    {
        class IFolderDialog
        {
        public:
            virtual const std::string& folderName() const = 0;
            virtual void folderName(const std::string&) = 0;
            virtual const std::string& message() const = 0;
            virtual void message(const std::string&) = 0;
            virtual const std::string& title() const = 0;
            virtual void title(const std::string&) = 0;
            virtual bool create() = 0;
            virtual bool open() = 0;
        };

        class FolderDialogImpl;

        class FolderDialog : public IFolderDialog
        {
        public:
            explicit FolderDialog(ISharedDialog parent = nullptr);
            virtual ~FolderDialog() = default;

            // IFolderDialog impl.
            const std::string& folderName() const override;
            void folderName(const std::string&) override;
            const std::string& message() const override;
            void message(const std::string&) override;
            const std::string& title() const override;
            void title(const std::string&) override;
            bool create() override;
            bool open() override;

        private:
            FolderDialog(std::shared_ptr<FolderDialogImpl> impl);

            std::shared_ptr<FolderDialogImpl> _impl;
        };
    }
}
