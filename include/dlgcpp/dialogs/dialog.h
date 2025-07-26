#pragma once

#include "dlgcpp/controls/control.h"
#include "dlgcpp/defs.h"
#include "dlgcpp/event.h"
#include "dlgcpp/menus/menu.h"

#include <memory>
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

        /// <summary>
        /// Dialog display state when visible
        /// </summary>
        enum class DisplayState
        {
            Normal = 0,
            Minimized,
            Maximized
        };

        class IDialog
        {
        public:
            // properties
            virtual bool enabled() const = 0;
            virtual void enabled(bool value) = 0;
            virtual bool visible() const = 0;
            virtual void visible(bool value) = 0;
            virtual DisplayState displayState() const = 0;
            virtual void displayState(DisplayState value) = 0;
            virtual const Position& p() const = 0;
            virtual void p(const Position& p) = 0;
            virtual DialogType type() const = 0;
            virtual bool showHelp() const = 0;
            virtual void showHelp(bool value) = 0;
            virtual const std::string& title() const = 0;
            virtual void title(const std::string& value) = 0;
            virtual const ImageSource& image() const = 0;
            virtual void image(const ImageSource& image) = 0;
            virtual std::shared_ptr<dlgcpp::menus::Menu> menu() const = 0;
            virtual void menu(std::shared_ptr<dlgcpp::menus::Menu> menu) = 0;
            virtual Color color() const = 0;
            virtual void color(Color value) = 0;
            virtual Cursor cursor() const = 0;
            virtual void cursor(Cursor value) = 0;
            virtual bool dropTarget() const = 0;
            virtual void dropTarget(bool value) = 0;
            virtual bool mouseCapture() const = 0;
            virtual void mouseCapture(bool value) = 0;
            virtual void* handle() const = 0;
            virtual void* user() const = 0;
            virtual void user(void* value) = 0;
            virtual ISharedDialog parent() const = 0;

            // actions
            virtual void show() = 0;
            virtual void close(int result = 0) = 0;
            virtual void redraw(bool drawChildren = false) = 0;
            virtual void setFocus() = 0;
            virtual void bringToFront() = 0;
            virtual void sendToBack() = 0;
            virtual void move(const Point& point) = 0;
            virtual void resize(const Size& size) = 0;
            virtual void center() = 0;
            virtual void message(const std::string& message, const std::string& title = std::string()) = 0;
            virtual void sendUserEvent(int param = 0) = 0;
            virtual void timer(int timeout) = 0;

            // child management
            virtual void add(std::shared_ptr<dlgcpp::controls::Control> child) = 0;
            virtual void remove(std::shared_ptr<dlgcpp::controls::Control> child) = 0;
            virtual std::vector<std::shared_ptr<dlgcpp::controls::Control>> controls() const = 0;
            virtual void add(std::shared_ptr<Dialog> child) = 0;
            virtual void remove(std::shared_ptr<Dialog> child) = 0;
            virtual std::vector<std::shared_ptr<Dialog>> dialogs() const = 0;

            // events
            virtual IEvent<ISharedDialog>& ConfirmEvent() = 0;
            virtual IEvent<ISharedDialog>& CancelEvent() = 0;
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
            virtual IEvent<ISharedDialog, std::vector<std::string>>& DropEvent() = 0;
            virtual IEvent<ISharedDialog, int>& UserEvent() = 0;
        };

        class Dialog :
            public IDialog,
            public std::enable_shared_from_this<Dialog>
        {
            friend class dlgcpp::dialogs::DialogImpl;

        public:
            explicit Dialog(
                DialogType type = DialogType::Application,
                ISharedDialog parent = nullptr);

            virtual ~Dialog() = default;
            int exec();

            // IDialog impl.
            bool enabled() const override;
            void enabled(bool value) override;
            bool visible() const override;
            void visible(bool value) override;
            DisplayState displayState() const override;
            void displayState(DisplayState value) override;
            const Position& p() const override;
            void p(const Position& p) override;
            DialogType type() const override;
            bool showHelp() const override;
            void showHelp(bool value) override;
            Color color() const override;
            void color(Color value) override;
            Cursor cursor() const override;
            void cursor(Cursor value) override;
            bool dropTarget() const override;
            void dropTarget(bool value) override;
            bool mouseCapture() const override;
            void mouseCapture(bool value) override;
            const std::string& title() const override;
            void title(const std::string& value) override;
            const ImageSource& image() const override;
            void image(const ImageSource& image) override;
            virtual std::shared_ptr<dlgcpp::menus::Menu> menu() const override;
            virtual void menu(std::shared_ptr<dlgcpp::menus::Menu> menu) override;
            void* handle() const override;
            void* user() const override;
            void user(void* value) override;
            ISharedDialog parent() const override;

            // actions
            void show() override;
            void close(int result = 0) override;
            void setFocus() override;
            void bringToFront() override;
            void sendToBack() override;
            void move(const Point& point) override;
            void resize(const Size& size) override;
            void center() override;
            void message(const std::string& message, const std::string& title = std::string()) override;
            void sendUserEvent(int param = 0) override;
            void redraw(bool drawChildren = false) override;
            void timer(int timeout) override;

            // child management
            void add(std::shared_ptr<dlgcpp::controls::Control> child) override;
            void remove(std::shared_ptr<dlgcpp::controls::Control> child) override;
            std::vector<std::shared_ptr<dlgcpp::controls::Control>> controls() const override;
            void add(std::shared_ptr<Dialog> child) override;
            void remove(std::shared_ptr<Dialog> child) override;
            std::vector<std::shared_ptr<Dialog>> dialogs() const override;

            // utility
            Point toPixels(const Point& point) const;
            Size toPixels(const Size& size) const;
            Position toPixels(const Position& pos) const;

            // events
            IEvent<ISharedDialog>& ConfirmEvent() override;
            IEvent<ISharedDialog>& CancelEvent() override;
            IEvent<ISharedDialog>& HelpEvent() override;
            IEvent<ISharedDialog>& MoveEvent() override;
            IEvent<ISharedDialog>& SizeEvent() override;
            IEvent<ISharedDialog, ISharedDrawingContext>& PaintEvent() override;
            IEvent<ISharedDialog>& TimerEvent() override;
            IEvent<ISharedDialog, std::vector<std::string>>& DropEvent() override;
            IEvent<ISharedDialog, KeyboardEvent>& KeyDownEvent() override;
            IEvent<ISharedDialog, KeyboardEvent>& KeyUpEvent() override;
            IEvent<ISharedDialog, MouseEvent>& MouseDownEvent() override;
            IEvent<ISharedDialog, MouseEvent>& MouseUpEvent() override;
            IEvent<ISharedDialog, MouseEvent>& MouseMoveEvent() override;
            IEvent<ISharedDialog, MouseEvent>& MouseDoubleClickEvent() override;
            IEvent<ISharedDialog>& MouseCaptureLostEvent() override;
            IEvent<ISharedDialog, int>& UserEvent() override;

        protected:
            Dialog(std::shared_ptr<DialogImpl> impl);

            std::shared_ptr<DialogImpl> impl();

        private:
            std::shared_ptr<DialogImpl> _impl;
        };
    }
}
