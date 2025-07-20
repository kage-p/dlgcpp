#pragma once

namespace dlgcpp
{
    class Size
    {
    public:

        inline Size(int width = 0, int height = 0) :
            _width(width), _height(height)
        {
        }

        inline Size(const Size& other) :
            _width(other._width), _height(other._height)
        {
        }

        inline Size& operator=(const Size& other)
        {
            _width = other._width;
            _height = other._height;
            return *this;
        }

        inline bool operator==(const Size& other) const
        {
            return
                _width == other._width &&
                _height == other._height;
        }

        inline bool operator!=(const Size& other) const
        {
            return !(*this == other);
        }

        inline int width() const
        {
            return _width;
        }

        inline void width(int value)
        {
            _width = value;
        }

        inline int height() const
        {
            return _height;
        }

        inline void height(int value)
        {
            _height = value;
        }

        inline bool empty() const
        {
            return (_width * _height) < 1;
        }

    private:
        int _width = 0;
        int _height = 0;
    };
}