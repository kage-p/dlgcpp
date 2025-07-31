#include "dlgcpp/dialogs/dialog.h"
#include "file_p.h"
#include "utility/string_encoder.h"

using namespace dlgcpp::dialogs;

FileDialogImpl::FileDialogImpl(
    FileDialog& fileDialog,
    ISharedDialog parent)
    : _fileDialog(fileDialog),
    _parent(parent)
{
}

const std::string& FileDialogImpl::fileName() const
{
    return _fileName;
}

void FileDialogImpl::fileName(const std::string& value)
{
    _fileName = value;
}

const std::string& FileDialogImpl::filters() const
{
    return _filters;
}

void FileDialogImpl::filters(const std::string& value)
{
    _filters = value;
}

const std::string& FileDialogImpl::title() const
{
    return _title;
}

void FileDialogImpl::title(const std::string& value)
{
    _title = value;
}

bool FileDialogImpl::create()
{
    unsigned int flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_NOREADONLYRETURN;
    std::string title = _title;
    if (title.empty())
        title = "Create File";
    return show(false, flags, title);
}

bool FileDialogImpl::open()
{
    // TODO: support OFN_ALLOWMULTISELECT (open only)
    unsigned int flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_FILEMUSTEXIST;
    std::string title = _title;
    if (title.empty())
        title = "Open File";
    return show(false, flags, title);
}

bool FileDialogImpl::save()
{
    unsigned int flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_NOREADONLYRETURN;
    std::string title = _title;
    if (title.empty())
        title = "Save File";
    return show(true, flags, title);
}

bool FileDialogImpl::show(
    bool isSaveFile,
    unsigned int flags,
    const std::string& title)
{
    _extErr = 0;

    auto ofn = OPENFILENAMEW();
    ofn.lStructSize = sizeof(OPENFILENAMEW);
    if (_parent != nullptr)
        ofn.hwndOwner = reinterpret_cast<HWND>(_parent->handle());

    ofn.Flags = flags;

    std::wstring wstrTitle = StringEncoder::toWide(title);
    ofn.lpstrTitle = wstrTitle.c_str();

    std::wstring wstrFile(MAX_PATH, 0x0);
    if (!_fileName.empty())
        wstrFile.insert(0, StringEncoder::toWide(_fileName));

    ofn.lpstrFile = &wstrFile[0];
    ofn.nMaxFile = (DWORD)wstrFile.size();

    std::wstring wstrFilters(MAX_PATH, 0x0);
    std::string filters = _filters;
    if (!filters.empty())
    {
        filters += "|All Files (*.*)|*.*||";
        wstrFilters.insert(0, StringEncoder::toWide(filters));
        for (auto& c : wstrFilters) // null-splitter
            if (c == '|') c = 0;
        ofn.lpstrFilter = wstrFilters.c_str();
        ofn.nFilterIndex = _filterIndex;
    }

    // TODO: support dir init and def ext (for create/save)
    //ofn.lpstrInitialDir = szStartTmp;
    //ofn.lpstrDefExt = szExtTmp;

    BOOL r = FALSE;
    if (isSaveFile)
        r = GetSaveFileNameW(&ofn);
    else
        r = GetOpenFileNameW(&ofn);

    if (r == FALSE)
    {
        _extErr = CommDlgExtendedError();
        return false;
    }

    _fileName = StringEncoder::toBytes(wstrFile.c_str());
    _filterIndex = ofn.nFilterIndex;
    return true;
}
