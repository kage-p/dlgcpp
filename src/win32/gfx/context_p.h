#pragma once

#include "dlgcpp/gfx/context.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <objbase.h>
#include <Gdiplus.h>

namespace dlgcpp
{
    namespace gfx
    {
        class DrawingContextImpl
        {
        public:
            DrawingContextImpl(HWND hwnd, HDC hdc);
            virtual ~DrawingContextImpl();

            bool handled() const;
            void handled(bool value);
            Size canvasSize() const;
            void render();

            void fill(
                const Rect& r,
                Color fill);

            void drawLine(
                Point p1,
                Point p2,
                Color stroke,
                int thickness = 1);

            void drawRect(
                const Rect& r,
                const Color stroke,
                int thickness,
                Color fill = Color::None);

            void drawEllipse(
                const Rect& r,
                const Color stroke,
                int thickness,
                Color fill = Color::None);

            void drawText(
                const std::string& text,
                const Font& font,
                const Color color,
                Point anchor,
                HorizontalAlign horzAlign = HorizontalAlign::Left,
                VerticalAlign vertAlign = VerticalAlign::Top);

            void drawImage(
                const ImageSource& image,
                const Rect& dest,
                double rotationDeg = 0.0,
                Color bgColor = Color::None);

        private:
            HWND m_hwnd = nullptr;
            HDC m_hdcScreen = nullptr;
            HDC m_hdcMem = nullptr;
            HBITMAP m_hbmpBack = nullptr;
            HBITMAP m_hbmpOld = nullptr;
            std::unique_ptr<Gdiplus::Graphics> m_Graphics;
            bool _handled = false;
            Size m_size;
        };
    }
}
