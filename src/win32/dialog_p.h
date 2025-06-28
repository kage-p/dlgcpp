#pragma once
#include "dlgcpp/dialog.h"
#include "event_p.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    static const int ChildStartId = 100;
    static const int MenuStartId = 10000;

    static const int WM_DLGCPP_USER = WM_USER+1000;

    struct dlg_props
    {
        ISharedDialog parent;
        int id = 0;
        bool enabled = true;
        bool visible = false;  // hidden by default
        bool dropTarget = false;
        DialogType type = DialogType::Application;
        bool showHelp = false;
        Position p;
        std::string title;
        ImageSource image;
        std::shared_ptr<IChildMenu> menu;
        Color backColor = Color::Default;
        Cursor cursor = Cursor::Default;
        std::vector<std::shared_ptr<IChildControl>> controls;
        std::vector<std::shared_ptr<IChildDialog>> dialogs;
        int nextId = ChildStartId;
        bool execRunning = false;
        struct
        {
            int id = 0;
            int timeout = 0;
        } timer;
        void* user = nullptr;
        Event<ISharedDialog, MouseEvent> mouseDownEvent;
        Event<ISharedDialog, MouseEvent> mouseUpEvent;
        Event<ISharedDialog, MouseEvent> mouseMoveEvent;
        Event<ISharedDialog, MouseEvent> mouseDblClickEvent;
        Event<ISharedDialog> mouseCaptureLost;
        Event<ISharedDialog, std::vector<std::string>> dropEvent;
        Event<ISharedDialog> focusEvent;
        Event<ISharedDialog> helpEvent;
        Event<ISharedDialog> moveEvent;
        Event<ISharedDialog> sizeEvent;
        Event<ISharedDialog> timerEvent;
        Event<ISharedDialog, int> userEvent;
    };

    struct dlg_state
    {
        HWND hwnd = NULL;
        HBRUSH hbrBgColor = NULL;
        HGDIOBJ hImage = NULL;
        bool execParentEnabled = false;
    };

    struct dlg_priv
    {
        dlg_props props;
        dlg_state state;
    };
}
