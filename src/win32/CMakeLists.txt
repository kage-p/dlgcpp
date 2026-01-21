cmake_minimum_required(VERSION 3.27)

set(platform_src
    controls/button_impl.cpp
    controls/button_impl.h
    controls/checkbox_impl.cpp
    controls/checkbox_impl.h
    controls/combobox_impl.cpp
    controls/combobox_impl.h
    controls/control_impl.cpp
    controls/control_impl.h
    controls/custom_impl.cpp
    controls/custom_impl.h
    controls/image_impl.cpp
    controls/image_impl.h
    controls/label_impl.cpp
    controls/label_impl.h
    controls/listbox_impl.cpp
    controls/listbox_impl.h
    controls/listview_impl.cpp
    controls/listview_impl.h
    controls/option_btn_impl.cpp
    controls/option_btn_impl.h
    controls/progress_impl.cpp
    controls/progress_impl.h
    controls/slider_impl.cpp
    controls/slider_impl.h
    controls/tabs_impl.cpp
    controls/tabs_impl.h
    controls/textbox_impl.cpp
    controls/textbox_impl.h
    controls/toolbar_impl.cpp
    controls/toolbar_impl.h
    controls/trackbar_impl.cpp
    controls/trackbar_impl.h
    controls/treeview_impl.cpp
    controls/treeview_impl.h
    dialogs/about_impl.cpp
    dialogs/about_impl.h
    dialogs/dialog_impl.cpp
    dialogs/dialog_impl.h
    dialogs/file_impl.cpp
    dialogs/file_impl.h
    dialogs/folder_impl.cpp
    dialogs/folder_impl.h
    dialogs/message_impl.cpp
    dialogs/message_impl.h
    dialogs/property_impl.cpp
    dialogs/property_impl.h
    dialogs/splash_impl.cpp
    dialogs/splash_impl.h
    gfx/context_impl.cpp
    gfx/context_impl.h
    menus/menu_impl.cpp
    menus/menu_impl.h
    menus/menu_item_impl.cpp
    menus/menu_item_impl.h
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
