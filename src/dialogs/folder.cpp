#include "dialogs/folder_impl.h"
#include "dlgcpp/dialogs/folder.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;

FolderDialog::FolderDialog(ISharedDialog parent)
    : FolderDialog(std::make_shared<FolderDialogImpl>(this), parent)
{
}

FolderDialog::FolderDialog(
    std::shared_ptr<FolderDialogImpl> impl,
    ISharedDialog parent)
    : _impl(std::move(impl))
{
    auto ownerFn = [this]() -> ISharedFolderDialog { return shared_from_this(); };

    _parent.reset(parent, nullptr, ownerFn, "parent");
    _title.reset(std::string(), nullptr, ownerFn, "title");
    _message.reset(std::string(), nullptr, ownerFn, "message");
    _folderName.reset(std::string(), nullptr, ownerFn, "folderName");
}

IReadOnlyProperty<ISharedDialog, ISharedFolderDialog>& FolderDialog::parent()
{
    return _parent;
}

IProperty<std::string, ISharedFolderDialog>& FolderDialog::title()
{
    return _title;
}

IProperty<std::string, ISharedFolderDialog>& FolderDialog::message()
{
    return _message;
}

IProperty<std::string, ISharedFolderDialog>& FolderDialog::folderName()
{
    return _folderName;
}

bool FolderDialog::create()
{
    return _impl->create();
}

bool FolderDialog::open()
{
    return _impl->open();
}
