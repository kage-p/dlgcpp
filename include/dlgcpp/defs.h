#pragma once

#include <memory>

#ifdef DLGCPP_DEBUG
#include <iostream>
#define DLGCPP_CMSG(m_args) std::cout << __func__ << ": " << m_args << std::endl
#define DLGCPP_CERR(m_args) std::cerr << __func__ << ": " << m_args << std::endl
#else
#define DLGCPP_CMSG(m_args)
#define DLGCPP_CERR(m_args)
#endif

// forward definitions

namespace dlgcpp
{
    namespace dialogs
    {
        class IDialog;
        class Dialog;
        class DialogImpl;
    }

    namespace controls
    {
        class IControl;
        class Control;
        class ControlImpl;
    }

    namespace gfx
    {
        class IDrawingContext;
        class DrawingContext;
        class DrawingContextImpl;
    }

    namespace menus
    {
        class IMenu;
        class Menu;
        class MenuImpl;

        class IMenuItem;
        class MenuItem;
        class MenuItemImpl;
    }

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

    // shared forwards
    typedef std::shared_ptr<dlgcpp::dialogs::IDialog> ISharedDialog;
    typedef std::shared_ptr<dlgcpp::controls::IControl> ISharedControl;
    typedef std::shared_ptr<dlgcpp::gfx::IDrawingContext> ISharedDrawingContext;
    typedef std::shared_ptr<dlgcpp::menus::IMenu> ISharedMenu;
    typedef std::shared_ptr<dlgcpp::menus::IMenuItem> ISharedMenuItem;
}

#include "dlgcpp/align.h"
#include "dlgcpp/colors.h"
#include "dlgcpp/font.h"
#include "dlgcpp/image.h"
#include "dlgcpp/keys.h"
#include "dlgcpp/mouse.h"
#include "dlgcpp/point.h"
#include "dlgcpp/position.h"
#include "dlgcpp/size.h"
