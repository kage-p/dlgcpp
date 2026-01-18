#include "dlgcpp/controls/treeview_node.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

TreeViewNode::TreeViewNode(
    int id,
    const std::string& text,
    bool checked,
    const std::string& tag)
{
    auto ownerFn = [this]() -> ISharedTreeViewNode
        {
            return std::static_pointer_cast<dlgcpp::controls::ITreeViewNode>(shared_from_this());
        };

    _id.reset(id, nullptr, ownerFn, "id");
    _tag.reset(tag, nullptr, ownerFn, "tag");
    _text.reset(text, nullptr, ownerFn, "text");
    _checked.reset(checked, nullptr, ownerFn, "checked");
}

TreeViewNode::TreeViewNode(
    const std::string& tag,
    const std::string& text,
    bool checked) :
    TreeViewNode(0, text, checked, tag)
{
}

IReadOnlyProperty<int, ISharedTreeViewNode>& TreeViewNode::id()
{
    return _id;
}

IReadOnlyProperty<std::string, ISharedTreeViewNode>& TreeViewNode::tag()
{
    return _tag;
}

IProperty<std::string, ISharedTreeViewNode>& TreeViewNode::text()
{
    return _text;
}

IProperty<bool, ISharedTreeViewNode>& TreeViewNode::checked()
{
    return _checked;
}
