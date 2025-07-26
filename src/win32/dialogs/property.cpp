#include "property_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

PropertyDialogImpl::PropertyDialogImpl(
    PropertyDialog& propertyDialog,
    ISharedDialog parent) :
    _propertyDialog(propertyDialog)
{
    _parent = parent;
}

const std::string& PropertyDialogImpl::title() const
{
    return _title;
}

void PropertyDialogImpl::title(const std::string& value)
{
    _title = value;
}

const std::string& PropertyDialogImpl::message() const
{
    return _message;
}

void PropertyDialogImpl::message(const std::string& value)
{
    _message = value;
}

int PropertyDialogImpl::sectionWidth() const
{
    return _sectionWidth;
}

void PropertyDialogImpl::sectionWidth(int value)
{
    _sectionWidth = value;
}

bool PropertyDialogImpl::show()
{
    _dialog = std::make_shared<Dialog>(DialogType::Application, _parent);
    _dialog->title(_title);
    _dialog->resize({ 400,275 });

    _sectionListBox = std::make_shared<ListBox>();
    _dialog->add(_sectionListBox);

    _propertySheet = std::make_shared<Dialog>();
    _dialog->add(_propertySheet);

    _dialog->SizeEvent() += [this](ISharedDialog dlg)
        {
            auto width = dlg->p().width();
            auto height = dlg->p().height();

            // TODO: complete property dialog
            Position pos;

            pos = Position{ 10,10, _sectionWidth, (height - 20) };
            _sectionListBox->p(pos);

            pos = Position{ pos.x() + pos.width() + 10, 10, (width / 3) * 2, (height - 20) };
            //_propertySheet->p(pos);
        };

    _dialog->center();
    int r = _dialog->exec();
    return (r == 0);
}
