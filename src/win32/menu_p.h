#pragma once
#include "dlgcpp/menu.h"
#include "event_p.h"
#include <map>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    struct menu_props
    {
        std::shared_ptr<IDialog> parent;
        int startId = 0;
        std::vector<std::shared_ptr<IMenuItem>> items;
    };

    struct menu_state
    {
        HMENU hMenu = NULL;
        std::map<int, std::shared_ptr<IMenuItem>> idMap;
    };

    struct menui_props
    {
        int id = 0;
        std::string text;
        bool separator = false;
        std::vector<std::shared_ptr<IMenuItem>> items;
        Event<> clickedEvent;
    };
}
