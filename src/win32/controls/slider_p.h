#pragma once

#include "control_p.h"
#include "dlgcpp/controls/slider.h"
#include "utility/event.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        class SliderImpl : public ControlImpl
        {
        public:
            explicit SliderImpl(
                const std::string& text = std::string(),
                const Position& p = Position());

            ~SliderImpl() override;

            bool vertical() const;
            void vertical(bool value);
            int value() const;
            void value(int value);
            std::pair<int, int> range() const;
            void range(int from, int to);
            void colors(Color fgColor, Color bgColor) override;
            Color barColor() const;
            void barColor(Color value);

            IEvent<ISharedControl>& ChangedEvent();

        private:
            bool _vertical = false;
            int _value = 0;
            std::pair<int, int> _range = { 0,100 };
            Color _barColor = Color::Default;
            Event<ISharedControl> _changedEvent;

            static constexpr const char* SliderControlClass = "SliderControl32";

            static constexpr UINT SLM_FIRST = WM_USER + 400;

            // SLM_GETRANGE: If wParam is nonzero then return min range, else return max range.
            static constexpr UINT SLM_GETRANGE = SLM_FIRST + 0;

            // SLM_SETRANGE: wParam is min value (typecasted) / lParam is max value
            static constexpr UINT SLM_SETRANGE = SLM_FIRST + 1;

            // SLM_GETPOS: Return current position.
            static constexpr UINT SLM_GETPOS = SLM_FIRST + 2;

            // SLM_SETPOS: Set position. wParam is position. lParam is notify message
            static constexpr UINT SLM_SETPOS = SLM_FIRST + 3;

            // SLM_SETDIRECTION: Sets direction, lParam is one of the SLS_ values
            static constexpr UINT SLM_SETDIRECTION = SLM_FIRST + 4;

            // SLM_SETLOCK: Lock the control if lParam is nonzero (dont allow user interaction)
            static constexpr UINT SLM_SETLOCK = SLM_FIRST + 5;

            // SLM_SETBKCOLOR: lParam is COLORREF of the back color
            static constexpr UINT SLM_SETBKCOLOR = SLM_FIRST + 6;

            // SLM_SETTEXTCOLOR: lParam is COLORREF of the text color
            static constexpr UINT SLM_SETTEXTCOLOR = SLM_FIRST + 7;

            // SLM_SETBARCOLOR: lParam is COLORREF of the bar color
            static constexpr UINT SLM_SETBARCOLOR = SLM_FIRST + 8;

            // lParam nonzero to display percentage value instead of text
            static constexpr UINT SLM_USEPERCENT = SLM_FIRST + 9;

            // wParam unused. lParam background image handle. Returns old image, if any.
            static constexpr UINT SLM_SETIMAGE = SLM_FIRST + 10;

            // SLM_GETIMAGE: Returns background image handle, if any.
            static constexpr UINT SLM_GETIMAGE = SLM_FIRST + 11;

            // SLS_HORIZONTAL: Horizontal orientation
            static constexpr UINT SLS_HORIZONTAL = 0;

            // SLS_VERTICAL: Vertical orientation
            static constexpr UINT SLS_VERTICAL = 1;

            // SLS_LOCKON: Require focus before change is made.
            // TODO: implement
            static constexpr UINT SLS_LOCKON = 0x0000004;

            void rebuild() override;
            std::string className() const override;
            void notify(DialogMessage&) override;

            void updateDisplayStyles();
            bool sldRegister();
            bool sldUnregister();
            static void sldRedraw(struct TSLDR_DATA* psd);
            static void sldMoveIncrement(TSLDR_DATA* psd, bool redraw);
            static void sldMoveWithMouse(TSLDR_DATA* psd);

            static LRESULT WINAPI SliderWndProc(HWND hwnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
        };
    }
}
