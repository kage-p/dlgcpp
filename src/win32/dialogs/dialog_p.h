#pragma once
#include "dlgcpp/dialogs/dialog.h"
#include "utility/event.h"
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
            explicit DialogImpl(
                Dialog& dialog,
                DialogType type = DialogType::Application,
                ISharedDialog parent = nullptr);
            virtual ~DialogImpl();
            int exec();

            bool enabled() const;
            void enabled(bool value);
            bool visible() const;
            void visible(bool value);
            DisplayState displayState() const;
            void displayState(DisplayState value);
            const Position& p() const;
            void p(const Position& p);
            DialogType type() const;
            bool showHelp() const;
            void showHelp(bool value);
            Color color() const;
            void color(Color value);
            Cursor cursor() const;
            void cursor(Cursor value);
            bool dropTarget() const;
            void dropTarget(bool value);
            bool mouseCapture() const;
            void mouseCapture(bool value);
            const std::string& title() const;
            void title(const std::string& value);
            const ImageSource& image() const;
            void image(const ImageSource& image);
            virtual std::shared_ptr<dlgcpp::menus::Menu> menu() const;
            virtual void menu(std::shared_ptr<dlgcpp::menus::Menu> menu);
            void* handle() const;
            void* user() const;
            void user(void* value);

            // actions
            void show();
            void close(int result = 0);
            void setFocus();
            void bringToFront();
            void sendToBack();
            void move(const Point& point);
            void resize(const Size& size);
            void center();
            void message(const std::string& message, const std::string& title = std::string());
            void sendUserEvent(int param = 0);
            void redraw(bool drawChildren = false);
            void timer(int timeout);

            // child management
            void add(std::shared_ptr<dlgcpp::controls::Control> child);
            void remove(std::shared_ptr<dlgcpp::controls::Control> child);
            std::vector<std::shared_ptr<dlgcpp::controls::Control>> controls() const;
            void add(std::shared_ptr<Dialog> child);
            void remove(std::shared_ptr<Dialog> child);
            std::vector<std::shared_ptr<Dialog>> dialogs() const;

            // utility
            Point toPixels(const Point& point) const;
            Size toPixels(const Size& size) const;
            Position toPixels(const Position& pos) const;

            // events
            IEvent<ISharedDialog>& ConfirmEvent();
            IEvent<ISharedDialog>& CancelEvent();
            IEvent<ISharedDialog>& HelpEvent();
            IEvent<ISharedDialog>& MoveEvent();
            IEvent<ISharedDialog>& SizeEvent();
            IEvent<ISharedDialog, ISharedDrawingContext>& PaintEvent();
            IEvent<ISharedDialog>& TimerEvent();
            IEvent<ISharedDialog, std::vector<std::string>>& DropEvent();
            IEvent<ISharedDialog, KeyboardEvent>& KeyDownEvent();
            IEvent<ISharedDialog, KeyboardEvent>& KeyUpEvent();
            IEvent<ISharedDialog, MouseEvent>& MouseDownEvent();
            IEvent<ISharedDialog, MouseEvent>& MouseUpEvent();
            IEvent<ISharedDialog, MouseEvent>& MouseMoveEvent();
            IEvent<ISharedDialog, MouseEvent>& MouseDoubleClickEvent();
            IEvent<ISharedDialog>& MouseCaptureLostEvent();
            IEvent<ISharedDialog, int>& UserEvent();

            // overridable by derived class
            virtual unsigned int styles() const;
            virtual unsigned int exStyles() const;

            ISharedDialog parent() const;
            void parent(ISharedDialog);
            int id() const;
            void id(int value);
            int idRange() const;
            void notify(DialogMessage&);
            void rebuild();

        private:
            Dialog& _dialog;
            ISharedDialog _parent;
            DialogType _type = DialogType::Application;
            int _id = 0;
            bool _enabled = true;
            bool _visible = false;
            DisplayState _displayState = DisplayState::Normal;
            Position _p = Position(0, 0, 600, 400);
            bool _dropTarget = false;
            bool _showHelp = false;
            std::string _title;
            ImageSource _image;
            std::shared_ptr<dlgcpp::menus::Menu> _menu;
            Color _backColor = Color::Default;
            Cursor _cursor = Cursor::Default;
            std::vector<std::shared_ptr<dlgcpp::controls::Control>> _controls;
            std::vector<std::shared_ptr<Dialog>> _dialogs;
            int _nextId = ChildStartId;
            bool _execRunning = false;
            struct
            {
                int id = 0;
                int timeout = 0;
            } _timer;
            void* _user = nullptr;

            // state properties
            HWND _hwnd = NULL;
            HBRUSH _hbrBgColor = NULL;
            HGDIOBJ _hImage = NULL;
            bool _execParentEnabled = false;

            // event inhibitors
            MessageLockValue _inhibitSizeAndMoveMessages;

            // events
            Event<ISharedDialog> _confirmEvent;
            Event<ISharedDialog> _cancelEvent;
            Event<ISharedDialog, KeyboardEvent> _keyDownEvent;
            Event<ISharedDialog, KeyboardEvent> _keyUpEvent;
            Event<ISharedDialog, MouseEvent> _mouseDownEvent;
            Event<ISharedDialog, MouseEvent> _mouseUpEvent;
            Event<ISharedDialog, MouseEvent> _mouseMoveEvent;
            Event<ISharedDialog, MouseEvent> _mouseDblClickEvent;
            Event<ISharedDialog> _mouseCaptureLostEvent;
            Event<ISharedDialog, std::vector<std::string>> _dropEvent;
            Event<ISharedDialog> _focusEvent;
            Event<ISharedDialog> _helpEvent;
            Event<ISharedDialog> _moveEvent;
            Event<ISharedDialog> _sizeEvent;
            Event<ISharedDialog, ISharedDrawingContext> _paintEvent;
            Event<ISharedDialog> _timerEvent;
            Event<ISharedDialog, int> _userEvent;

            int nextId(int reservedRange = 1);
            void updateDisplayState();
            void updateIcon();
            void updatePosition();
            void updateTimer();
            void updateVisibility();
            void onSetCursor(DialogMessage& msg);
            void onColorDlg(DialogMessage& msg);
            void onColorCtl(DialogMessage& msg);
            void destruct();
            void quit(int result = 0);

            std::shared_ptr<dlgcpp::controls::Control> findControl(int id, HWND hwnd = NULL);
            static LRESULT CALLBACK dialogWndProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);

            static constexpr int ChildStartId = 100;
            static constexpr int MenuStartId = 10000;
            static constexpr int WM_DLGCPP_USER = WM_USER + 1000;
        };
    }
}
