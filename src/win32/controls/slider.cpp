#include "../dlgmsg.h"
#include "slider_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Slider::Slider(const std::string& text, const Position& p) :
    Control(text, p),
    _props(new slider_props())
{
    sldRegister();
    this->border(BorderStyle::Thin);
}

Slider::~Slider()
{
    delete _props;
}

void Slider::rebuild()
{
    Control::rebuild();

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    // apply the properties
    SendMessage(hwnd, SLM_SETDIRECTION, 0, _props->vertical ? SLS_VERTICAL : SLS_HORIZONTAL);
    SendMessage(hwnd, SLM_SETRANGE, (WPARAM)_props->range.first, (LPARAM)_props->range.second);
    SendMessage(hwnd, SLM_SETPOS, (WPARAM)_props->value, TRUE);
    SendMessage(hwnd, SLM_SETTEXTCOLOR, 0, (LPARAM)colors().first);
    SendMessage(hwnd, SLM_SETBKCOLOR, 0, (LPARAM)colors().second);
    SendMessage(hwnd, SLM_SETBARCOLOR, 0, (LPARAM)_props->barColor);
}

std::string Slider::className() const
{
    return SLDR_CLASS;
}

IEvent<ISharedControl>& Slider::ChangedEvent()
{
    return _props->changedEvent;
}

void Slider::notify(dlg_message& msg)
{
    if (msg.wMsg == WM_HSCROLL)
    {
        value((int)msg.wParam);
        ChangedEvent().invoke(shared_from_this());
    }
    else if (msg.wMsg == WM_VSCROLL)
    {
        value((int)msg.wParam);
        ChangedEvent().invoke(shared_from_this());
    }
    Control::notify(msg);
}

bool Slider::vertical() const
{
    return _props->vertical;
}

void Slider::vertical(bool value)
{
    if (_props->vertical == value)
        return;
    _props->vertical = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, SLM_SETDIRECTION, 0, _props->vertical ? SLS_VERTICAL : SLS_HORIZONTAL);
}

int Slider::value() const
{
    return _props->value;
}

void Slider::value(int value)
{
    if (_props->value == value)
        return;
    _props->value = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, SLM_SETPOS, (WPARAM)_props->value, TRUE);
}

std::pair<int, int> Slider::range() const
{
    return _props->range;
}

void Slider::range(int from, int to)
{
    if (_props->range == std::pair<int, int>(from, to))
        return;
    _props->range = std::pair<int, int>(from, to);

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, SLM_SETRANGE, (WPARAM)_props->range.first, (LPARAM)_props->range.second);
}

std::pair<Color, Color> Slider::colors() const
{
    return Control::colors();
}

void Slider::colors(Color fgColor, Color bgColor)
{
    Control::colors(fgColor, bgColor);

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, SLM_SETTEXTCOLOR, 0, (LPARAM)colors().first);
    SendMessage(hwnd, SLM_SETBKCOLOR, 0, (LPARAM)colors().second);
}

Color Slider::barColor() const
{
    return _props->barColor;
}

void Slider::barColor(Color value)
{
    if (_props->barColor == value)
        return;
    _props->barColor = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, SLM_SETBARCOLOR, 0, (LPARAM)_props->barColor);
}
