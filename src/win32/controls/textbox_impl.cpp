#include "textbox_impl.h"
#include "utility/string_encoder.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

TextBoxImpl::TextBoxImpl() :
    ControlImpl()
{
}

TextBoxImpl::~TextBoxImpl()
{
}

void TextBoxImpl::owner(ITextBox* textBox)
{
    ControlImpl::owner(textBox);

    _textBox = textBox;

    _textBox->maxChars().event() += [&](auto) { onMaxCharsChanged(); };
    _textBox->password().event() += [&](auto) { rebuild(); };
    _textBox->readOnly().event() += [&](auto) { onReadOnlyChanged(); };
    _textBox->multiline().event() += [&](auto) { rebuild(); };
    _textBox->wrapText().event() += [&](auto) { rebuild(); };
    _textBox->horizontalAlignment().event() += [&](auto) { rebuild(); };
    _textBox->text().event() += [&](auto) { setText(_textBox->text()); };
}

void TextBoxImpl::build()
{
    ControlImpl::build();

    if (hwnd() == 0)
        return;

    SendMessage(hwnd(), EM_LIMITTEXT, (WPARAM)_textBox->maxChars(), 0);
    setText(_textBox->text());
}

std::string TextBoxImpl::className() const
{
    return "EDIT";
}

unsigned int TextBoxImpl::styles() const
{
    auto styles = ControlImpl::styles();
    if (_textBox->password())
        styles |= ES_PASSWORD;
    if (_textBox->readOnly())
        styles |= ES_READONLY;

    if (_textBox->multiline())
    {
        styles |= ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | WS_VSCROLL;
        if (!_textBox->wrapText())
            styles |= ES_AUTOHSCROLL | WS_HSCROLL;
    }
    else
        styles |= ES_AUTOHSCROLL;

    switch (_textBox->horizontalAlignment())
    {
    case HorizontalAlign::Left:
        styles |= ES_LEFT;
        break;
    case HorizontalAlign::Center:
        styles |= ES_CENTER;
        break;
    case HorizontalAlign::Right:
        styles |= ES_RIGHT;
        break;
    }

    return styles;
}

void TextBoxImpl::notify(DialogMessage& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == EN_CHANGE)
        {
            readInput();
            _textBox->ChangedEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == EN_SETFOCUS)
        {
            _textBox->focus() = true;
        }
        else if (HIWORD(msg.wParam) == EN_KILLFOCUS)
        {
            _textBox->focus() = false;
        }
    }
}

void TextBoxImpl::readInput()
{
    // if the control input has been changed by the user, save the content
    if (hwnd() == 0)
        return;

    _textBox->text() = getText(hwnd());
}

void TextBoxImpl::onMaxCharsChanged()
{
    auto maxChars = _textBox->maxChars().value();
    auto text = _textBox->text().value();

    // auto truncate
    if (maxChars > 0 && text.size() > (size_t)maxChars)
    {
        _textBox->text() = text.substr(0, (size_t)maxChars);
    }

    if (hwnd() == 0)
        return;

    SendMessage(hwnd(), EM_LIMITTEXT, (WPARAM)maxChars, 0);
}

void TextBoxImpl::onReadOnlyChanged()
{
    if (hwnd() == 0)
        return;

    SendMessage(hwnd(), EM_SETREADONLY, (WPARAM)_textBox->readOnly(), 0);
}
