#pragma once

#include "dlgcpp/dlgcpp.h"
#include <map>

void controls_treeview_demo(dlgcpp::ISharedDialog parent);

struct TreeViewDemoItem
{
    std::string path;
    std::string text;
};

class TreeViewDemoImpl : public dlgcpp::controls::TreeView
{
public:
    TreeViewDemoImpl();

    void setItems(const std::vector<TreeViewDemoItem>& items);

    // TreeView impl.
    std::shared_ptr<dlgcpp::controls::TreeViewNode> rootNode() const override;
    std::vector<std::shared_ptr<dlgcpp::controls::TreeViewNode>> childNodes(std::shared_ptr<dlgcpp::controls::TreeViewNode> parent) const override;
    bool beginEdit(std::shared_ptr<dlgcpp::controls::TreeViewNode> node) override;
    bool endEdit(std::shared_ptr<dlgcpp::controls::TreeViewNode> node, const std::string& text) override;

private:
    std::shared_ptr<dlgcpp::controls::TreeViewNode> _rootNode;
    std::multimap<std::string, std::shared_ptr<dlgcpp::controls::TreeViewNode>> _nodeMap;
};
