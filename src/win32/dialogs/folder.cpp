#include "dlgcpp/dialogs/dialog.h"
#include "folder_p.h"
#include "utility/string_encoder.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shlobj.h>

using namespace dlgcpp;
using namespace dlgcpp::dialogs;

FolderDialogImpl::FolderDialogImpl(
    FolderDialog& folderDialog,
    ISharedDialog parent)
    : _folderDialog(folderDialog),
    _parent(parent)
{
}

const std::string& FolderDialogImpl::folderName() const
{
    return _folderName;
}

void FolderDialogImpl::folderName(const std::string& value)
{
    _folderName = value;
}

const std::string& FolderDialogImpl::message() const
{
    return _message;
}

void FolderDialogImpl::message(const std::string& value)
{
    _message = value;
}

const std::string& FolderDialogImpl::title() const
{
    return _title;
}

void FolderDialogImpl::title(const std::string& value)
{
    _title = value;
}

bool FolderDialogImpl::create()
{
    unsigned int flags = BIF_EDITBOX;
    std::string title = _title;
    return show(flags);
}

bool FolderDialogImpl::open()
{
    unsigned int flags = BIF_NONEWFOLDERBUTTON;
    return show(flags);
}

int CALLBACK FolderDialogImpl::browseFolderProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    auto impl = (FolderDialogImpl*)lpData;

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

    auto titleBuf = StringEncoder::toWide(_message);
    bi.lpszTitle = titleBuf.c_str();

    if (_parent != nullptr)
        bi.hwndOwner = reinterpret_cast<HWND>(_parent->handle());

    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN | BIF_USENEWUI | flags;
    bi.lpfn = &browseFolderProc;
    bi.lParam = (LPARAM)this;

    auto pidlList = SHBrowseForFolderW(&bi);

    std::wstring buf(MAX_PATH, 0);
    if ((pidlList == NULL) || (SHGetPathFromIDListW(pidlList, &buf[0]) == FALSE))
        return false;

    _folderName = StringEncoder::toBytes(buf.c_str());
    auto c = _folderName.back();
    if (c == '\\' || c == '/')
        _folderName.pop_back();
    return true;
}
