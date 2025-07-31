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
            virtual bool checkboxes() const = 0;
            virtual void checkboxes(bool value) = 0;

            virtual bool editing() const = 0;
            virtual bool beginEditing(std::shared_ptr <TreeViewNode> node) = 0;
            virtual void confirmEditing() = 0;
            virtual void cancelEditing() = 0;

            virtual bool multiselect() const = 0;
            virtual void multiselect(bool value) = 0;
            virtual std::shared_ptr<TreeViewNode> selectedNode() const = 0;
            virtual void selectedNode(std::shared_ptr<TreeViewNode> node) = 0;
            virtual const std::vector<std::shared_ptr<TreeViewNode>>& selectedNodes() const = 0;
            virtual void selectedNodes(const std::vector<std::shared_ptr<TreeViewNode>>& nodes) = 0;

            // node handling
            virtual std::shared_ptr<TreeViewNode> rootNode() const = 0;
            virtual std::vector<std::shared_ptr<TreeViewNode>> childNodes(std::shared_ptr<TreeViewNode> parent) const = 0;
            virtual bool beginEdit(std::shared_ptr<TreeViewNode> node) = 0;
            virtual bool endEdit(std::shared_ptr<TreeViewNode> node, const std::string& text) = 0;

            // actions
            virtual void expand(std::shared_ptr<TreeViewNode> node, bool allChildNodes = false) = 0;
            virtual void collapse(std::shared_ptr<TreeViewNode> node, bool allChildNodes = false) = 0;

            virtual IEvent<ISharedControl>& RootNodeChangedEvent() = 0;
            virtual IEvent<ISharedControl>& SelChangedEvent() = 0;
            virtual IEvent<ISharedControl, std::shared_ptr<TreeViewNode>>& ItemClickEvent() = 0;
            virtual IEvent<ISharedControl, std::shared_ptr<TreeViewNode>>& ItemDoubleClickEvent() = 0;
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
            bool checkboxes() const override;
            void checkboxes(bool value) override;
            bool editing() const override;
            bool beginEditing(std::shared_ptr<TreeViewNode> node) override;
            void confirmEditing() override;
            void cancelEditing() override;
            bool multiselect() const override;
            void multiselect(bool value) override;
            std::shared_ptr<TreeViewNode> selectedNode() const override;
            void selectedNode(std::shared_ptr<TreeViewNode> node) override;
            const std::vector<std::shared_ptr<TreeViewNode>>& selectedNodes() const override;
            void selectedNodes(const std::vector<std::shared_ptr<TreeViewNode>>& nodes) override;

            // placeholders
            std::shared_ptr<TreeViewNode> rootNode() const override;
            std::vector<std::shared_ptr<TreeViewNode>> childNodes(std::shared_ptr<TreeViewNode> parent) const override;
            bool beginEdit(std::shared_ptr<TreeViewNode> node) override;
            bool endEdit(std::shared_ptr<TreeViewNode> node, const std::string& text) override;

            // actions
            void expand(std::shared_ptr<TreeViewNode> node, bool allChildNodes = false) override;
            void collapse(std::shared_ptr<TreeViewNode> node, bool allChildNodes = false) override;

            IEvent<ISharedControl>& RootNodeChangedEvent() override;
            IEvent<ISharedControl>& SelChangedEvent() override;
            IEvent<ISharedControl, std::shared_ptr<TreeViewNode>>& ItemClickEvent() override;
            IEvent<ISharedControl, std::shared_ptr<TreeViewNode>>& ItemDoubleClickEvent() override;

        private:
            TreeView(std::shared_ptr<TreeViewImpl> impl);

            std::shared_ptr<TreeViewImpl> _impl;
        };
    }
}
