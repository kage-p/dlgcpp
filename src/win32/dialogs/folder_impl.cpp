#include "dlgcpp/dialogs/dialog.h"
#include "folder_impl.h"
#include "utility/string_encoder.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shlobj.h>

using namespace dlgcpp;
using namespace dlgcpp::dialogs;

FolderDialogImpl::FolderDialogImpl(
    FolderDialog* folderDialog)
    : _folderDialog(folderDialog)
{
}

bool FolderDialogImpl::create()
{
    unsigned int flags = BIF_EDITBOX;
    std::string title = _folderDialog->title();
    return show(flags);
}

bool FolderDialogImpl::open()
{
    unsigned int flags = BIF_NONEWFOLDERBUTTON;
    return show(flags);
}

int CALLBACK FolderDialogImpl::browseFolderProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    auto impl = (FolderDialog*)lpData;

    switch (uMsg)
    {
    case BFFM_INITIALIZED:
        // initialize additional properties
        if (!impl->folderName().empty())
            SendMessageW(hwnd, BFFM_SETSELECTION, 1, (LPARAM)StringEncoder::toWide(impl->folderName()).c_str());

        if (!impl->title().empty())
            SetWindowTextW(hwnd, StringEncoder::toWide(impl->title()).c_str());
        break;

    case BFFM_SELCHANGED:
        // folder was selected
        break;

    }
    return 0;
}

bool FolderDialogImpl::show(unsigned int flags)
{
    auto bi = BROWSEINFOW();

    auto titleBuf = StringEncoder::toWide(_folderDialog->message());
    bi.lpszTitle = titleBuf.c_str();

    if (_folderDialog->parent() != nullptr)
        bi.hwndOwner = reinterpret_cast<HWND>(_folderDialog->parent().value()->handle().value());

    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN | BIF_USENEWUI | flags;
    bi.lpfn = &browseFolderProc;
    bi.lParam = (LPARAM)_folderDialog;

    auto pidlList = SHBrowseForFolderW(&bi);

    std::wstring buf(MAX_PATH, 0);
    if ((pidlList == NULL) || (SHGetPathFromIDListW(pidlList, &buf[0]) == FALSE))
        return false;

    auto folderName = StringEncoder::toBytes(buf.c_str());
    auto c = folderName.back();
    if (c == '\\' || c == '/')
        folderName.pop_back();

    _folderDialog->folderName() = folderName;

    return true;
}
