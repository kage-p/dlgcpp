#include "../utility.h"
#include "property_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

PropertyDialog::PropertyDialog(ISharedDialog parent) :
    _props(new propdlg_props()),
    _state(new propdlg_state())
{
    _props->parent = parent;
}

PropertyDialog::~PropertyDialog()
{
    delete _props;
}

const std::string& PropertyDialog::title() const
{
    return _props->title;
}

void PropertyDialog::title(const std::string& value)
{
    _props->title = value;
}

const std::string& PropertyDialog::message() const
{
    return _props->message;
}

void PropertyDialog::message(const std::string& value)
{
    _props->message = value;
}

int PropertyDialog::sectionWidth() const
{
    return _props->sectionWidth;
}

void PropertyDialog::sectionWidth(int value)
{
    _props->sectionWidth = value;
}

bool PropertyDialog::show()
{
    _state->propertyDialog = std::make_shared<Dialog>(DialogType::Application, _props->parent);
    _state->propertyDialog->title(_props->title);
    _state->propertyDialog->resize({ 400,275 });

    _state->sectionListBox = std::make_shared<ListBox>();
    _state->propertyDialog->add(_state->sectionListBox);

    _state->propertySheet = std::make_shared<Dialog>();
    _state->propertyDialog->add(_state->propertySheet);

    _state->propertyDialog->SizeEvent() += [this](ISharedDialog dlg)
        {
            auto width = dlg->p().width();
            auto height = dlg->p().height();

            // ##### PROPDLG1: complete property dialog
            Position pos;

            pos = Position{ 10,10, _props->sectionWidth, (height - 20) };
            _state->sectionListBox->p(pos);

            pos = Position{ pos.x() + pos.width() + 10, 10, (width / 3) * 2, (height - 20) };
            //_state->propertySheet->p(pos);
        };

    _state->propertyDialog->center();
    int r = _state->propertyDialog->exec();
    return (r == 0);
}
