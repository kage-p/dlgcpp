#pragma once

#include "dlgcpp/dlgcpp.h"
#include <map>

void controls_treeview_demo(dlgcpp::ISharedDialog parent);

struct DemoTreeViewItem
{
    std::string path;
    std::string text;
};

class DemoTreeView : public dlgcpp::TreeView
{
public:
    DemoTreeView();

    void setItems(const std::vector<DemoTreeViewItem>& items);

    // TreeView impl.
    dlgcpp::ISharedTreeViewNode rootNode() const override;
    std::vector<dlgcpp::ISharedTreeViewNode> childNodes(dlgcpp::ISharedTreeViewNode parent) const override;
    bool beginEdit(dlgcpp::ISharedTreeViewNode node) override;
    bool endEdit(dlgcpp::ISharedTreeViewNode node, const std::string& text) override;

private:
    dlgcpp::ISharedTreeViewNode _rootNode;
    std::multimap<std::string, dlgcpp::ISharedTreeViewNode> _nodeMap;
};
