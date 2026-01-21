#pragma once

#include "point.h"
#include "size.h"
#include <ostream>

namespace dlgcpp
{
    struct Rect { int x{ 0 }, y{ 0 }, w{ 0 }, h{ 0 }; };

    class Position : public Point, public Size
    {
    public:
        inline Position() :
            Point(), Size()
        {
        }

        inline Position(int x, int y, int w, int h) :
            Point(x, y), Size(w, h)
        {
        }

        inline Position(const Position& other) :
            Point(other), Size(other)
        {
        }

        inline Position(const Point& other) :
            Point(other)
        {
        }

        inline Position(const Size& other) :
            Size(other)
        {
        }

        inline Position& operator=(const Position& other)
        {
            x(other.x());
            y(other.y());
            width(other.width());
            height(other.height());
            return *this;
        }

        inline bool operator==(const Position& other) const
        {
            return point() == other.point() &&
                size() == other.size();
        }

        inline bool operator!=(const Position& other) const
        {
            return !(*this == other);
        }

        inline const Point& point() const { return *this; }
        inline const Size& size() const { return *this; }

        friend std::ostream& operator<<(std::ostream& os, const Position& obj)
        {
            return os <<
                "{ x: " << obj.x() <<
                ", y: " << obj.y() <<
                ", width: " << obj.width() <<
                ", height: " << obj.height() <<
                " }";
        }
    };
}