#pragma once

#include "../event_p.h"
#include "dlgcpp/controls/treeview.h"
#include "utility/message.h"
#include <map>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <CommCtrl.h>

namespace dlgcpp
{
    namespace controls
    {
        struct treeview_priv
        {
            bool checkboxes = false;
            bool multiselect = false;
            bool editingLabel = false;
            HTREEITEM hRootNode = NULL;
            std::map<HTREEITEM, std::shared_ptr<TreeViewNode>> nodeMap;
            std::wstring displayBuffer;
            std::vector<std::shared_ptr<TreeViewNode>> selectedNodes;

            // event inhibitors
            MessageLockValue inhibitSelectionMessages;

            // events
            Event<ISharedControl> rootNodeChangedEvent;
            Event<ISharedControl> selChangedEvent;
            Event<ISharedControl, std::shared_ptr<TreeViewNode>> itemClickEvent;
            Event<ISharedControl, std::shared_ptr<TreeViewNode>> itemDblClickEvent;
            Event<std::shared_ptr<TreeViewNode>> expandNodePrivateEvent;
        };
    }
}
