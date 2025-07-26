cmake_minimum_required(VERSION 3.27)

set(target libdlgcpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(src_files
    controls/button.cpp
    controls/checkbox.cpp
    controls/combobox.cpp
    controls/control.cpp
    controls/custom.cpp
    controls/image.cpp
    controls/label.cpp
    controls/listbox.cpp
    controls/listview.cpp
    controls/listview_col.cpp
    controls/option_btn.cpp
    controls/progress.cpp
    controls/slider.cpp
    controls/tab_item.cpp
    controls/tabs.cpp
    controls/textbox.cpp
    controls/toolbar.cpp
    controls/toolbar_item.cpp
    controls/trackbar.cpp
    controls/treeview.cpp
    controls/treeview_node.cpp
    dialogs/dialog.cpp
    dialogs/dialog.cpp
    dialogs/about.cpp
    dialogs/file.cpp
    dialogs/folder.cpp
    dialogs/message.cpp
    dialogs/property.cpp
    dialogs/splash.cpp
    gfx/context.cpp
    menus/menu.cpp
    menus/menu_item.cpp
    utility/event.h
)

if (WIN32)
    add_subdirectory(win32)
endif()

# define other platforms here

add_library(${target} STATIC ${src_files} ${platform_src})

# requires unicode
target_compile_definitions(${target} PRIVATE UNICODE _UNICODE)

# set include paths
target_include_directories(${target} PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${target} PRIVATE ${platform_includes})

# set dependencies
target_link_libraries(${target} ${platform_deps})

if (DLGCPP_DEBUG)
    target_compile_definitions(${target} PRIVATE DLGCPP_DEBUG)
endif()
