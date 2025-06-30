cmake_minimum_required(VERSION 3.27)

set(target libdlgcpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(src_files
    neutral/point.cpp
    neutral/position.cpp
    neutral/size.cpp
)

if (WIN32)
set(src_files ${src_files}
    win32/control.cpp
    win32/control_p.h
    win32/controls/button.cpp
    win32/controls/button_p.h
    win32/controls/checkbox.cpp
    win32/controls/checkbox_p.h
    win32/controls/combobox.cpp
    win32/controls/combobox_p.h
    win32/controls/custom.cpp
    win32/controls/custom_p.h
    win32/controls/image.cpp
    win32/controls/image_p.h
    win32/controls/label.cpp
    win32/controls/label_p.h
    win32/controls/listbox.cpp
    win32/controls/listbox_p.h
    win32/controls/listview.cpp
    win32/controls/listview_col.cpp
    win32/controls/listview_p.h
    win32/controls/progress.cpp
    win32/controls/progress_p.h
    win32/controls/slider.cpp
    win32/controls/slider_imp.cpp
    win32/controls/slider_imp.h
    win32/controls/slider_p.h
    win32/controls/tab_item.cpp
    win32/controls/tabs.cpp
    win32/controls/tabs_p.h
    win32/controls/textbox.cpp
    win32/controls/textbox_p.h
    win32/controls/toolbar.cpp
    win32/controls/toolbar_item.cpp
    win32/controls/toolbar_p.h
    win32/dialog.cpp
    win32/dialog_p.h
    win32/dialogs/about.cpp
    win32/dialogs/about_p.h
    win32/dialogs/file.cpp
    win32/dialogs/file_p.h
    win32/dialogs/folder.cpp
    win32/dialogs/folder_p.h
    win32/dialogs/message.cpp
    win32/dialogs/message_p.h
    win32/dialogs/property.cpp
    win32/dialogs/property_p.h
    win32/dialogs/splash.cpp
    win32/dialogs/splash_p.h
    win32/dlgmsg.h
    win32/event_p.h
    win32/menu.cpp
    win32/menu_p.h
    win32/utility.cpp
    win32/utility.h
)
endif()

# define other platforms here

add_library(${target} STATIC ${src_files})

# requires unicode
target_compile_definitions(${target} PRIVATE UNICODE _UNICODE)

# consumers need the header target
target_include_directories(${target} PUBLIC "$<TARGET_PROPERTY:dlgcpp,SOURCE_DIR>")

if (DLGCPP_DEBUG)
    target_compile_definitions(${target} PRIVATE DLGCPP_DEBUG)
endif()

if (WIN32)
    target_link_libraries(${target} PUBLIC comctl32.lib)
endif()
