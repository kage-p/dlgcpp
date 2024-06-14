#include "file_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;

void dialogs_file_demo(ISharedDialog parent)
{
    auto fileDlg = std::make_shared<FileDialog>(parent);

    fileDlg->title("Open File");
    fileDlg->filters("Text Files (*.txt)|*.txt");
    auto r = fileDlg->open();
    if (r)
        parent->message("File selected: " + fileDlg->fileName());
}
