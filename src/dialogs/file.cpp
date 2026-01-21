#include "dialogs/file_impl.h"
#include "dlgcpp/dialogs/file.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;

FileDialog::FileDialog(ISharedDialog parent)
    : FileDialog(std::make_shared<FileDialogImpl>(this), parent)
{
}

FileDialog::FileDialog(
    std::shared_ptr<FileDialogImpl> impl,
    ISharedDialog parent)
    : _impl(std::move(impl))
{
    auto ownerFn = [this]() -> ISharedFileDialog { return shared_from_this(); };

    _parent.reset(parent, nullptr, ownerFn, "parent");
    _title.reset(std::string(), nullptr, ownerFn, "title");
    _fileName.reset(std::string(), nullptr, ownerFn, "fileName");
    _filters.reset(std::string(), nullptr, ownerFn, "filters");
}

IReadOnlyProperty<ISharedDialog, ISharedFileDialog>& FileDialog::parent()
{
    return _parent;
}

IProperty<std::string, ISharedFileDialog>& FileDialog::title()
{
    return _title;
}

IProperty<std::string, ISharedFileDialog>& FileDialog::fileName()
{
    return _fileName;
}

IProperty<std::string, ISharedFileDialog>& FileDialog::filters()
{
    return _filters;
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
