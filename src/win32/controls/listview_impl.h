#pragma once

#include "control_impl.h"
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
        class ListViewEditProps
        {
        public:
            IListView* listView = nullptr;
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
            explicit ListViewImpl();
            ~ListViewImpl() override;
            void owner(IListView* listView);

            bool beginEditing(size_t row, int role);
            void confirmEditing();
            void cancelEditing();

            void notify(struct ControlMessage&) override;

        private:
            IListView* _listView = nullptr;

            // internals 
            ListViewEditProps _editState;
            std::wstring _displayBuffer;
            std::map<int, int> _columnRoles;
            MessageLockValue _inhibitSelectionMessages;
            Event<int> _selChangedPrivateEvent;

            std::string className() const override;
            unsigned int styles() const override;
            bool wantInternalEvents() const override;
            void notify(DialogMessage&) override;
            void build() override;
            void destruct() override;

            void updateSelection();
            void updateColumns();
            void updateRows();
            void updateDisplayStyles();
            void updateExtStyles();

            void onColumnsChanged();
            void onDisplayChanged();
            void onMultiselectChanged();
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
