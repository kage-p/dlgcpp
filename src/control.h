#pragma once

#include "defs.h"
#include "child.h"
#include "event.h"

#include <memory>
#include <string>

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace dlgcpp
{
    class IControl
    {
    public:
        virtual bool enabled() const = 0;
        virtual void enabled(bool value) = 0;
        virtual bool visible() const = 0;
        virtual void visible(bool value) = 0;
        virtual const Position& p() const = 0;
        virtual void p(const Position& p) = 0;
        virtual const std::string& text() const = 0;
        virtual void text(const std::string& value) = 0;
        virtual std::pair<Color, Color> colors() const = 0;
        virtual void colors(Color fgColor, Color bgColor) = 0;
        virtual const Font& font() const = 0;
        virtual void font(const Font& value) = 0;
        virtual Cursor cursor() const = 0;
        virtual void cursor(Cursor value) = 0;
        virtual void* handle() const = 0;
        virtual void* user() const = 0;
        virtual void user(void* value) = 0;
        virtual std::shared_ptr<IDialog> parent() = 0;
        virtual void redraw() = 0;
        virtual void setFocus() = 0;
#ifdef _WIN32
        virtual LRESULT sendMsg(UINT wMsg, WPARAM wParam = 0, LPARAM lParam = 0) = 0;
#endif

        // events
        virtual IEvent& CommandEvent() = 0;
    };

    class Control : public IChild,
                    public IControl,
                    public std::enable_shared_from_this<Control>
    {
    public:
        // IChild impl.
        int id() const override;
        void id(int value) override;
        std::shared_ptr<IControl> control() override;
        struct ctl_state state() override;

        // IControl impl.
        bool enabled() const override;
        void enabled(bool value) override;
        bool visible() const override;
        void visible(bool value) override;
        const Position& p() const override;
        void p(const Position& p) override;
        const std::string& text() const override;
        void text(const std::string& value) override;
        std::pair<Color, Color> colors() const override;
        void colors(Color fgColor, Color bgColor) override;
        const Font& font() const override;
        void font(const Font& value) override;
        Cursor cursor() const override;
        void cursor(Cursor value) override;
        void* handle() const override;
        void* user() const override;
        void user(void* value) override;
        std::shared_ptr<IDialog> parent() override;
        void redraw() override;
        void setFocus() override;
#ifdef _WIN32
        LRESULT sendMsg(UINT wMsg, WPARAM wParam = 0, LPARAM lParam = 0) override;
#endif
        // events
        IEvent& CommandEvent() override;

    protected:
        explicit Control(std::shared_ptr<IDialog> parent);
        virtual ~Control();
        std::shared_ptr<Control> shared_ptr();

        // overridable by subclass
        virtual void rebuild();
        virtual void dump();
        virtual std::string className() const;
        virtual unsigned int styles() const;
        virtual unsigned int exStyles() const;

    private:
        struct ctl_props* _props;
        struct ctl_state* _state;

        std::shared_ptr<IDialog> _parent;
        Event _commandEvent;
    };
}
