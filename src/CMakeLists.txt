cmake_minimum_required(VERSION 3.27)

set(target libdlgcpp)

set(sources
    neutral/point.cpp
    neutral/position.cpp
    neutral/size.cpp
)

if (WIN32)
set(headers ${headers}
    win32/control_p.h
    win32/dialog_p.h
    win32/dlgmsg.h
    win32/event_p.h
    win32/menu_p.h
    win32/utility.h
    win32/controls/button_p.h
    win32/controls/checkbox_p.h
    win32/controls/combobox_p.h
    win32/controls/custom_p.h
    win32/controls/image_p.h
    win32/controls/label_p.h
    win32/controls/listbox_p.h
    win32/controls/slider_p.h
    win32/controls/textbox_p.h
    win32/controls/slider_imp.h
    win32/dialogs/about_p.h
    win32/dialogs/file_p.h
    win32/dialogs/folder_p.h
    win32/dialogs/message_p.h
    win32/dialogs/property_p.h
    win32/dialogs/splash_p.h
)

set(sources ${sources}
    win32/control.cpp
    win32/dialog.cpp
    win32/menu.cpp
    win32/utility.cpp
    win32/controls/button.cpp
    win32/controls/checkbox.cpp
    win32/controls/combobox.cpp
    win32/controls/custom.cpp
    win32/controls/image.cpp
    win32/controls/label.cpp
    win32/controls/listbox.cpp
    win32/controls/slider.cpp
    win32/controls/textbox.cpp
    win32/controls/slider_imp.cpp
    win32/dialogs/about.cpp
    win32/dialogs/file.cpp
    win32/dialogs/folder.cpp
    win32/dialogs/message.cpp
    win32/dialogs/property.cpp
    win32/dialogs/splash.cpp
)
endif()

# define other platforms here

add_library(${target} STATIC ${sources} ${headers})

# consumers need the header target
target_include_directories(${target} PUBLIC "$<TARGET_PROPERTY:dlgcpp,SOURCE_DIR>")

if (MSVC)
    target_link_libraries(${target} PRIVATE comctl32.lib)
endif()
