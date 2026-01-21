//______________________________________________________________________________
//
//  DLGCPP: Win32 DIALOG wrapper for C++
//  ------------------------------------
//
//
//
//  ----------------------------------------
//  (C) Copyright Kevin G. Peel, 2005-2017.
//      Updated: Jul 2005. Nov 2007.
//______________________________________________________________________________

#pragma once

#include "dlgcpp/defs.h"

#include "dlgcpp/controls/button.h"
#include "dlgcpp/controls/checkbox.h"
#include "dlgcpp/controls/combobox.h"
#include "dlgcpp/controls/custom.h"
#include "dlgcpp/controls/image.h"
#include "dlgcpp/controls/label.h"
#include "dlgcpp/controls/listbox.h"
#include "dlgcpp/controls/listview.h"
#include "dlgcpp/controls/option_btn.h"
#include "dlgcpp/controls/progress.h"
#include "dlgcpp/controls/slider.h"
#include "dlgcpp/controls/tabs.h"
#include "dlgcpp/controls/textbox.h"
#include "dlgcpp/controls/toolbar.h"
#include "dlgcpp/controls/trackbar.h"
#include "dlgcpp/controls/treeview.h"

#include "dlgcpp/dialogs/about.h"
#include "dlgcpp/dialogs/dialog.h"
#include "dlgcpp/dialogs/file.h"
#include "dlgcpp/dialogs/folder.h"
#include "dlgcpp/dialogs/message.h"
#include "dlgcpp/dialogs/property.h"
#include "dlgcpp/dialogs/splash.h"

#include "dlgcpp/menus/menu.h"

namespace dlgcpp
{
    // flatten the namespaces so consumers only need to use dlgcpp
    using dialogs::AboutDialog;
    using dialogs::Dialog;
    using dialogs::DialogType;
    using dialogs::DisplayState;
    using dialogs::FileDialog;
    using dialogs::FolderDialog;
    using dialogs::MessageDialog;
    using dialogs::MessageDialogButton;
    using dialogs::MessageDialogButtonGroup;
    using dialogs::PropertyDialog;
    using dialogs::SplashDialog;

    using menus::Menu;
    using menus::MenuItem;

    using controls::BorderStyle;
    using controls::Button;
    using controls::CheckBox;
    using controls::ComboBox;
    using controls::Custom;
    using controls::Image;
    using controls::Label;
    using controls::ListBox;
    using controls::ListView;
    using controls::ListViewColumn;
    using controls::ISharedListViewColumn;
    using controls::ListViewDisplay;
    using controls::OptionButton;
    using controls::Progress;
    using controls::Slider;
    using controls::Tabs;
    using controls::TabItem;
    using controls::ISharedTabItem;
    using controls::TextBox;
    using controls::ToolBar;
    using controls::ToolBarItem;
    using controls::ISharedToolBarItem;
    using controls::TrackBar;
    using controls::TrackBarTickMark;
    using controls::TreeView;
    using controls::TreeViewNode;
    using controls::ISharedTreeViewNode;
}
