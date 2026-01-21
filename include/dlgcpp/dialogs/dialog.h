#pragma once

#include "dlgcpp/controls/control.h"
#include "dlgcpp/defs.h"
#include "dlgcpp/event.h"
#include "dlgcpp/menus/menu.h"

#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace dlgcpp
{
    namespace dialogs
    {
        /// <summary>
        /// Dialog type
        /// </summary>
        enum class DialogType
        {
            Application,
            Popup,
            Frameless,
            Tool
        };

        inline std::ostream& operator<<(std::ostream& os, DialogType e)
        {
            switch (e) {
            case DialogType::Application:   return os << "Application";
            case DialogType::Popup:  return os << "Popup";
            case DialogType::Frameless: return os << "Frameless";
            case DialogType::Tool:  return os << "Tool";
            }
            return os << static_cast<int>(e); // fallback
        }

        /// <summary>
        /// Dialog display state when visible
        /// </summary>
        enum class DisplayState
        {
            Normal = 0,
            Minimized,
            Maximized
        };

        inline std::ostream& operator<<(std::ostream& os, DisplayState e)
        {
            switch (e) {
            case DisplayState::Normal:   return os << "Normal";
            case DisplayState::Minimized:  return os << "Minimized";
            case DisplayState::Maximized: return os << "Maximized";
            }
            return os << static_cast<int>(e); // fallback
        }

        struct CloseDialogEvent
        {
            bool cancel = false;

            friend std::ostream& operator<<(std::ostream& os, const CloseDialogEvent& obj)
            {
                return os <<
                    "{ cancel: " << obj.cancel << " }";
            }
        };

        class IDialog
        {
        public:
            // properties
            virtual IProperty<bool, ISharedDialog>& enabled() = 0;
            virtual IProperty<bool, ISharedDialog>& visible() = 0;
            virtual IProperty<std::string, ISharedDialog>& title() = 0;
            virtual IProperty<ImageSource, ISharedDialog>& image() = 0;
            virtual IProperty<DisplayState, ISharedDialog>& display() = 0;
            virtual IProperty<Position, ISharedDialog>& p() = 0;
            virtual IProperty<DialogType, ISharedDialog>& type() = 0;
            virtual IProperty<Color, ISharedDialog>& color() = 0;
            virtual IProperty<Cursor, ISharedDialog>& cursor() = 0;
            virtual IProperty<std::shared_ptr<dlgcpp::menus::Menu>, ISharedDialog>& menu() = 0;
            virtual IProperty<bool, ISharedDialog>& active() = 0;
            virtual IProperty<bool, ISharedDialog>& dropTarget() = 0;
            virtual IProperty<bool, ISharedDialog>& mouseCapture() = 0;
            virtual IProperty<bool, ISharedDialog>& showHelp() = 0;
            virtual IReadOnlyProperty<void*, ISharedDialog>& handle() = 0;
            virtual IProperty<void*, ISharedDialog>& user() = 0;

            // actions
            virtual int exec() = 0;
            virtual void show() = 0;
            virtual void close(int result = 0) = 0;
            virtual void redraw(bool drawChildren = false) = 0;
            virtual void bringToFront() = 0;
            virtual void sendToBack() = 0;
            virtual void move(const Point& point) = 0;
            virtual void resize(const Size& size) = 0;
            virtual void center() = 0;
            virtual void message(const std::string& message, const std::string& title = std::string()) = 0;
            virtual void sendUserEvent(int param = 0) = 0;
            virtual void timer(int timeout) = 0;

            // parent/child management
            virtual IWeakDialog parent() const = 0;
            virtual void add(ISharedControl control) = 0;
            virtual void remove(ISharedControl control) = 0;
            virtual std::vector<ISharedControl> controls() const = 0;
            virtual void add(ISharedDialog dialog) = 0;
            virtual void remove(ISharedDialog dialog) = 0;
            virtual std::vector<ISharedDialog> dialogs() const = 0;

            // events
            virtual IEvent<ISharedDialog>& ConfirmEvent() = 0;
            virtual IEvent<ISharedDialog>& CancelEvent() = 0;
            virtual IEvent<ISharedDialog, CloseDialogEvent>& CloseEvent() = 0;
            virtual IEvent<ISharedDialog>& CreateEvent() = 0;
            virtual IEvent<ISharedDialog>& DestroyEvent() = 0;
            virtual IEvent<ISharedDialog>& HelpEvent() = 0;
            virtual IEvent<ISharedDialog>& MoveEvent() = 0;
            virtual IEvent<ISharedDialog>& SizeEvent() = 0;
            virtual IEvent<ISharedDialog, ISharedDrawingContext>& PaintEvent() = 0;
            virtual IEvent<ISharedDialog>& TimerEvent() = 0;
            virtual IEvent<ISharedDialog, KeyboardEvent>& KeyDownEvent() = 0;
            virtual IEvent<ISharedDialog, KeyboardEvent>& KeyUpEvent() = 0;
            virtual IEvent<ISharedDialog, MouseEvent>& MouseDownEvent() = 0;
            virtual IEvent<ISharedDialog, MouseEvent>& MouseUpEvent() = 0;
            virtual IEvent<ISharedDialog, MouseEvent>& MouseMoveEvent() = 0;
            virtual IEvent<ISharedDialog, MouseEvent>& MouseDoubleClickEvent() = 0;
            virtual IEvent<ISharedDialog>& MouseCaptureLostEvent() = 0;
            virtual IEvent<ISharedDialog, DropFilesEvent>& DropEvent() = 0;
            virtual IEvent<ISharedDialog, int>& UserEvent() = 0;

            // internal
            virtual std::shared_ptr<DialogImpl> impl() = 0;
        };

        class Dialog :
            public IDialog,
            public std::enable_shared_from_this<Dialog>
        {
        public:
            explicit Dialog(
                DialogType type = DialogType::Application,
                ISharedDialog parent = nullptr);

            virtual ~Dialog();

            // compatibility setters
            void enabled(bool value);
            void visible(bool value);
            void title(const std::string& value);
            void image(const ImageSource& image);
            void display(DisplayState value);
            void p(const Position& p);
            void color(Color bgColor);
            void cursor(Cursor value);
            void menu(std::shared_ptr<dlgcpp::menus::Menu> value);
            void active(bool value);
            void dropTarget(bool value);
            void mouseCapture(bool value);
            void showHelp(bool value);
            void user(void* value);

            // utility
            Point toPixels(const Point& point) const;
            Size toPixels(const Size& size) const;
            Position toPixels(const Position& pos) const;

            // IDialog impl.

            // properties
            IProperty<bool, ISharedDialog>& enabled() override;
            IProperty<bool, ISharedDialog>& visible() override;
            IProperty<std::string, ISharedDialog>& title() override;
            IProperty<ImageSource, ISharedDialog>& image() override;
            IProperty<DisplayState, ISharedDialog>& display() override;
            IProperty<Position, ISharedDialog>& p() override;
            IProperty<DialogType, ISharedDialog>& type() override;
            IProperty<Color, ISharedDialog>& color() override;
            IProperty<Cursor, ISharedDialog>& cursor() override;
            IProperty<std::shared_ptr<dlgcpp::menus::Menu>, ISharedDialog>& menu() override;
            IProperty<bool, ISharedDialog>& active() override;
            IProperty<bool, ISharedDialog>& dropTarget() override;
            IProperty<bool, ISharedDialog>& mouseCapture() override;
            IProperty<bool, ISharedDialog>& showHelp() override;
            IReadOnlyProperty<void*, ISharedDialog>& handle() override;
            IProperty<void*, ISharedDialog>& user() override;

            // actions
            int exec() override;
            void show() override;
            void close(int result = 0) override;
            void bringToFront() override;
            void sendToBack() override;
            void move(const Point& point) override;
            void resize(const Size& size) override;
            void center() override;
            void message(const std::string& message, const std::string& title = std::string()) override;
            void sendUserEvent(int param = 0) override;
            void redraw(bool drawChildren = false) override;
            void timer(int timeout) override;

            // parent/child management
            IWeakDialog parent() const override;
            void add(ISharedControl control) override;
            void remove(ISharedControl control) override;
            std::vector<ISharedControl> controls() const override;
            void add(ISharedDialog dialog) override;
            void remove(ISharedDialog dialog) override;
            std::vector<ISharedDialog> dialogs() const override;

            // events
            IEvent<ISharedDialog>& ConfirmEvent() override;
            IEvent<ISharedDialog>& CancelEvent() override;
            IEvent<ISharedDialog, CloseDialogEvent>& CloseEvent() override;
            IEvent<ISharedDialog>& CreateEvent() override;
            IEvent<ISharedDialog>& DestroyEvent() override;
            IEvent<ISharedDialog>& HelpEvent() override;
            IEvent<ISharedDialog>& MoveEvent() override;
            IEvent<ISharedDialog>& SizeEvent() override;
            IEvent<ISharedDialog, ISharedDrawingContext>& PaintEvent() override;
            IEvent<ISharedDialog>& TimerEvent() override;
            IEvent<ISharedDialog, DropFilesEvent>& DropEvent() override;
            IEvent<ISharedDialog, KeyboardEvent>& KeyDownEvent() override;
            IEvent<ISharedDialog, KeyboardEvent>& KeyUpEvent() override;
            IEvent<ISharedDialog, MouseEvent>& MouseDownEvent() override;
            IEvent<ISharedDialog, MouseEvent>& MouseUpEvent() override;
            IEvent<ISharedDialog, MouseEvent>& MouseMoveEvent() override;
            IEvent<ISharedDialog, MouseEvent>& MouseDoubleClickEvent() override;
            IEvent<ISharedDialog>& MouseCaptureLostEvent() override;
            IEvent<ISharedDialog, int>& UserEvent() override;

            // internal
            std::shared_ptr<DialogImpl> impl() override;

        protected:
            Dialog(
                std::shared_ptr<DialogImpl> impl,
                DialogType type,
                ISharedDialog parent);

        private:
            std::shared_ptr<DialogImpl> _impl;

            // properties
            IWeakDialog _parent;
            std::vector<ISharedControl> _controls;
            std::vector<ISharedDialog> _dialogs;
            Property<bool, ISharedDialog> _enabled;
            Property<bool, ISharedDialog> _visible;
            Property<DialogType, ISharedDialog> _type;
            Property<std::string, ISharedDialog> _title;
            Property<ImageSource, ISharedDialog> _image;
            Property<DisplayState, ISharedDialog> _display;
            Property<Position, ISharedDialog> _p;
            Property<Color, ISharedDialog> _color;
            Property<Cursor, ISharedDialog> _cursor;
            Property<std::shared_ptr<dlgcpp::menus::Menu>, ISharedDialog> _menu;
            Property<bool, ISharedDialog> _active;
            Property<bool, ISharedDialog> _dropTarget;
            Property<bool, ISharedDialog> _mouseCapture;
            Property<bool, ISharedDialog> _showHelp;
            Property<void*, ISharedDialog> _handle;
            Property<void*, ISharedDialog> _user;

            // events
            Event<ISharedDialog> _confirmEvent;
            Event<ISharedDialog> _cancelEvent;
            Event<ISharedDialog, CloseDialogEvent> _closeEvent;
            Event<ISharedDialog> _createEvent;
            Event<ISharedDialog> _destroyEvent;
            Event<ISharedDialog, KeyboardEvent> _keyDownEvent;
            Event<ISharedDialog, KeyboardEvent> _keyUpEvent;
            Event<ISharedDialog, MouseEvent> _mouseDownEvent;
            Event<ISharedDialog, MouseEvent> _mouseUpEvent;
            Event<ISharedDialog, MouseEvent> _mouseMoveEvent;
            Event<ISharedDialog, MouseEvent> _mouseDblClickEvent;
            Event<ISharedDialog> _mouseCaptureLostEvent;
            Event<ISharedDialog, DropFilesEvent> _dropEvent;
            Event<ISharedDialog> _focusEvent;
            Event<ISharedDialog> _helpEvent;
            Event<ISharedDialog> _moveEvent;
            Event<ISharedDialog> _sizeEvent;
            Event<ISharedDialog, ISharedDrawingContext> _paintEvent;
            Event<ISharedDialog> _timerEvent;
            Event<ISharedDialog, int> _userEvent;
        };
    }
}
