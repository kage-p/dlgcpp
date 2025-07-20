#pragma once

#include <memory>
#include <string>

#ifdef DLGCPP_DEBUG
#include <iostream>
#define DLGCPP_CMSG(m_args) std::cout << __func__ << ": " << m_args << std::endl
#define DLGCPP_CERR(m_args) std::cerr << __func__ << ": " << m_args << std::endl
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
    class IDrawingContext;

    class Point;
    class Size;
    class Rect;
    class Position;
    class Font;
    class ImageSource;

    enum class Color : unsigned long;
    enum class Key;
    enum class Cursor;
    enum class HorizontalAlign;
    enum class VerticalAlign;

    struct KeyboardEvent;
    struct MouseEvent;
    struct dlg_message;

    // shared forwards
    typedef std::shared_ptr<IDialog> ISharedDialog;
    typedef std::shared_ptr<IControl> ISharedControl;
    typedef std::shared_ptr<IDrawingContext> ISharedDrawingContext;
}

#include "align.h"
#include "colors.h"
#include "font.h"
#include "image.h"
#include "keys.h"
#include "mouse.h"
#include "point.h"
#include "position.h"
#include "size.h"
