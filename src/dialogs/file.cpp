#include "dlgcpp/dialogs/file.h"
#include "dialogs/file_p.h"

using namespace dlgcpp::dialogs;

FileDialog::FileDialog(ISharedDialog parent)
    : FileDialog(std::make_shared<FileDialogImpl>(*this, parent))
{
}

FileDialog::FileDialog(std::shared_ptr<FileDialogImpl> impl)
    : _impl(std::move(impl))
{
}

const std::string& FileDialog::fileName() const
{
    return _impl->fileName();
}

void FileDialog::fileName(const std::string& value)
{
    if (_impl->fileName() == value)
        return;
    _impl->fileName(value);
}

const std::string& FileDialog::filters() const
{
    return _impl->filters();
}

void FileDialog::filters(const std::string& value)
{
    if (_impl->filters() == value)
        return;
    _impl->filters(value);
}

const std::string& FileDialog::title() const
{
    return _impl->title();
}

void FileDialog::title(const std::string& value)
{
    if (_impl->title() == value)
        return;
    _impl->title(value);
}

bool FileDialog::create()
{
    return _impl->create();
}

bool FileDialog::open()
{
    return _impl->open();
}

bool FileDialog::save()
{
    return _impl->save();
}
