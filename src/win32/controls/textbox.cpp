#include "textbox_p.h"
#include "../utility.h"

using namespace dlgcpp::controls;

TextBox::TextBox(std::shared_ptr<IDialog> parent,
                 const std::string& text,
                 const Position& p)
    : Control(parent),
    _props(new tb_props())
{
    this->text(text);
    this->p(p);

    // TODO: need notify parameter (EN_CHANGED)
    // CommandEvent() += [this](){
    //     auto cb = (size_t)GetWindowTextLength((HWND)handle()) + 1;
    //     std::wstring buf(cb, 0);
    //     GetWindowTextW((HWND)handle(), &buf[0], cb);
    //     this->text(toBytes(buf.c_str()));
    // };
}

TextBox::~TextBox()
{
    delete _props;
}

std::string TextBox::className() const
{
    return "EDIT";
}

unsigned int TextBox::styles() const
{
    auto styles = Control::styles();
    if (_props->readOnly)
        styles |= ES_READONLY;
    if (_props->multiline)
        styles |= ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL;
    if (!_props->wrapText)
        styles |= ES_AUTOHSCROLL;
    return styles;
}

unsigned int TextBox::exStyles() const
{
    return Control::exStyles() | WS_EX_CLIENTEDGE;
}

bool TextBox::readOnly() const
{
    return _props->readOnly;
}

void TextBox::readOnly(bool value)
{
    if (_props->readOnly == value)
        return;
    _props->readOnly = value;
    rebuild();
}

bool TextBox::multiline() const
{
    return _props->multiline;
}

void TextBox::multiline(bool value)
{
    if (_props->multiline == value)
        return;
    _props->multiline = value;
    rebuild();
}

bool TextBox::wrapText() const
{
    return _props->wrapText;
}

void TextBox::wrapText(bool value)
{
    if (_props->wrapText == value)
        return;
    _props->wrapText = value;
    rebuild();
}
