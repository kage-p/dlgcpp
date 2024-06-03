cmake_minimum_required(VERSION 3.27)

set(target libdlgcpp)

set(headers
    dlgcpp.h
    child.h
    control.h
    control_p.h
    defs.h
    dialog.h
    dialog_p.h
    event.h
    utility.h
    controls/button.h
    controls/custom.h
    controls/custom_p.h
    controls/image.h
    controls/image_p.h
    controls/label.h
    controls/label_p.h
    controls/listbox.h
    controls/slider.h
    controls/textbox.h
    controls/textbox_p.h
    controls/p_slider.h
    dialogs/about.h
    dialogs/file.h
    dialogs/folder.h
    dialogs/splash.h
)

set(sources    
    control.cpp
    dialog.cpp
    event.cpp
    utility.cpp
    controls/button.cpp
    controls/custom.cpp
    controls/image.cpp
    controls/label.cpp
    controls/listbox.cpp
    controls/slider.cpp
    controls/textbox.cpp
    controls/p_slider.cpp
    dialogs/about.cpp
    dialogs/file.cpp
    dialogs/folder.cpp
    dialogs/splash.cpp
)

add_library(${target} STATIC ${sources} ${headers})

target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_link_libraries(${target} PRIVATE comctl32.lib)
endif()
