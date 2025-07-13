#pragma once

#include "dlgcpp/gfx/context.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <objbase.h>
#include <gdiplus.h>

namespace dlgcpp
{
    namespace gfx
    {
        class DrawingContextGdip final : public IDrawingContext
        {
        public:
            DrawingContextGdip(HWND hwnd, HDC hdc);
            ~DrawingContextGdip() override;

            bool handled() const override;
            void handled(bool value) override;
            Size canvasSize() const override;
            void render();

            // Shapes

            void fill(
                const Rect& r,
                Color fill) override;

            void drawLine(
                Point p1,
                Point p2,
                Color stroke,
                int thickness = 1) override;

            void drawRect(
                const Rect& r,
                const Color stroke,
                int thickness,
                Color fill = Color::None) override;

            void drawEllipse(
                const Rect& r,
                const Color stroke,
                int thickness,
                Color fill = Color::None) override;

            // Text

            void drawText(
                const std::string& text,
                const Font& font,
                const Color color,
                Point anchor,
                HorizontalAlign horzAlign = HorizontalAlign::Left,
                VerticalAlign vertAlign = VerticalAlign::Top) override;

            // Images

            void drawImage(
                const ImageSource& image,
                const Rect& dest,
                double rotationDeg = 0.0,
                Color bgColor = Color::None) override;

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
