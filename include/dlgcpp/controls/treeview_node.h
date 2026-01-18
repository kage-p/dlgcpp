#pragma once

#include "dlgcpp/defs.h"
#include "dlgcpp/property.h"
#include <memory>
#include <string>

namespace dlgcpp
{
    namespace controls
    {
        class ITreeViewNode;
        typedef std::shared_ptr<ITreeViewNode> ISharedTreeViewNode;

        class ITreeViewNode
        {
        public:
            virtual IReadOnlyProperty<int, ISharedTreeViewNode>& id() = 0;
            virtual IReadOnlyProperty<std::string, ISharedTreeViewNode>& tag() = 0;
            virtual IProperty<std::string, ISharedTreeViewNode>& text() = 0;
            virtual IProperty<bool, ISharedTreeViewNode>& checked() = 0;
        };

        class TreeViewNode :
            public ITreeViewNode,
            public std::enable_shared_from_this<TreeViewNode>
        {
        public:

            // construct with id
            explicit TreeViewNode(
                int id = 0,
                const std::string& text = {},
                bool checked = false,
                const std::string& tag = {});

            // construct with tag
            explicit TreeViewNode(
                const std::string& tag,
                const std::string& text = {},
                bool checked = false);

            ~TreeViewNode() = default;

            // ITreeViewNode impl.
            IReadOnlyProperty<int, ISharedTreeViewNode>& id() override;
            IReadOnlyProperty<std::string, ISharedTreeViewNode>& tag() override;
            IProperty<std::string, ISharedTreeViewNode>& text() override;
            IProperty<bool, ISharedTreeViewNode>& checked() override;

        private:
            Property<int, ISharedTreeViewNode> _id;
            Property<std::string, ISharedTreeViewNode> _tag;
            Property<std::string, ISharedTreeViewNode> _text;
            Property<bool, ISharedTreeViewNode> _checked;
        };
    }
}