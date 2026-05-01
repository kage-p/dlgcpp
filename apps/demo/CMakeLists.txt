cmake_minimum_required(VERSION 3.27)

set(target dlgcpp_demo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(src_files
    demo.cpp
    demo.h
    advanced/clock_demo.cpp
    advanced/clock_demo.h
    advanced/drawing_demo.cpp
    advanced/drawing_demo.h
    advanced/graph_demo.cpp
    advanced/graph_demo.h
    advanced/note_keeper_demo.cpp
    advanced/note_keeper_demo.h
    advanced/popup_menu_demo.cpp
    advanced/popup_menu_demo.h
    controls/button_demo.cpp    
    controls/button_demo.h
    controls/checkbox_demo.cpp
    controls/checkbox_demo.h
    controls/combobox_demo.cpp
    controls/combobox_demo.h
    controls/custom_demo.cpp
    controls/custom_demo.h
    controls/image_demo.cpp
    controls/image_demo.h
    controls/label_demo.cpp
    controls/label_demo.h
    controls/listbox_demo.cpp
    controls/listbox_demo.h
    controls/listview_demo.cpp
    controls/listview_demo.h
    controls/optionbtn_demo.cpp    
    controls/optionbtn_demo.h
    controls/progress_demo.cpp
    controls/progress_demo.h
    controls/slider_demo.cpp
    controls/slider_demo.h
    controls/tabs_demo.cpp
    controls/tabs_demo.h
    controls/textbox_demo.cpp
    controls/textbox_demo.h
    controls/toolbar_demo.cpp
    controls/toolbar_demo.h
    controls/trackbar_demo.cpp
    controls/trackbar_demo.h
    controls/treeview_demo.cpp
    controls/treeview_demo.h
    dialogs/about_demo.cpp
    dialogs/about_demo.h
    dialogs/dialog_demo.cpp
    dialogs/dialog_demo.h
    dialogs/file_demo.cpp
    dialogs/file_demo.h
    dialogs/folder_demo.cpp
    dialogs/folder_demo.h
    dialogs/message_demo.cpp
    dialogs/message_demo.h
    dialogs/property_demo.cpp
    dialogs/property_demo.h
    dialogs/splash_demo.cpp
    dialogs/splash_demo.h
)

if (WIN32)
   enable_language("RC")
   set(other
       assets/win32/demo.rc)
endif()

add_executable(${target} ${src_files} ${other})

if (WIN32)
    set_target_properties(${target} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# requires unicode
target_compile_definitions(${target} PRIVATE UNICODE _UNICODE)

target_link_libraries(${target} libdlgcpp)

if (MSVC)
    target_link_options(${target} PRIVATE /manifest:NO /ENTRY:mainCRTStartup)
endif()

# copy assets
add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/assets/images"
            "$<TARGET_FILE_DIR:${target}>/images"
    COMMENT "Copying entire images folder to build output"
)