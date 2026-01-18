#pragma once
#include "dlgcpp/dialogs/dialog.h"
#include "utility/message.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace dialogs
    {
        class DialogImpl
        {
        public:
            explicit DialogImpl();
            virtual ~DialogImpl();

            // set hard link to owner dialog
            // not ideal but we can't use a shared_ptr or pollute the interface
            void owner(IDialog* dialog);

            int exec();

            // actions
            void show();
            void close(int result = 0);
            void bringToFront();
            void sendToBack();
            void center();
            void message(const std::string& message, const std::string& title = std::string());
            void sendUserEvent(int param = 0);
            void redraw(bool drawChildren = false);
            void timer(int timeout);

            // child management
            void add(ISharedControl control);
            void remove(ISharedControl control);
            void add(ISharedDialog dialog);
            void remove(ISharedDialog dialog);

            // utility
            Point toPixels(const Point& point) const;
            Size toPixels(const Size& size) const;
            Position toPixels(const Position& pos) const;

            // overridable by derived class
            virtual void build();
            virtual unsigned int styles() const;
            virtual unsigned int exStyles() const;

            IReadOnlyProperty<void*, int>& handle();
            HWND hwnd() const;
            int id() const;
            void id(int value);
            int idRange() const;
            void notify(DialogMessage&);

        protected:
            void rebuild();

        private:
            IDialog* _dialog = nullptr;
            Property<void*, int> _handle;

            // internals
            HWND _hwnd = NULL;
            HBRUSH _hbrBgColor = NULL;
            HGDIOBJ _hImage = NULL;
            int _id = 0;
            bool _execParentEnabled = false;
            int _nextId = ChildStartId;
            bool _execRunning = false;
            struct
            {
                int id = 0;
                int timeout = 0;
            } _timer;

            // event inhibitors
            MessageLockValue _inhibitSizeAndMoveMessages;

            int nextId(int reservedRange = 1);
            void updateDisplayState();
            void updateIcon();
            void updatePosition();
            void updateTimer();
            void updateVisibility();
            void storePosition();
            void handleSetCursor(DialogMessage& msg);
            void handleCtlColorDlg(DialogMessage& msg);
            void handleCtlColor(DialogMessage& msg);
            void destruct();
            void quit(int result = 0);

            void onEnabledChanged();
            void onVisibleChanged();
            void onDisplayChanged();
            void onPosChanged();
            void onShowHelpChanged();
            void onColorChanged();
            void onActiveChanged();
            void onDropTargetChanged();
            void onMouseCaptureChanged();
            void onTitleChanged();
            void onImageChanged();
            void onMenuChanged();

            ISharedControl findControl(int id, HWND hwnd = NULL);
            static LRESULT CALLBACK dialogWndProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);

            static constexpr int ChildStartId = 100;
            static constexpr int MenuStartId = 10000;
            static constexpr int WM_DLGCPP_USER = WM_USER + 1000;
        };
    }
}
