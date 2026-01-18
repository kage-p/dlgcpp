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
            FolderDialogImpl(FolderDialog* folderDialog);
            ~FolderDialogImpl() = default;

            bool create();
            bool open();

        private:
            FolderDialog* _folderDialog = nullptr;

            bool show(unsigned int flags);

            static int CALLBACK browseFolderProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
        };
    }
}
