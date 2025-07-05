#pragma once

#include "../event_p.h"
#include "dlgcpp/controls/listview.h"
#include <map>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <CommCtrl.h>

namespace dlgcpp
{
    namespace controls
    {
        struct listview_edit_priv
        {
            ListView* listView = nullptr;
            bool editing = false;
            int idEditor = 100;
            HWND hwndEditor = 0;
            size_t row = 0;
            int column = 0;
            int role = 0;
            bool confirmed = false;
        };

        struct listview_priv
        {
            bool checkboxes = false;
            bool gridlines = false;
            bool multiselect = false;
            ListViewDisplay _disp = ListViewDisplay::Details;
            int selectedIndex = -1;
            std::vector<int> selectedIndexes;
            std::wstring _displayBuffer;
            std::map<int, int> columnRoles;

            // edit state
            listview_edit_priv editState;

            // events
            Event<ISharedControl> columnsChangedEvent;
            Event<ISharedControl> rowsChangedEvent;
            Event<ISharedControl> selChangedEvent;
            Event<ISharedControl> selChangedEventInternal;
            Event<ISharedControl, int> columnClickEvent;
            Event<ISharedControl, size_t, int> itemClickEvent;
            Event<ISharedControl, size_t, int> itemDblClickEvent;
        };
    }
}
