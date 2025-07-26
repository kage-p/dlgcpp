#pragma once

namespace dlgcpp
{
    class Point
    {
    public:

        inline Point(int x = 0, int y = 0) :
            _x(x), _y(y)
        {
        }

        inline Point(const Point& other) :
            _x(other._x), _y(other._y)
        {
        }

        inline Point& operator=(const Point& other)
        {
            _x = other._x;
            _y = other._y;
            return *this;
        }

        inline bool operator==(const Point& other) const
        {
            return
                _x == other._x &&
                _y == other._y;
        }

        inline bool operator!=(const Point& other) const
        {
            return !(*this == other);
        }

        inline int x() const
        {
            return _x;
        }

        inline void x(int value)
        {
            _x = value;
        }

        inline int y() const
        {
            return _y;
        }

        inline void y(int value)
        {
            _y = value;
        }

    private:
        int _x = 0;
        int _y = 0;
    };
}