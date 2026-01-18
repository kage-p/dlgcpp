#pragma once

#include "dlgcpp/dialogs/file.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <CommDlg.h>

namespace dlgcpp
{
    namespace dialogs
    {
        class FileDialogImpl
        {
        public:
            FileDialogImpl(FileDialog* fileDialog);
            ~FileDialogImpl() = default;

            const std::string& fileName() const;
            void fileName(const std::string&);
            const std::string& filters() const;
            void filters(const std::string&);
            const std::string& title() const;
            void title(const std::string&);
            bool create();
            bool open();
            bool save();

        private:
            FileDialog* _fileDialog = nullptr;
            int _filterIndex = 0;
            int _extErr = 0;

            bool show(
                bool isSaveFile,
                unsigned int flags,
                const std::string& title);
        };
    }
}
