#pragma once

#include "../event_p.h"
#include "dlgcpp/controls/listview.h"

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
            bool checkboxes = false;
            bool gridlines = false;
            ListViewDisplay _disp = ListViewDisplay::Details;
            std::vector<ISharedListViewColumn> columns;
            int selectedIndex = -1;
            std::vector<int> selectedIndexes;
            std::wstring _displayBuffer;
            Event<ISharedControl> columnsChangedEvent;
            Event<ISharedControl> itemsChangedEvent;
            Event<ISharedControl> selChangedEvent;
            Event<ISharedControl> selChangedEventInternal;
        };
    }
}
