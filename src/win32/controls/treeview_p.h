#pragma once

#include "control_p.h"
#include "dlgcpp/controls/treeview.h"
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
        class TreeViewImpl : public ControlImpl
        {
        public:
            explicit TreeViewImpl(TreeView& treeView, const Position& p = Position());
            ~TreeViewImpl() override;

            // properties
            void colors(Color fgColor, Color bgColor) override;
            bool checkboxes() const;
            void checkboxes(bool value);
            bool editing() const;
            bool beginEditing(std::shared_ptr<TreeViewNode> node);
            void confirmEditing();
            void cancelEditing();
            bool multiselect() const;
            void multiselect(bool value);
            std::shared_ptr<TreeViewNode> selectedNode() const;
            void selectedNode(std::shared_ptr<TreeViewNode> node);
            const std::vector<std::shared_ptr<TreeViewNode>>& selectedNodes() const;
            void selectedNodes(const std::vector<std::shared_ptr<TreeViewNode>>& nodes);

            // actions
            void expand(std::shared_ptr<TreeViewNode> node, bool allChildNodes = false);
            void collapse(std::shared_ptr<TreeViewNode> node, bool allChildNodes = false);

            // events
            IEvent<ISharedControl>& RootNodeChangedEvent();
            IEvent<ISharedControl>& SelChangedEvent();
            IEvent<ISharedControl, std::shared_ptr<TreeViewNode>>& ItemClickEvent();
            IEvent<ISharedControl, std::shared_ptr<TreeViewNode>>& ItemDoubleClickEvent();

        protected:
            void notify(struct ControlMessage&) override;

        private:
            TreeView& _treeView;
            bool _checkboxes = false;
            bool _multiselect = false;
            bool _editingLabel = false;
            HTREEITEM _hRootNode = NULL;
            std::map<HTREEITEM, std::shared_ptr<TreeViewNode>> _nodeMap;
            std::wstring _displayBuffer;
            std::vector<std::shared_ptr<TreeViewNode>> _selectedNodes;

            // event inhibitors
            MessageLockValue _inhibitSelectionMessages;

            // events
            Event<ISharedControl> _rootNodeChangedEvent;
            Event<ISharedControl> _selChangedEvent;
            Event<ISharedControl, std::shared_ptr<TreeViewNode>> _itemClickEvent;
            Event<ISharedControl, std::shared_ptr<TreeViewNode>> _itemDblClickEvent;
            Event<std::shared_ptr<TreeViewNode>> _expandNodePrivateEvent;

            std::string className() const override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;
            void rebuild() override;

            void updateRootNode();
            void updateChildNodes(std::shared_ptr<TreeViewNode> parent);
            void updateSelection();
            void updateDisplayStyles();
            void updateExtStyles();

            void onRootNodeChanged();

            HTREEITEM itemFromNode(std::shared_ptr<TreeViewNode> node);
            std::shared_ptr<TreeViewNode> nodeFromItem(HTREEITEM hitem);
        };
    }
}
