#include "folder_p.h"
#include "../utility.h"
#include <Shlobj.h>

using namespace dlgcpp;
using namespace dlgcpp::dialogs;

FolderDialog::FolderDialog(ISharedDialog parent)
    : _props(new folder_props())
{
    _props->parent = parent;
}

FolderDialog::~FolderDialog()
{
    delete _props;
}

const std::string& FolderDialog::folderName() const
{
    return _props->folderName;
}

void FolderDialog::folderName(const std::string& value)
{
    _props->folderName = value;
}

const std::string& FolderDialog::message() const
{
    return _props->message;
}

void FolderDialog::message(const std::string& value)
{
    _props->message = value;
}

const std::string& FolderDialog::title() const
{
    return _props->title;
}

void FolderDialog::title(const std::string& value)
{
    _props->title = value;
}

bool FolderDialog::create()
{
    unsigned int flags = BIF_EDITBOX;
    std::string title = _props->title;
    return show(flags);
}

bool FolderDialog::open()
{
    unsigned int flags = BIF_NONEWFOLDERBUTTON;
    return show(flags);
}

int CALLBACK browseFolderProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    auto props = (folder_props*)lpData;

    switch (uMsg)
    {
    case BFFM_INITIALIZED:
        // initialize additional properties
        if (!props->folderName.empty())
            SendMessageW(hwnd, BFFM_SETSELECTION, 1, (LPARAM)toWide(props->folderName).c_str());
        if (!props->title.empty())
            SetWindowTextW(hwnd, toWide(props->title).c_str());
        break;

    case BFFM_SELCHANGED:
        // folder was selected
        break;

    }
    return 0;
}

bool FolderDialog::show(unsigned int flags)
{

    auto bi = BROWSEINFOW();

    auto titleBuf = toWide(_props->message);
    bi.lpszTitle = titleBuf.c_str();

    if (_props->parent != nullptr)
        bi.hwndOwner = reinterpret_cast<HWND>(_props->parent->handle());

    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN | BIF_USENEWUI | flags;
    bi.lpfn = &browseFolderProc;
    bi.lParam = (LPARAM)_props;

    auto pidlList = SHBrowseForFolderW(&bi);

    std::wstring buf(MAX_PATH, 0);
    if ((pidlList == NULL) || (SHGetPathFromIDListW(pidlList, &buf[0]) == FALSE) )
        return false;

    _props->folderName = toBytes(buf.c_str());
    auto c = _props->folderName.back();
    if (c == '\\' || c == '/')
        _props->folderName.pop_back();
    return true;
}
