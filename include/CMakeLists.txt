cmake_minimum_required(VERSION 3.27)

set(target dlgcpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(src_files
    dlgcpp/align.h
    dlgcpp/clipbrd.h
    dlgcpp/colors.h
    dlgcpp/defs.h
    dlgcpp/dlgcpp.h
    dlgcpp/drop.h
    dlgcpp/event.h
    dlgcpp/font.h
    dlgcpp/image.h
    dlgcpp/keys.h
    dlgcpp/mouse.h
    dlgcpp/point.h
    dlgcpp/position.h
	dlgcpp/property.h
    dlgcpp/size.h
    dlgcpp/controls/button.h
    dlgcpp/controls/checkbox.h
    dlgcpp/controls/combobox.h
    dlgcpp/controls/control.h
    dlgcpp/controls/custom.h
    dlgcpp/controls/image.h
    dlgcpp/controls/label.h
    dlgcpp/controls/listbox.h
    dlgcpp/controls/listview.h
    dlgcpp/controls/listview_col.h
    dlgcpp/controls/option_btn.h
    dlgcpp/controls/progress.h
    dlgcpp/controls/slider.h
    dlgcpp/controls/tabs.h
    dlgcpp/controls/tab_item.h
    dlgcpp/controls/textbox.h
    dlgcpp/controls/toolbar.h
    dlgcpp/controls/toolbar_item.h
    dlgcpp/controls/trackbar.h
    dlgcpp/controls/treeview.h
    dlgcpp/controls/treeview_node.h
    dlgcpp/dialogs/about.h
    dlgcpp/dialogs/dialog.h
    dlgcpp/dialogs/file.h
    dlgcpp/dialogs/folder.h
    dlgcpp/dialogs/message.h
    dlgcpp/dialogs/property.h
    dlgcpp/dialogs/splash.h
    dlgcpp/gfx/context.h
    dlgcpp/menus/menu.h
    dlgcpp/menus/menu_item.h
)

add_library(${target} INTERFACE ${src_files})
