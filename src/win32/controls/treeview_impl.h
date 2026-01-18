#pragma once

#include "control_impl.h"
#include "dlgcpp/controls/treeview.h"
#include <map>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <CommCtrl.h>

namespace dlgcpp
{
    namespace controls
    {
        class TreeViewImpl : public ControlImpl
        {
        public:
            explicit TreeViewImpl();
            ~TreeViewImpl() override;
            void owner(ITreeView* treeView);

            bool beginEditing(ISharedTreeViewNode node);
            void confirmEditing();
            void cancelEditing();

            void expand(ISharedTreeViewNode node, bool allChildNodes = false);
            void collapse(ISharedTreeViewNode node, bool allChildNodes = false);

        protected:
            void notify(struct ControlMessage&) override;

        private:
            ITreeView* _treeView = nullptr;

            // internals
            HTREEITEM _hRootNode = NULL;
            std::map<HTREEITEM, ISharedTreeViewNode> _nodeMap;
            std::wstring _displayBuffer;
            MessageLockValue _inhibitSelectionMessages;
            Event<int, ISharedTreeViewNode> _expandNodePrivateEvent;

            std::string className() const override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;
            void build() override;
            void destruct() override;

            void updateRootNode();
            void updateChildNodes(ISharedTreeViewNode parent);
            void updateSelection();
            void updateDisplayStyles();
            void updateExtStyles();

            void onRootNodeChanged();
            void onCheckboxesChanged();
            void onMultiselectChanged();
            void onSelectedNodeChanged();
            void onSelectedNodesChanged();

            HTREEITEM itemFromNode(ISharedTreeViewNode node);
            ISharedTreeViewNode nodeFromItem(HTREEITEM hitem);
        };
    }
}
