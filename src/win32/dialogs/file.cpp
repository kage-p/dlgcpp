#include "file_p.h"
#include "../utility.h"
#include <CommDlg.h>

using namespace dlgcpp::dialogs;

FileDialog::FileDialog(ISharedDialog parent)
    : _props(new file_props())
{
    _props->parent = parent;
}

FileDialog::~FileDialog()
{
    delete _props;
}

const std::string& FileDialog::fileName() const
{
    return _props->fileName;
}

void FileDialog::fileName(const std::string& value)
{
    _props->fileName = value;
}

const std::string& FileDialog::filters() const
{
    return _props->filters;
}

void FileDialog::filters(const std::string& value)
{
    _props->filters = value;
}

const std::string& FileDialog::title() const
{
    return _props->title;
}

void FileDialog::title(const std::string& value)
{
    _props->title = value;
}

bool FileDialog::create()
{
    unsigned int flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_NOREADONLYRETURN;
    std::string title = _props->title;
    if (title.empty())
        title = "Create File";
    return show(false, flags, title);
}

bool FileDialog::open()
{
    // TODO: support OFN_ALLOWMULTISELECT (open only)
    unsigned int flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_FILEMUSTEXIST;
    std::string title = _props->title;
    if (title.empty())
        title = "Open File";
    return show(false, flags, title);
}

bool FileDialog::save()
{
    unsigned int flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_NOREADONLYRETURN;
    std::string title = _props->title;
    if (title.empty())
        title = "Save File";
    return show(true, flags, title);
}

bool FileDialog::show(bool isSaveFile,
                      unsigned int flags,
                      const std::string& title)
{
    _props->extErr = 0;

    auto ofn = OPENFILENAMEW();
    ofn.lStructSize = sizeof(OPENFILENAMEW);
    if (_props->parent != nullptr)
        ofn.hwndOwner = reinterpret_cast<HWND>(_props->parent->handle());

    ofn.Flags = flags;

    std::wstring wstrTitle = toWide(title);
    ofn.lpstrTitle = wstrTitle.c_str();

    std::wstring wstrFile(MAX_PATH, 0x0);
    if (!_props->fileName.empty())
        wstrFile.insert(0, toWide(_props->fileName));

    ofn.lpstrFile = &wstrFile[0];
    ofn.nMaxFile = (DWORD)wstrFile.size();

    std::wstring wstrFilters(MAX_PATH, 0x0);
    std::string filters = _props->filters;
    if (!filters.empty())
    {
        filters += "|All Files (*.*)|*.*||";
        wstrFilters.insert(0, toWide(filters));
        for (auto &c : wstrFilters) // null-splitter
            if (c == '|') c = 0;
        ofn.lpstrFilter = wstrFilters.c_str();
        ofn.nFilterIndex = _props->filterIndex;
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
        _props->extErr = CommDlgExtendedError();
        return false;
    }

    _props->fileName = toBytes(wstrFile.c_str());
    _props->filterIndex = ofn.nFilterIndex;
    return true;
}
