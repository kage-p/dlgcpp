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

Size& Size::operator=(const Size& other)
{
    _width = other._width;
    _height = other._height;
    return *this;
}

bool Size::operator==(const Size& other) const
{
    return _width == other._width &&
           _height == other._height;
}

bool Size::operator!=(const Size& other) const
{
    return !(*this == other);
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
