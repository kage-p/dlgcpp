#include "dlgcpp/defs.h"

using namespace dlgcpp;

Size::Size(int width, int height) :
    _width(width), _height(height)
{
}

Size::Size(const Size& other) :
    _width(other._width), _height(other._height)
{
}

int Size::width() const
{
    return _width;
}

void Size::width(int value)
{
    _width = value;
}

int Size::height() const
{
    return _height;
}

void Size::height(int value)
{
    _height = value;
}

bool Size::empty() const
{
    return (_width * _height) < 1;
}
