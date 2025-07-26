#include "dialogs/folder_p.h"

using namespace dlgcpp::dialogs;

FolderDialog::FolderDialog(ISharedDialog parent)
    : FolderDialog(std::make_shared<FolderDialogImpl>(*this, parent))
{
}

FolderDialog::FolderDialog(std::shared_ptr<FolderDialogImpl> impl)
    : _impl(std::move(impl))
{
}

const std::string& FolderDialog::folderName() const
{
    return _impl->folderName();
}

void FolderDialog::folderName(const std::string& value)
{
    if (_impl->folderName() == value)
        return;
    _impl->folderName(value);
}

const std::string& FolderDialog::message() const
{
    return _impl->message();
}

void FolderDialog::message(const std::string& value)
{
    if (_impl->message() == value)
        return;
    _impl->message(value);
}

const std::string& FolderDialog::title() const
{
    return _impl->title();
}

void FolderDialog::title(const std::string& value)
{
    if (_impl->title() == value)
        return;
    _impl->title(value);
}

bool FolderDialog::create()
{
    return _impl->create();
}

bool FolderDialog::open()
{
    return _impl->open();
}
