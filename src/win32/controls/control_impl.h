#pragma once

#include "dlgcpp/controls/control.h"
#include "dlgcpp/gfx/context.h"
#include "utility/message.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef CreateEvent

namespace dlgcpp
{
    namespace controls
    {
        struct ControlMessage
        {
            UINT wMsg = 0;
            WPARAM wParam = 0;
            LPARAM lParam = 0;
            LRESULT result = 0;
            WNDPROC orgWndProc = NULL;
        };

        class ControlImpl
        {
        public:
            ControlImpl();
            virtual ~ControlImpl();

            // set hard link to owner control
            // not ideal but we can't use a shared_ptr or pollute the interface
            void owner(IControl* control);
            IReadOnlyProperty<void*, int>& handle();
            HWND hwnd() const;

            // directly callable
            virtual void bringToFront();
            virtual void sendToBack();
            virtual void redraw();

            // must be accessible from dialog internal and derived internal class
            int id() const;
            void id(int value);
            virtual int idRange() const;
            virtual void notify(DialogMessage&);
            virtual void build();
            virtual void destruct();
            virtual bool isHandleEqual(void* otherHandle) const;
            HBRUSH backgroundBrush() const;

        protected:
            // utility
            std::string getText(HWND hwnd) const;
            void setText(const std::string& text);

            // overridable by derived internal class
            virtual std::string className() const;
            virtual unsigned int styles() const;
            virtual unsigned int exStyles() const;
            virtual void notify(ControlMessage&);
            virtual void rebuild();
            virtual bool wantInternalEvents() const;

        private:
            IControl* _control = nullptr;
            Property<void*, int> _handle;

            // state properties
            int _id = -1;
            HWND _hwnd = 0;
            HWND _hwndParent = 0;
            HFONT _hFont = 0;
            HBRUSH _hbrBack = 0;
            WNDPROC _prevWndProc = 0;

            bool mustBeSubclassed() const;
            static LRESULT CALLBACK controlWndProc(HWND hwnd, UINT wMsg, WPARAM wParam, LPARAM lParam);

            // internal event handlers
            virtual void onEnabledChanged();
            virtual void onVisibleChanged();
            virtual void onPosChanged();
            virtual void onColorsChanged();
            virtual void onFontChanged();
            virtual void onFocusChanged();
            virtual void onMouseCaptureChanged();
        };
    }
}
