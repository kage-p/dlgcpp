#pragma once

#include <memory>
#include <string>
#include <vector>

#include "defs.h"
#include "child.h"
#include "event.h"
#include "menu.h"

#ifdef _WIN32
    #define NOMINMAX
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

namespace dlgcpp
{
    enum class DialogType
    {
        Application,
        Popup,
        Frameless,
        Tool
    };

    enum class DialogState
    {
        // TODO: impl
        Hidden = 0,
        Normal,
        Minimized,
        Maximized
    };

    class IDialog
    {
    public:
        virtual bool enabled() const = 0;
        virtual void enabled(bool value) = 0;
        virtual bool visible() const = 0;
        virtual void visible(bool value) = 0;
        virtual const Position& p() const = 0;
        virtual void move(const Point& point) = 0;
        virtual void resize(const Size& size) = 0;
        virtual void center() = 0;
        virtual DialogType type() const = 0;
        virtual bool showHelp() const = 0;
        virtual void showHelp(bool value) = 0;
        virtual const std::string& title() const = 0;        
        virtual void title(const std::string& value) = 0;
        virtual const ImageSource& image() const = 0;
        virtual void image(const ImageSource& image) = 0;
        virtual std::shared_ptr<IMenu> menu() const = 0;
        virtual void menu(std::shared_ptr<IChildMenu> menu) = 0;
        virtual Color color() const = 0;
        virtual void color(Color value) = 0;
        virtual Cursor cursor() const = 0;
        virtual void cursor(Cursor value) = 0;
        virtual bool dropTarget() const = 0;
        virtual void dropTarget(bool value) = 0;
        virtual void* handle() const = 0;
        virtual void* user() const = 0;
        virtual void user(void* value) = 0;
        virtual ISharedDialog parent() = 0;
        virtual void close(int result = 0) = 0;
        virtual void message(const std::string& message, const std::string& title = std::string()) = 0;
        virtual void timer(int timeout) = 0;

        // child management
        virtual void add(std::shared_ptr<IChildControl> child) = 0;
        virtual void remove(std::shared_ptr<IChildControl> child) = 0;
        virtual std::vector<std::shared_ptr<IControl>> controls() const = 0;
        virtual void add(std::shared_ptr<IChildDialog> child) = 0;
        virtual void remove(std::shared_ptr<IChildDialog> child) = 0;
        virtual std::vector<ISharedDialog> dialogs() const = 0;

        // events
        virtual IEvent<ISharedDialog, MouseButton, Point>& ClickEvent() = 0;
        virtual IEvent<ISharedDialog, MouseButton, Point>& DoubleClickEvent() = 0;
        virtual IEvent<ISharedDialog, std::vector<std::string>>& DropEvent() = 0;
        virtual IEvent<ISharedDialog>& HelpEvent() = 0;
        virtual IEvent<ISharedDialog>& MoveEvent() = 0;
        virtual IEvent<ISharedDialog>& SizeEvent() = 0;
        virtual IEvent<ISharedDialog>& TimerEvent() = 0;
    };

    class Dialog : public IChildDialog,
                   public IDialog,
                   public std::enable_shared_from_this<Dialog>
    {
    public:
        explicit Dialog(DialogType type = DialogType::Application,
                        ISharedDialog parent = nullptr);
        virtual ~Dialog();
        int exec();

        // IChildDialog impl.
        ISharedDialog parent() const override;
        void parent(ISharedDialog) override;
        int id() const override;
        void id(int value) override;
        ISharedDialog dialog() override;
        void notify(dlg_message&) override;
        void rebuild() override;

        // IDialog impl.
        bool enabled() const override;
        void enabled(bool value) override;
        bool visible() const override;
        void visible(bool value) override;
        const Position& p() const override;
        void move(const Point& point) override;
        void resize(const Size& size) override;
        void center() override;
        DialogType type() const override;
        bool showHelp() const override;
        void showHelp(bool value) override;
        Color color() const override;
        void color(Color value) override;
        Cursor cursor() const override;
        void cursor(Cursor value) override;
        bool dropTarget() const override;
        void dropTarget(bool value) override;
        const std::string& title() const override;
        void title(const std::string& value) override;
        const ImageSource& image() const override;
        void image(const ImageSource& image) override;
        virtual std::shared_ptr<IMenu> menu() const override;
        virtual void menu(std::shared_ptr<IChildMenu> menu) override;
        void* handle() const override;
        void* user() const override;
        void user(void* value) override;
        ISharedDialog parent() override;
        void close(int result = 0) override;
        void message(const std::string& message, const std::string& title = std::string()) override;
        void timer(int timeout) override;

        // child management
        void add(std::shared_ptr<IChildControl> child) override;
        void remove(std::shared_ptr<IChildControl> child) override;
        std::vector<std::shared_ptr<IControl>> controls() const override;
        void add(std::shared_ptr<IChildDialog> child) override;
        void remove(std::shared_ptr<IChildDialog> child) override;
        std::vector<ISharedDialog> dialogs() const override;

        // events
        IEvent<ISharedDialog, MouseButton, Point>& ClickEvent() override;
        IEvent<ISharedDialog, MouseButton, Point>& DoubleClickEvent() override;
        IEvent<ISharedDialog, std::vector<std::string>>& DropEvent() override;
        IEvent<ISharedDialog>& HelpEvent() override;
        IEvent<ISharedDialog>& MoveEvent() override;
        IEvent<ISharedDialog>& SizeEvent() override;
        IEvent<ISharedDialog>& TimerEvent() override;

    protected:
        std::shared_ptr<IChildControl> controlFromId(int id);
        void redraw(bool drawChildren = false);

        // overridable by derived class
        virtual unsigned int styles() const;
        virtual unsigned int exStyles() const;

    private:
        struct dlg_props* _props;
        struct dlg_state* _state;

        void destruct();
        int nextId();
        void quit(int result = 0);
        void updateImage();
        void updateTimer();

#ifdef _WIN32
        // TODO: replace these with internal impl.
        LRESULT onSetCursor(HWND hwndChild);
        LRESULT onColorDlg(HDC hdc);
        LRESULT onColorCtl(HDC hdc, HWND hwndChild);
#endif
    };
}
