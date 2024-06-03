#pragma once

#include "dlgcpp.h"

#include <memory>
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
            virtual const std::string& title() const = 0;
            virtual void title(const std::string&) = 0;
            virtual bool create() = 0;
            virtual bool open() = 0;
            virtual bool save() = 0;
        };

        class FolderDialog : public IFolderDialog
        {
        public:
            explicit FolderDialog(std::shared_ptr<IDialog> parent = nullptr);

            // IFolderDialog impl.
            const std::string& folderName() const override;
            void folderName(const std::string&) override;
            const std::string& title() const override;
            void title(const std::string&) override;
            bool create() override;
            bool open() override;
            bool save() override;

        private:
            std::shared_ptr<IDialog> _parent;
            std::string _folderName;
            std::string _title;

            //bool show(bool isSaveFile, unsigned int flags, const std::string& defaultTitle);
        };
    }
}
