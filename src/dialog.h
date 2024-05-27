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

#include <memory>
#include <string>
#include <vector>

#include "utility.h"

namespace dlgcpp
{
    class IControl;
    class Button;
    class TextBox;
    class Label;
    class ListBox;

    typedef struct _dlg_priv_props
    {
        bool _visible = true;
        Position _p;
        std::string _text;
        void* _hwnd = nullptr;
    } _priv_props;

    class IDialog
    {
    public:
        virtual bool visible() const = 0;
        virtual void visible(bool value) = 0;
        virtual const Position& p() const = 0;
        virtual void p(const Position& p) = 0;
        virtual const std::string& title() const = 0;
        virtual void title(const std::string& value) = 0;
        virtual void* handle() = 0;
        virtual std::shared_ptr<IDialog> parent() = 0;
        virtual void close() = 0;

        // child management
        virtual std::shared_ptr<IDialog> dialog(const std::string& title, const Position& p) = 0;
        virtual std::shared_ptr<IControl> button(const std::string& text, const Position& p = Position()) = 0;
        virtual std::shared_ptr<IControl> label(const std::string& text, const Position& p = Position()) = 0;
        virtual std::shared_ptr<IControl> listBox(const Position& p = Position()) = 0;
        virtual std::shared_ptr<IControl> textBox(const std::string& text, const Position& p = Position()) = 0;
        virtual void remove(std::shared_ptr<IControl> child) = 0;
        virtual const std::vector<std::shared_ptr<IControl>>& children() const = 0;
    };

    class Dialog : public IDialog
    {
    public:
        explicit Dialog(std::shared_ptr<IDialog> parent = nullptr);
        virtual ~Dialog();

        // IDialog impl.
        bool visible() const override;
        void visible(bool value) override;
        const Position& p() const override;
        void p(const Position& p) override;
        const std::string& title() const override;
        void title(const std::string& value) override;
        void* handle() override;
        std::shared_ptr<IDialog> parent() override;
        void close() override;

        std::shared_ptr<IDialog> dialog(const std::string& title, const Position& p) override;
        std::shared_ptr<IControl> button(const std::string& text, const Position& p = Position()) override;
        std::shared_ptr<IControl> label(const std::string& text, const Position& p = Position()) override;
        std::shared_ptr<IControl> listBox(const Position& p = Position()) override;
        std::shared_ptr<IControl> textBox(const std::string& text, const Position& p = Position()) override;
        void remove(std::shared_ptr<IControl> child) override;
        const std::vector<std::shared_ptr<IControl>>& children() const override;

        int exec();

    private:
        std::shared_ptr<IDialog> _parent;
        std::vector<std::shared_ptr<IControl>> _children;
        _dlg_priv_props _props;
        int _nextId = 100;

        void add(std::shared_ptr<IControl> child);
        int nextId();

        void _rebuild();
        void _dump();
    };

}
