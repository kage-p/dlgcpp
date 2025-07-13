#include "dlgcpp/controls/treeview_node.h"

using namespace dlgcpp::controls;

TreeViewNode::TreeViewNode()
{
}

TreeViewNode::TreeViewNode(
    const std::string& tag,
    const std::string& text,
    bool checked) :
    _text(text),
    _checked(checked),
    _tag(tag)
{
}

TreeViewNode::TreeViewNode(
    int id,
    const std::string& text,
    bool checked,
    const std::string& tag) :
    _id(id),
    _text(text),
    _checked(checked),
    _tag(tag)
{
}

int TreeViewNode::id() const
{
    return _id;
}

const std::string& TreeViewNode::tag() const
{
    return _tag;
}

const std::string& TreeViewNode::text() const
{
    return _text;
}

void TreeViewNode::text(const std::string& value)
{
    _text = value;
}

bool TreeViewNode::checked() const
{
    return false;
}

void TreeViewNode::checked(bool value)
{
    _checked = value;
}
