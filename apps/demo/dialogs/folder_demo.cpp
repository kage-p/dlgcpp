#include "folder_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;

void dialogs_folder_demo(ISharedDialog parent)
{
    auto folderDlg = std::make_shared<FolderDialog>(parent);

    folderDlg->title("Select Folder");
    folderDlg->message("Select the folder you need for this operation");
    auto r = folderDlg->open();
    if (r)
        parent->message("Folder selected: " + folderDlg->folderName());
}

