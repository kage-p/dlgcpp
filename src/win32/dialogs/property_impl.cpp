#include "property_impl.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

PropertyDialogImpl::PropertyDialogImpl(
    PropertyDialog* propertyDialog) :
    _propertyDialog(propertyDialog)
{
}

bool PropertyDialogImpl::show()
{
    _dialog = std::make_shared<Dialog>(DialogType::Application, _propertyDialog->parent());
    _dialog->title(_propertyDialog->title());
    _dialog->resize({ 400,275 });

    _sectionListBox = std::make_shared<ListBox>();
    _dialog->add(_sectionListBox);

    _propertySheet = std::make_shared<Dialog>();
    _dialog->add(_propertySheet);

    _dialog->SizeEvent() += [this](ISharedDialog dlg)
        {
            auto width = dlg->p()->width();
            auto height = dlg->p()->height();

            // TODO: complete property dialog
            Position pos;

            pos = Position{ 10,10, _propertyDialog->sectionWidth(), (height - 20) };
            _sectionListBox->p(pos);

            pos = Position{ pos.x() + pos.width() + 10, 10, (width / 3) * 2, (height - 20) };
            //_propertySheet->p(pos);
        };

    _dialog->center();
    int r = _dialog->exec();
    return (r == 0);
}
