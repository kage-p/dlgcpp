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
