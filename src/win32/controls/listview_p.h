#pragma once

#include "control_p.h"
#include "dlgcpp/controls/listview.h"
#include "utility/event.h"
#include <map>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <CommCtrl.h>

namespace dlgcpp
{
    namespace controls
    {
        class ListViewEditProps
        {
        public:
            IListView* listView = nullptr;
            bool editing = false;
            int idEditor = 100;
            HWND hwndEditor = 0;
            size_t row = 0;
            int column = 0;
            int role = 0;
            bool confirmed = false;
        };

        class ListViewImpl : public ControlImpl
        {
        public:
            explicit ListViewImpl(
                IListView* listView,
                const Position& p = Position());

            ~ListViewImpl() override;

            ListViewDisplay display() const;
            void display(ListViewDisplay value);

            bool checkboxes() const;
            void checkboxes(bool value);

            bool editing() const;
            bool beginEditing(size_t row, int role);
            void confirmEditing();
            void cancelEditing();

            bool gridlines() const;
            void gridlines(bool value);

            bool multiselect() const;
            void multiselect(bool value);
            int selectedIndex() const;
            void selectedIndex(int value);
            const std::vector<int>& selectedIndexes() const;
            void selectedIndexes(const std::vector<int>& indexes);

            bool columnHeader() const;
            void columnHeader(bool value);

            bool sortColumns() const;
            void sortColumns(bool value);

            IEvent<ISharedControl>& SelChangedEvent();
            IEvent<ISharedControl>& ColumnsChangedEvent();
            IEvent<ISharedControl>& RowsChangedEvent();
            IEvent<ISharedControl, int>& ColumnClickEvent();
            IEvent<ISharedControl, size_t, int>& ItemClickEvent();
            IEvent<ISharedControl, size_t, int>& ItemDoubleClickEvent();

            void notify(struct ControlMessage&) override;
            void colors(Color fgColor, Color bgColor) override;

        private:
            IListView* _listView = nullptr;
            bool _checkboxes = false;
            bool _gridlines = false;
            bool _multiselect = false;
            bool _columnHeader = true;
            bool _sortColumns = true;
            ListViewDisplay _displayType = ListViewDisplay::Details;
            int _selectedIndex = -1;
            std::vector<int> _selectedIndexes;
            std::wstring _displayBuffer;
            std::map<int, int> _columnRoles;

            // edit state
            ListViewEditProps _editState;

            // events
            Event<ISharedControl> _columnsChangedEvent;
            Event<ISharedControl> _rowsChangedEvent;
            Event<ISharedControl> _selChangedEvent;
            Event<ISharedControl> _selChangedEventInternal;
            Event<ISharedControl, int> _columnClickEvent;
            Event<ISharedControl, size_t, int> _itemClickEvent;
            Event<ISharedControl, size_t, int> _itemDblClickEvent;

            std::string className() const override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;
            void rebuild() override;

            void updateSelection();
            void updateColumns();
            void updateRows();
            void updateDisplayStyles();
            void updateExtStyles();

            void onColumnsChanged();
            void onRowsChanged();
            void onSelectionChanged();

            static LRESULT CALLBACK ListViewEdit_SubclassProc(
                HWND hwnd,
                UINT msg,
                WPARAM wParam,
                LPARAM lParam,
                UINT_PTR uIdSubclass,
                DWORD_PTR dwRefData);
        };
    }
}
