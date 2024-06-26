#pragma once

#include <memory>
#include <string>

#ifdef DLGCPP_DEBUG
#include <iostream>
#define DLGCPP_CMSG(m_args) std::cout << m_args << std::endl
#define DLGCPP_CERR(m_args) std::cerr << m_args << std::endl
#else
#define DLGCPP_CMSG(m_args)
#define DLGCPP_CERR(m_args)
#endif

namespace dlgcpp
{
    // forwards
    class IDialog;
    class IMenu;
    class IControl;
    struct dlg_message;

    typedef std::shared_ptr<IDialog> ISharedDialog;
    typedef std::shared_ptr<IControl> ISharedControl;

    class Point
    {
    public:
        Point(int x = 0, int y = 0);
        Point(const Point&);
        Point& operator=(const Point& other);
        bool operator==(const Point& other) const;
        bool operator!=(const Point& other) const;

        int x() const;
        void x(int value);
        int y() const;
        void y(int value);

    private:
        int _x = 0;
        int _y = 0;
    };

    class Size
    {
    public:
        Size(int w = 0, int h = 0);
        Size(const Size&);
        Size& operator=(const Size& other);
        bool operator==(const Size& other) const;
        bool operator!=(const Size& other) const;

        int width() const;
        void width(int value);
        int height() const;
        void height(int value);
        bool empty() const;

    private:
        int _width = 0;
        int _height = 0;
    };

    class Position : public Point, public Size
    {
    public:
        Position(int x = 0, int y = 0, int w = 0, int h = 0);
        Position(const Position&);
        Position(const Point&);
        Position(const Size&);
        Position& operator=(const Position& other);
        bool operator==(const Position& other) const;
        bool operator!=(const Position& other) const;

        const Point& point() const { return *this; }
        const Size& size() const { return *this; }
    };

    enum class HorizontalAlign
    {
        Left = 0,
        Center,
        Right
    };

    enum class VerticalAlign
    {
        Top = 0,
        Center,
        Bottom
    };

    enum class BorderStyle
    {
        None = 0,
        Thin,
        Sunken,
        Raised
    };

    enum class Color : unsigned long
    {
        // not specified.
        None = 0xFFFFFFFFL,
        // default color
        Default = 0xFF000000L,

        Black = 0x0,
        White = 0xFFFFFF,
        Gray = 0x808080,
        LtGray = 0xC0C0C0,
        DarkGray = 0xA9A9A9,
        Red = 0x0000FF,
        Green = 0x00FF00,
        Blue = 0xFF0000,
        Cyan = 0xFFFF00,
        Magenta = 0xFF00FF,
        Yellow = 0x00FFFF,
        DarkRed = 0x0000C4,
        DarkGreen = 0x00C400,
        DarkBlue = 0xC40000,
        MedBlue = 0xFF4040,
        Aqua = 0xFFFF00,
        Fuchsia = 0xFF00FF,
        Lime = 0x00FF00,
        Maroon = 0x000080,
        Navy = 0x800000,
        Olive = 0x008080,
        Orange = 0x00A5FF,
        Purple = 0x800080,
        Silver = 0xC0C0C0,
        Teal = 0x808000,
        Brown = 0x2A2AA5
    };

    #define MakeColor(r,g,b) (dlgcpp::Color) (((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));

    typedef struct Font
    {
        std::string faceName;
        int pointSize = 8;
        bool bold = false;
        bool underline = false;
        bool symbolType = false;
    } Font;

    enum class Cursor
    {
        Default = 0,
        Arrow,
        ArrowBusy,
        Busy,
        Cross,
        Hand,
        Help,
        Text,
        Unavailable
    };

    struct ImageSource
    {
        // location of a file or resource
        std::string id;
        // image is an icon, else a bitmap
        bool isIcon = false;
        // id is a file, else a resource
        bool isFile = false;

        inline bool operator==(const ImageSource& other) const
        {
            return (id == other.id &&
                    isIcon == other.isIcon &&
                    isFile == other.isFile);
        }
        inline bool operator!=(const ImageSource& other) const
        {
            return !(*this == other);
        }
    };

    enum class MouseButton
    {
        Left = 0,
        Right,
        Middle
    };

    struct MouseEvent
    {
        MouseButton button = MouseButton::Left;
        Point point = Point(0,0);
    };
}
