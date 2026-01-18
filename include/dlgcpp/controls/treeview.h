#pragma once

#include "dlgcpp/controls/control.h"
#include "dlgcpp/controls/treeview_node.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class ITreeView : public virtual IControl
        {
        public:

            virtual IProperty<bool, ISharedControl>& checkboxes() = 0;
            virtual IProperty<bool, ISharedControl>& editing() = 0;
            virtual IProperty<bool, ISharedControl>& multiselect() = 0;
            virtual IProperty<ISharedTreeViewNode, ISharedControl>& selectedNode() = 0;
            virtual IProperty<std::vector<ISharedTreeViewNode>, ISharedControl>& selectedNodes() = 0;

            // editing
            virtual bool beginEditing(ISharedTreeViewNode node) = 0;
            virtual void confirmEditing() = 0;
            virtual void cancelEditing() = 0;

            // node handling
            virtual ISharedTreeViewNode rootNode() const = 0;
            virtual std::vector<ISharedTreeViewNode> childNodes(ISharedTreeViewNode parent) const = 0;
            virtual bool beginEdit(ISharedTreeViewNode node) = 0;
            virtual bool endEdit(ISharedTreeViewNode node, const std::string& text) = 0;

            // actions
            virtual void expand(ISharedTreeViewNode node, bool allChildNodes = false) = 0;
            virtual void collapse(ISharedTreeViewNode node, bool allChildNodes = false) = 0;

            // events
            virtual IEvent<ISharedControl>& ClickEvent() = 0;
            virtual IEvent<ISharedControl>& DoubleClickEvent() = 0;
            virtual IEvent<ISharedControl>& RightClickEvent() = 0;
            virtual IEvent<ISharedControl>& DoubleRightClickEvent() = 0;
            virtual IEvent<ISharedControl>& RootNodeChangedEvent() = 0;
            virtual IEvent<ISharedControl>& SelChangedEvent() = 0;
            virtual IEvent<ISharedControl, ISharedTreeViewNode>& ItemClickEvent() = 0;
            virtual IEvent<ISharedControl, ISharedTreeViewNode>& ItemDoubleClickEvent() = 0;
        };

        typedef std::shared_ptr<ITreeView> ISharedTreeView;

        class TreeViewImpl;

        class TreeView :
            public Control,
            public ITreeView
        {
        public:
            explicit TreeView(const Position& p = Position());
            ~TreeView() override;

            // ITreeView impl.
            IProperty<bool, ISharedControl>& checkboxes() override;
            IProperty<bool, ISharedControl>& editing() override;
            IProperty<bool, ISharedControl>& multiselect() override;
            IProperty<ISharedTreeViewNode, ISharedControl>& selectedNode() override;
            IProperty<std::vector<ISharedTreeViewNode>, ISharedControl>& selectedNodes() override;

            bool beginEditing(ISharedTreeViewNode node) override;
            void confirmEditing() override;
            void cancelEditing() override;

            // placeholders
            ISharedTreeViewNode rootNode() const override;
            std::vector<ISharedTreeViewNode> childNodes(ISharedTreeViewNode parent) const override;
            bool beginEdit(ISharedTreeViewNode node) override;
            bool endEdit(ISharedTreeViewNode node, const std::string& text) override;

            // actions
            void expand(ISharedTreeViewNode node, bool allChildNodes = false) override;
            void collapse(ISharedTreeViewNode node, bool allChildNodes = false) override;

            // events
            IEvent<ISharedControl>& ClickEvent() override;
            IEvent<ISharedControl>& DoubleClickEvent() override;
            IEvent<ISharedControl>& RightClickEvent() override;
            IEvent<ISharedControl>& DoubleRightClickEvent() override;
            IEvent<ISharedControl>& RootNodeChangedEvent() override;
            IEvent<ISharedControl>& SelChangedEvent() override;
            IEvent<ISharedControl, ISharedTreeViewNode>& ItemClickEvent() override;
            IEvent<ISharedControl, ISharedTreeViewNode>& ItemDoubleClickEvent() override;

            // compatibility setters
            void checkboxes(bool value);
            void multiselect(bool value);
            void selectedNode(ISharedTreeViewNode node);
            void selectedNodes(const std::vector<ISharedTreeViewNode>& nodes);

        private:
            TreeView(std::shared_ptr<TreeViewImpl> impl, const Position& p);

            std::shared_ptr<TreeViewImpl> _impl;

            Property<bool, ISharedControl> _checkboxes;
            Property<bool, ISharedControl> _editing;
            Property<bool, ISharedControl> _multiselect;
            Property<ISharedTreeViewNode, ISharedControl> _selectedNode;
            Property<std::vector<ISharedTreeViewNode>, ISharedControl> _selectedNodes;

            // events
            Event<ISharedControl> _clickEvent;
            Event<ISharedControl> _dblClickEvent;
            Event<ISharedControl> _rightClickEvent;
            Event<ISharedControl> _dblRightClickEvent;
            Event<ISharedControl> _rootNodeChangedEvent;
            Event<ISharedControl> _selChangedEvent;
            Event<ISharedControl, ISharedTreeViewNode> _itemClickEvent;
            Event<ISharedControl, ISharedTreeViewNode> _itemDblClickEvent;
        };
    }
}
