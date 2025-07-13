
#include "context.h"
#include "utility/image.h"
#include "utility/string.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <objbase.h>
#include <gdiplus.h>

#include <cassert>
#include <memory>

using namespace dlgcpp;
using namespace dlgcpp::gfx;

inline COLORREF toCOLORREF(Color c) { return (COLORREF)c; }

static Gdiplus::Color toGdiColor(Color c)
{
    auto color = Gdiplus::Color();
    color.SetFromCOLORREF(toCOLORREF(c));
    return color;
}

DrawingContextGdip::DrawingContextGdip(HWND hwnd, HDC hdc)
    : m_hwnd(hwnd)
{
    static bool gdipInit = false;
    if (!gdipInit)
    {
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;
        Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
        gdipInit = true;
    }

    // acquire hwnd DC via BeginPaint
    m_hdcScreen = hdc;
    RECT rc{}; GetClientRect(hwnd, &rc);
    m_size = { rc.right - rc.left, rc.bottom - rc.top };

    // create off-screen buffer
    m_hdcMem = CreateCompatibleDC(m_hdcScreen);
    m_hbmpBack = CreateCompatibleBitmap(m_hdcScreen, m_size.width(), m_size.height());
    m_hbmpOld = static_cast<HBITMAP>(SelectObject(m_hdcMem, m_hbmpBack));

    // clear to window background colour
    COLORREF bg = GetSysColor(COLOR_WINDOW);
    Gdiplus::Graphics gTmp(m_hdcMem);
    gTmp.Clear(Gdiplus::Color(255, GetRValue(bg), GetGValue(bg), GetBValue(bg)));

    m_Graphics = std::make_unique<Gdiplus::Graphics>(m_hdcMem);
    m_Graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    m_Graphics->SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
    m_Graphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
}

DrawingContextGdip::~DrawingContextGdip()
{
    // clean-up
    SelectObject(m_hdcMem, m_hbmpOld);
    DeleteObject(m_hbmpBack);
    DeleteDC(m_hdcMem);
}

bool DrawingContextGdip::handled() const
{
    return _handled;
}

void DrawingContextGdip::handled(bool value)
{
    _handled = value;
}

void DrawingContextGdip::render()
{
    BitBlt(m_hdcScreen, 0, 0, m_size.width(), m_size.height(), m_hdcMem, 0, 0, SRCCOPY);
}

Size DrawingContextGdip::canvasSize() const
{
    return m_size;
}

void DrawingContextGdip::fill(const Rect& r, const Color fill)
{
    Gdiplus::SolidBrush brush(toGdiColor(fill));
    m_Graphics->FillRectangle(&brush, r.x, r.y, r.w, r.h);
}

void DrawingContextGdip::drawLine(
    Point p1,
    Point p2,
    Color stroke,
    int thickness)
{
    Gdiplus::Pen pen(toGdiColor(stroke), static_cast<Gdiplus::REAL>(thickness));
    m_Graphics->DrawLine(&pen, p1.x(), p1.y(), p2.x(), p2.y());
}

void DrawingContextGdip::drawRect(
    const Rect& r,
    const Color stroke,
    int thickness,
    Color fill)
{
    if (fill != Color::None)
    {
        Gdiplus::SolidBrush brush(toGdiColor(fill));
        m_Graphics->FillRectangle(&brush, r.x, r.y, r.w, r.h);
    }

    Gdiplus::Pen pen(toGdiColor(stroke), static_cast<Gdiplus::REAL>(thickness));
    m_Graphics->DrawRectangle(&pen, r.x, r.y, r.w, r.h);
}

void DrawingContextGdip::drawEllipse(
    const Rect& r,
    const Color stroke,
    int thickness,
    Color fill)
{
    if (fill != Color::None)
    {
        Gdiplus::SolidBrush brush(toGdiColor(fill));
        m_Graphics->FillEllipse(&brush, r.x, r.y, r.w, r.h);
    }

    Gdiplus::Pen pen(toGdiColor(stroke), static_cast<Gdiplus::REAL>(thickness));
    m_Graphics->DrawEllipse(&pen, r.x, r.y, r.w, r.h);
}

