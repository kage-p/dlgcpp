#include "dlgcpp/controls/treeview_node.h"

using namespace dlgcpp::controls;

class dlgcpp::controls::TreeViewNodeImpl
{
public:
    int _id = 0;
    std::string _text;
    std::string _tag;
    bool _checked = false;
};

TreeViewNode::TreeViewNode(
    const std::string& tag,
    const std::string& text,
    bool checked) :
    _impl(std::make_shared< TreeViewNodeImpl>())
{
    _impl->_text = text;
    _impl->_checked = checked;
    _impl->_tag = tag;
}

TreeViewNode::TreeViewNode(
    int id,
    const std::string& text,
    bool checked,
    const std::string& tag) :
    TreeViewNode(tag, text, checked)
{
    _impl->_id = id;
}

int TreeViewNode::id() const
{
    return _impl->_id;
}

const std::string& TreeViewNode::tag() const
{
    return _impl->_tag;
}

const std::string& TreeViewNode::text() const
{
    return _impl->_text;
}

void TreeViewNode::text(const std::string& value)
{
    _impl->_text = value;
}

bool TreeViewNode::checked() const
{
    return _impl->_checked;
}

void TreeViewNode::checked(bool value)
{
    _impl->_checked = value;
}
