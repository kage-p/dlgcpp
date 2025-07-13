#pragma once
#include "dlgcpp/menu.h"
#include "event_p.h"
#include <map>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    static const int MenuStartId = 10000;

    struct menu_props
    {
        ISharedDialog parent;
        int startId = MenuStartId;
        std::vector<ISharedMenuItem> items;
    };

    struct menu_state
    {
        HMENU hMenu = NULL;
        std::map<int, ISharedMenuItem> idMap;
    };

    struct menui_props
    {
        int id = 0;
        std::string text;
        bool enabled = true;
        bool checked = false;
        bool defaultItem = false;
        bool separator = false;
        std::vector<ISharedMenuItem> items;

        Event<ISharedMenuItem> changedEvent;
        Event<ISharedMenuItem> clickEvent;
    };
}
