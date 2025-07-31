cmake_minimum_required(VERSION 3.27)

set(target dlgcpp_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(src_files
    dialogs/dialog_tests.cpp
    dialogs/dialog_tests.h
    mocks/mock_control.h
    mocks/mock_dialog.h
    mocks/mock_menu.h
    point_tests.cpp
    point_tests.h
    position_tests.cpp
    position_tests.h
    size_tests.cpp
    size_tests.h
)

if (WIN32)
    add_subdirectory(win32)
endif()

add_executable(${target} ${src_files} ${platform_src})
add_test(NAME ${target} COMMAND ${target})

# requires unicode
target_compile_definitions(${target} PRIVATE UNICODE _UNICODE)

target_link_libraries(${target} PRIVATE libdlgcpp)
target_link_libraries(${target} PRIVATE libutfw)

target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${target} PRIVATE ${platform_includes})
target_include_directories(${target} PRIVATE "$<TARGET_PROPERTY:libdlgcpp,SOURCE_DIR>")
target_include_directories(${target} PRIVATE "$<TARGET_PROPERTY:libutfw,SOURCE_DIR>")

if (WIN32)
    target_include_directories(${target} PRIVATE "$<TARGET_PROPERTY:libdlgcpp,SOURCE_DIR>/win32")
endif()

if (MSVC)
    target_link_options(${target} PRIVATE /manifest:NO /ENTRY:mainCRTStartup)
endif()
