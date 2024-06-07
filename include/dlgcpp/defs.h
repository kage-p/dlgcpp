#pragma once

#include <string>

namespace dlgcpp
{
    // forwards
    class IDialog;
    class IMenu;
    class IControl;
    struct dlg_message;

    typedef struct Position
    {
        int _x = 0;
        int _y = 0;
        int _cx = 0;
        int _cy = 0;
    } Position;

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

    enum class BorderStyle
    {
        None = 0,
        Thin,
        Sunken,
        Raised
    };

    struct ImageSource
    {
        std::string id;
        // image is an icon, else a bitmap
        bool isIcon = false;
        // id is a file, else a resource
        bool isFile = false;
    };
}
