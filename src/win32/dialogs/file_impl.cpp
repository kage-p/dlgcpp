#include "dlgcpp/dialogs/dialog.h"
#include "file_impl.h"
#include "utility/string_encoder.h"

using namespace dlgcpp::dialogs;

FileDialogImpl::FileDialogImpl(
    FileDialog* fileDialog)
    : _fileDialog(fileDialog)
{
}

bool FileDialogImpl::create()
{
    unsigned int flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_NOREADONLYRETURN;
    std::string title = _fileDialog->title();
    if (title.empty())
        title = "Create File";
    return show(false, flags, title);
}

bool FileDialogImpl::open()
{
    // TODO: support OFN_ALLOWMULTISELECT (open only)
    unsigned int flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_FILEMUSTEXIST;
    std::string title = _fileDialog->title();
    if (title.empty())
        title = "Open File";
    return show(false, flags, title);
}

bool FileDialogImpl::save()
{
    unsigned int flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_NOREADONLYRETURN;
    std::string title = _fileDialog->title();
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
    if (!_fileDialog->parent().empty())
        ofn.hwndOwner = reinterpret_cast<HWND>(_fileDialog->parent().value()->handle().value());

    ofn.Flags = flags;

    std::wstring wstrTitle = StringEncoder::toWide(title);
    ofn.lpstrTitle = wstrTitle.c_str();

    std::wstring wstrFile(MAX_PATH, 0x0);
    if (!_fileDialog->fileName().empty())
        wstrFile.insert(0, StringEncoder::toWide(_fileDialog->fileName()));

    ofn.lpstrFile = &wstrFile[0];
    ofn.nMaxFile = (DWORD)wstrFile.size();

    std::wstring wstrFilters(MAX_PATH, 0x0);
    std::string filters = _fileDialog->filters();
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

    _fileDialog->fileName() = StringEncoder::toBytes(wstrFile.c_str());
    _filterIndex = ofn.nFilterIndex;
    return true;
}
