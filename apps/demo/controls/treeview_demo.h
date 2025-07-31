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
    std::shared_ptr<dlgcpp::TreeViewNode> rootNode() const override;
    std::vector<std::shared_ptr<dlgcpp::TreeViewNode>> childNodes(std::shared_ptr<dlgcpp::TreeViewNode> parent) const override;
    bool beginEdit(std::shared_ptr<dlgcpp::TreeViewNode> node) override;
    bool endEdit(std::shared_ptr<dlgcpp::TreeViewNode> node, const std::string& text) override;

private:
    std::shared_ptr<dlgcpp::TreeViewNode> _rootNode;
    std::multimap<std::string, std::shared_ptr<dlgcpp::TreeViewNode>> _nodeMap;
};
