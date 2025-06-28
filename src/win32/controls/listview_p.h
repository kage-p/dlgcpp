#pragma once

#include "dlgcpp/controls/listview.h"
#include "../event_p.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <CommCtrl.h>

namespace dlgcpp
{
    namespace controls
    {
        struct listview_priv
        {
            bool multiselect = false;
            ListViewDisplay _disp = ListViewDisplay::Details;
            std::vector<ISharedListViewColumn> columns;
            int currentIndex = -1;
            std::vector<int> currentIndexes;
            Event<ISharedControl> selChangedEvent;
            Event<ISharedControl> columnsChangedEvent;
            Event<ISharedControl> itemsChangedEvent;
        };
    }
}
