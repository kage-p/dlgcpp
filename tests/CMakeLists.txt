cmake_minimum_required(VERSION 3.27)
include(GoogleTest)

set(target dlgcpp_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(src_files
    unit_test.cpp
    unit_test.h
    point_tests.cpp
    point_tests.h
    position_tests.cpp
    position_tests.h
    size_tests.cpp
    size_tests.h
    controls/button_tests.cpp
    controls/button_tests.h
    controls/checkbox_tests.cpp
    controls/checkbox_tests.h
    controls/combobox_tests.cpp
    controls/combobox_tests.h
    controls/control_tests.cpp
    controls/control_tests.h
    controls/custom_tests.cpp
    controls/custom_tests.h
    controls/image_tests.cpp
    controls/image_tests.h
    controls/label_tests.cpp
    controls/label_tests.h
    controls/listbox_tests.cpp
    controls/listbox_tests.h
    controls/listview_col_tests.cpp
    controls/listview_col_tests.h
    controls/listview_tests.cpp
    controls/listview_tests.h
    controls/option_btn_tests.cpp
    controls/option_btn_tests.h
    controls/progress_tests.cpp
    controls/progress_tests.h
    controls/slider_tests.cpp
    controls/slider_tests.h
    controls/tabs_tests.cpp
    controls/tabs_tests.h
    controls/tab_item_tests.cpp
    controls/tab_item_tests.h
    controls/textbox_tests.cpp
    controls/textbox_tests.h
    controls/toolbar_item_tests.cpp
    controls/toolbar_item_tests.h
    controls/toolbar_tests.cpp
    controls/toolbar_tests.h
    controls/trackbar_tests.cpp
    controls/trackbar_tests.h
    controls/treeview_node_tests.cpp
    controls/treeview_node_tests.h
    controls/treeview_tests.cpp
    controls/treeview_tests.h
    dialogs/dialog_tests.cpp
    dialogs/dialog_tests.h
    menus/menu_tests.h
    menus/menu_tests.cpp
    menus/menu_item_tests.h
    menus/menu_item_tests.cpp
    mocks/fake_dialog.h
    mocks/fake_control.h
    mocks/mock_control.h
    mocks/mock_dialog.h
    mocks/mock_event.h
    mocks/mock_menu.h
    mocks/mock_property.h
    utility/event_tests.h
    utility/event_tests.cpp
    utility/property_tests.h
    utility/property_tests.cpp
)

if (WIN32)
    add_subdirectory(win32)
endif()

add_executable(${target} ${src_files} ${platform_src})

# requires unicode
target_compile_definitions(${target} PRIVATE UNICODE _UNICODE)

target_link_libraries(${target} PRIVATE libdlgcpp)
target_link_libraries(${target} PRIVATE gmock)
target_link_libraries(${target} PRIVATE gtest_main)

target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${target} PRIVATE ${platform_includes})

if (WIN32)
    # Access internals
    target_include_directories(${target} PRIVATE "$<TARGET_PROPERTY:libdlgcpp,SOURCE_DIR>/win32")
endif()

# enable testing
gtest_discover_tests(${target})
set_property(TARGET ${target} PROPERTY VS_TEST true)

# copy assets
add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:${target}>/assets"
    COMMENT "Copying assets folder to build output"
)
