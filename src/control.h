//______________________________________________________________________________
//
//  DLGCPP: Win32 DIALOG wrapper for C++
//  ------------------------------------
//
//
//
//  ----------------------------------------
//  (C) Copyright Kevin G. Peel, 2005-2017.
//      Updated: Jul 2005. Nov 2007.
//______________________________________________________________________________

#pragma once

#include <string>
#include "utility.h"

namespace dlgcpp
{
    class IDialog;

    typedef struct _ctl_priv_props
    {
        std::string _className;
        bool _visible = true;
        bool _tabbable = true;
        Position _p;
        std::string _text;
        void* _hwnd = nullptr;
        int _id = -1;
    } _ctl_priv_props;

    class IControl
    {
    public:
        virtual int id() const = 0;
        virtual bool visible() const = 0;
        virtual void visible(bool value) = 0;
        virtual const Position& p() const = 0;
        virtual void p(const Position& p) = 0;
        virtual const std::string& text() const = 0;
        virtual void text(const std::string& value) = 0;
        virtual bool tabbable() const = 0;
        virtual void tabbable(bool value) = 0;
        virtual void* handle() = 0;
        virtual IDialog& parent() = 0;
        virtual long long sendMsg(unsigned int msg, long long wParam, long long lParam) = 0;
    };

    class Control : public IControl
    {

    public:

        // IControl impl.
        int id() const override;
        bool visible() const override;
        void visible(bool value) override;
        const Position& p() const override;
        void p(const Position& p) override;
        const std::string& text() const override;
        void text(const std::string& value) override;
        bool tabbable() const override;
        void tabbable(bool value) override;
        void* handle() override;
        IDialog& parent() override;
        long long sendMsg(unsigned int msg, long long wParam, long long lParam) override;

        void rebuild();
        void id(int value);
        unsigned int styles();
        unsigned int exStyles();

    protected:
        explicit Control(const std::string& className,
                         IDialog& parent);
        virtual ~Control();

    private:
        IDialog& _parent;
        _ctl_priv_props _props;

        void _rebuild();
        void _dump();
    };

    class Button : public Control
    {
    public:
        explicit Button(IDialog& parent)
            : Control("BUTTON", parent)
        {
        }
    };

    class Label : public Control
    {
    public:
        explicit Label(IDialog& parent)
            : Control("STATIC", parent)
        {
        }
    };

    class ListBox : public Control
    {
    public:
        explicit ListBox(IDialog& parent)
            : Control("LISTBOX", parent)
        {
        }
    };

    class TextBox : public Control
    {
    public:
        explicit TextBox(IDialog& parent)
            : Control("EDIT", parent)
        {
        }
    };
}
