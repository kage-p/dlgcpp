#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "defs.h"
#include "child.h"
#include "event.h"

#ifdef _WIN32
    #define NOMINMAX
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

namespace dlgcpp
{
    enum class DialogMessageType
    {
        Information = 0,
        Warning,
        Error
    };

    enum class DialogType
    {
        Application,
        Popup,
        Frameless,
        Tool
    };

    class IDialog
    {
    public:
        virtual bool enabled() const = 0;
        virtual void enabled(bool value) = 0;
        virtual bool visible() const = 0;
        virtual void visible(bool value) = 0;
        virtual const Position& p() const = 0;
        virtual void move(int x, int y) = 0;
        virtual void resize(int width, int height) = 0;
        virtual void center() = 0;
        virtual DialogType type() const = 0;
        virtual void type(DialogType value) = 0;
        virtual const std::string& title() const = 0;
        virtual void title(const std::string& value) = 0;
        virtual Color color() const = 0;
        virtual void color(Color value) = 0;
        virtual Cursor cursor() const = 0;
        virtual void cursor(Cursor value) = 0;
        virtual void* handle() const = 0;
        virtual void* user() const = 0;
        virtual void user(void* value) = 0;
        virtual std::shared_ptr<IDialog> parent() = 0;
        virtual void close(int result = 0) = 0;
        virtual void message(const std::string& message, const std::string& title, DialogMessageType type) = 0;
        virtual void timer(int timeout, std::function<void(void)> handler) = 0;

        // child management
        virtual void add(std::shared_ptr<IChild> child) = 0;
        virtual void remove(std::shared_ptr<IChild> child) = 0;
        virtual std::vector<std::shared_ptr<IControl>> children() const = 0;

        // events
        virtual IEvent& ClickEvent() = 0;
    };

    class Dialog : public IDialog, public std::enable_shared_from_this<Dialog>
    {
    public:
        explicit Dialog(std::shared_ptr<IDialog> parent = nullptr);
        virtual ~Dialog();
        std::shared_ptr<Dialog> shared_ptr();
        int exec();
        void quit(int result = 0);

        // IDialog impl.
        bool enabled() const override;
        void enabled(bool value) override;
        bool visible() const override;
        void visible(bool value) override;
        const Position& p() const override;
        void move(int x, int y) override;
        void resize(int width, int height) override;
        void center() override;
        DialogType type() const override;
        void type(DialogType value) override;
        Color color() const override;
        void color(Color value) override;
        Cursor cursor() const override;
        void cursor(Cursor value) override;
        const std::string& title() const override;
        void title(const std::string& value) override;
        void* handle() const override;
        void* user() const override;
        void user(void* value) override;
        std::shared_ptr<IDialog> parent() override;
        void close(int result = 0) override;
        void message(const std::string& message, const std::string& title = std::string(), DialogMessageType type = DialogMessageType::Information) override;
        void timer(int timeout, std::function<void(void)> handler) override;

        // child management
        void add(std::shared_ptr<IChild> child) override;
        void remove(std::shared_ptr<IChild> child) override;
        std::vector<std::shared_ptr<IControl>> children() const override;

        // events
        IEvent& ClickEvent() override;

    protected:
        std::shared_ptr<IChild> childFromId(int id);
        void redraw();

        // overridable by derived class
        virtual void rebuild();
        virtual void dump();
        virtual unsigned int styles() const;
        virtual unsigned int exStyles() const;
#ifdef _WIN32
        // window procedure
        virtual LRESULT defaultWndProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);
#endif

    private:
        struct dlg_props* _props;
        struct dlg_state* _state;

        int nextId();
        void updateTimer();
#ifdef _WIN32
        static LRESULT CALLBACK staticWndProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);
        LRESULT onSetCursor(HWND hwndChild);
        LRESULT onColorDlg(HDC hdc);
        LRESULT onColorCtl(HDC hdc, HWND hwndChild);
#endif
    };
}