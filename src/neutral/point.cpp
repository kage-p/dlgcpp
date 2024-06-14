#include "dlgcpp/defs.h"

using namespace dlgcpp;

Point::Point(int x, int y) :
    _x(x), _y(y)
{
}

Point::Point(const Point& other) :
    _x(other._x), _y(other._y)
{
}

int Point::x() const
{
    return _x;
}

void Point::x(int value)
{
    _x = value;
}

int Point::y() const
{
    return _y;
}

void Point::y(int value)
{
    _y = value;
}
