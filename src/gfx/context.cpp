#include "dlgcpp/gfx/context.h"
#include "gfx/context_p.h"

using namespace dlgcpp;
using namespace dlgcpp::gfx;

DrawingContext::DrawingContext(std::shared_ptr<DrawingContextImpl> impl)
    : _impl(impl)
{
}

bool DrawingContext::handled() const
{
    return _impl->handled();
}

void DrawingContext::handled(bool value)
{
    _impl->handled(value);
}

void DrawingContext::render()
{
    _impl->render();
}

Size DrawingContext::canvasSize() const
{
    return _impl->canvasSize();
}

void DrawingContext::fill(const Rect& r, const Color fill)
{
    _impl->fill(r, fill);
}

void DrawingContext::drawLine(
    Point p1,
    Point p2,
    Color stroke,
    int thickness)
{
    _impl->drawLine(p1, p2, stroke, thickness);
}

void DrawingContext::drawRect(
    const Rect& r,
    const Color stroke,
    int thickness,
    Color fill)
{
    _impl->drawRect(r, stroke, thickness, fill);
}

void DrawingContext::drawEllipse(
    const Rect& r,
    const Color stroke,
    int thickness,
    Color fill)
{
    _impl->drawEllipse(r, stroke, thickness, fill);
}

void DrawingContext::drawText(
    const std::string& text,
    const Font& font,
    const Color color,
    Point anchor,
    HorizontalAlign horzAlign,
    VerticalAlign vertAlign)
{
    _impl->drawText(text, font, color, anchor, horzAlign, vertAlign);
}

void DrawingContext::drawImage(
    const ImageSource& image,
    const Rect& dest,
    double rotationDeg,
    Color bgColor)
{
    drawImage(image, dest, rotationDeg, bgColor);
}