void DrawingContextGdip::drawText(
    const std::string& text,
    const Font& font,
    const Color color,
    Point anchor,
    HorizontalAlign horzAlign,
    VerticalAlign vertAlign)
{
    // horizontal alignment default: left
    Gdiplus::StringAlignment hAlign = Gdiplus::StringAlignmentNear;
    switch (horzAlign)
    {
    case HorizontalAlign::Right:
        hAlign = Gdiplus::StringAlignmentFar;
        break;
    case HorizontalAlign::Center:
        hAlign = Gdiplus::StringAlignmentCenter;
        break;
    }

    // vertical alignment default: top
    Gdiplus::StringAlignment vAlign = Gdiplus::StringAlignmentNear;
    switch (vertAlign)
    {
    case VerticalAlign::Bottom:
        vAlign = Gdiplus::StringAlignmentFar;
        break;
    case VerticalAlign::Center:
        vAlign = Gdiplus::StringAlignmentCenter;
        break;
    }

    Gdiplus::FontFamily family(toWide(font.family).data());
    Gdiplus::FontStyle style = Gdiplus::FontStyleRegular;
    if (font.bold) style = static_cast<Gdiplus::FontStyle>(style | Gdiplus::FontStyleBold);
    if (font.italic) style = static_cast<Gdiplus::FontStyle>(style | Gdiplus::FontStyleItalic);

    Gdiplus::Font gdiFont(&family, static_cast<Gdiplus::REAL>(font.pointSize), style, Gdiplus::UnitPoint);
    Gdiplus::SolidBrush brush(toGdiColor(color));
    std::wstring wtext = toWide(text);

    // Measure the size of the string
    Gdiplus::RectF textBounds;
    m_Graphics->MeasureString(
        wtext.c_str(),
        static_cast<INT>(wtext.length()),
        &gdiFont,
        Gdiplus::PointF(0, 0),  // dummy origin
        nullptr,
        &textBounds);

    // Compute adjusted origin based on alignment
    float x = static_cast<float>(anchor.x());
    float y = static_cast<float>(anchor.y());

    if (hAlign == Gdiplus::StringAlignmentCenter)
        x -= textBounds.Width / 2;
    else if (hAlign == Gdiplus::StringAlignmentFar)
        x -= textBounds.Width;

    if (vAlign == Gdiplus::StringAlignmentCenter)
        y -= textBounds.Height / 2;
    else if (vAlign == Gdiplus::StringAlignmentFar)
        y -= textBounds.Height;

    Gdiplus::PointF adjustedPoint(x, y);

    // Draw at the adjusted origin
    m_Graphics->DrawString(
        wtext.c_str(),
        static_cast<INT>(wtext.length()),
        &gdiFont,
        adjustedPoint,
        &brush);
}
void DrawingContextGdip::drawImage(
    const ImageSource& image,
    const Rect& dest,
    double rotationDeg,
    Color bgColor)
{
    if (image.isIcon)
        // cannot draw icons
        return;

    auto size = Size(dest.w, dest.h);

    // load image
    auto hImage = (HBITMAP)loadImage(image, size, bgColor);
    if (hImage == NULL)
        return;

    Gdiplus::Bitmap gdipImage(hImage, nullptr);

    Gdiplus::GraphicsState state = m_Graphics->Save();

    Gdiplus::REAL cx = dest.x + dest.w * 0.5f;
    Gdiplus::REAL cy = dest.y + dest.h * 0.5f;

    m_Graphics->TranslateTransform(cx, cy);
    m_Graphics->RotateTransform(static_cast<Gdiplus::REAL>(rotationDeg));
    m_Graphics->TranslateTransform(-cx, -cy);

    m_Graphics->DrawImage(&gdipImage,
        static_cast<int>(dest.x),
        static_cast<int>(dest.y),
        static_cast<int>(dest.w),
        static_cast<int>(dest.h));

    m_Graphics->Restore(state);

    DeleteObject(hImage);
}
