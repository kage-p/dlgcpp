cmake_minimum_required(VERSION 3.27)

set(platform_src
    controls/control.cpp
    controls/control_p.h
    controls/button.cpp
    controls/button_p.h
    controls/checkbox.cpp
    controls/checkbox_p.h
    controls/combobox.cpp
    controls/combobox_p.h
    controls/custom.cpp
    controls/custom_p.h
    controls/image.cpp
    controls/image_p.h
    controls/label.cpp
    controls/label_p.h
    controls/listbox.cpp
    controls/listbox_p.h
    controls/listview.cpp
    controls/listview_p.h
    controls/option_btn.cpp
    controls/option_btn_p.h
    controls/progress.cpp
    controls/progress_p.h
    controls/slider.cpp
    controls/slider_p.h
    controls/tabs.cpp
    controls/tabs_p.h
    controls/textbox.cpp
    controls/textbox_p.h
    controls/toolbar.cpp
    controls/toolbar_p.h
    controls/trackbar.cpp
    controls/trackbar_p.h
    controls/treeview.cpp
    controls/treeview_p.h
    dialogs/dialog.cpp
    dialogs/dialog_p.h
    dialogs/about.cpp
    dialogs/about_p.h
    dialogs/file.cpp
    dialogs/file_p.h
    dialogs/folder.cpp
    dialogs/folder_p.h
    dialogs/message.cpp
    dialogs/message_p.h
    dialogs/property.cpp
    dialogs/property_p.h
    dialogs/splash.cpp
    dialogs/splash_p.h
    gfx/context.cpp
    gfx/context_p.h
    menus/menu.cpp
    menus/menu_p.h
    menus/menu_item.cpp
    menus/menu_item_p.h
    utility/convert.cpp
    utility/convert.h
    utility/font_loader.cpp
    utility/font_loader.h
    utility/image_reader.cpp
    utility/image_reader.h
    utility/key_mapper.cpp
    utility/key_mapper.h
    utility/message.cpp
    utility/message.h
    utility/string_encoder.cpp
    utility/string_encoder.h
)

# prefix paths
list(TRANSFORM platform_src PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

set(platform_includes
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(platform_deps
    comctl32
    gdiplus
	Msimg32
)

set(platform_src ${platform_src} PARENT_SCOPE)
set(platform_includes ${platform_includes} PARENT_SCOPE)
set(platform_deps ${platform_deps} PARENT_SCOPE)
