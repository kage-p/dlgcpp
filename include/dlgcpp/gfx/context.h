#pragma once

#include "dlgcpp/defs.h"
#include <string>

namespace dlgcpp
{
    class IDrawingContext
    {
    public:
        virtual ~IDrawingContext() = default;

        virtual bool handled() const = 0;
        virtual void handled(bool value) = 0;

        virtual Size canvasSize() const = 0;

        // Shapes

        virtual void fill(
            const Rect& r,
            const Color fill) = 0;

        virtual void drawLine(
            Point p1,
            Point p2,
            Color stroke,
            int thickness = 1) = 0;

        virtual void drawRect(
            const Rect& r,
            const Color stroke,
            int thickness,
            Color fill = Color::None) = 0;

        virtual void drawEllipse(
            const Rect& r,
            const Color stroke,
            int thickness,
            Color fill = Color::None) = 0;

        // Text

        virtual void drawText(
            const std::string& text,
            const Font& font,
            const Color color,
            Point anchor,
            HorizontalAlign horzAlign = HorizontalAlign::Left,
            VerticalAlign vertAlign = VerticalAlign::Top) = 0;

        // Images

        virtual void drawImage(
            const ImageSource& image,
            const Rect& dest,
            double rotationDeg = 0.0,
            Color bgColor = Color::None) = 0;
    };
}
