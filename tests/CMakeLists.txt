cmake_minimum_required(VERSION 3.27)

set(target dlgcpp_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(src_files
    dialogs/dialog_tests.cpp
    dialogs/dialog_tests.h
    mocks/mocks.h
    point_tests.cpp
    point_tests.h
    position_tests.cpp
    position_tests.h
    size_tests.cpp
    size_tests.h
)

add_executable(${target} WIN32 ${src_files})
add_test(NAME ${target} COMMAND ${target})

# requires unicode
target_compile_definitions(${target} PRIVATE UNICODE _UNICODE)

target_link_libraries(${target} PRIVATE libdlgcpp)
target_link_libraries(${target} PRIVATE libutfw)

target_include_directories(${target} PRIVATE "$<TARGET_PROPERTY:libutfw,SOURCE_DIR>")

if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif()
