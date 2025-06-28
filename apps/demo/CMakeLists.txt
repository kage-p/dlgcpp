cmake_minimum_required(VERSION 3.27)

set(target dlgcpp_demo)

set(headers
    demo.h
    advanced/clock_demo.h
    advanced/notes_demo.h
    advanced/popup_menu_demo.h
    controls/button_demo.h
    controls/checkbox_demo.h
    controls/combobox_demo.h
    controls/custom_demo.h
    controls/image_demo.h
    controls/label_demo.h
    controls/listbox_demo.h
    controls/listview_demo.h
    controls/progress_demo.h
    controls/slider_demo.h
    controls/tabs_demo.h
    controls/textbox_demo.h
    dialogs/about_demo.h
    dialogs/dialog_demo.h
    dialogs/file_demo.h
    dialogs/folder_demo.h
    dialogs/message_demo.h
    dialogs/property_demo.h
    dialogs/splash_demo.h
)

set(sources
    demo.cpp
    advanced/clock_demo.cpp
    advanced/notes_demo.cpp
    advanced/popup_menu_demo.cpp
    controls/button_demo.cpp    
    controls/checkbox_demo.cpp
    controls/combobox_demo.cpp
    controls/custom_demo.cpp
    controls/image_demo.cpp
    controls/label_demo.cpp
    controls/listbox_demo.cpp
    controls/listview_demo.cpp
    controls/progress_demo.cpp
    controls/slider_demo.cpp
    controls/tabs_demo.cpp
    controls/textbox_demo.cpp
    dialogs/about_demo.cpp
    dialogs/dialog_demo.cpp
    dialogs/file_demo.cpp
    dialogs/folder_demo.cpp
    dialogs/message_demo.cpp
    dialogs/property_demo.cpp
    dialogs/splash_demo.cpp
)

if (WIN32)
   enable_language("RC")
   set(other
       win32/demo.rc)
endif()

add_executable(${target}  ${sources} ${headers} ${other})

target_link_libraries(${target} libdlgcpp)

if (MSVC)
    target_link_options(${target} PRIVATE /manifest:NO /ENTRY:mainCRTStartup)
endif()
