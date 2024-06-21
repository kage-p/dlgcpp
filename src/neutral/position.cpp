#include "dlgcpp/defs.h"

using namespace dlgcpp;

Position::Position(int x, int y, int w, int h) :
    Point(x,y), Size(w,h)
{
}

Position::Position(const Position& other) :
    Point(other), Size(other)
{
}

Position::Position(const Point& other) :
    Point(other)
{
}

Position::Position(const Size& other) :
    Size(other)
{
}

Position& Position::operator=(const Position& other)
{
    x(other.x());
    y(other.y());
    width(other.width());
    height(other.height());
    return *this;
}

bool Position::operator==(const Position& other) const
{
    return point() == other.point() &&
           size() == other.size();
}

bool Position::operator!=(const Position& other) const
{
    return !(*this == other);
}
