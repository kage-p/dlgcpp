#pragma once

#include "dlgcpp/dialogs/folder.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace dialogs
    {
        class FolderDialogImpl
        {
        public:
            FolderDialogImpl(FolderDialog& folderDialog, ISharedDialog parent);
            ~FolderDialogImpl() = default;

            const std::string& folderName() const;
            void folderName(const std::string&);
            const std::string& message() const;
            void message(const std::string&);
            const std::string& title() const;
            void title(const std::string&);
            bool create();
            bool open();

        private:
            FolderDialog& _folderDialog;
            ISharedDialog _parent;
            std::string _title;
            std::string _message;
            std::string _folderName;

            bool show(unsigned int flags);

            static int CALLBACK browseFolderProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
        };
    }
}
