cmake_minimum_required(VERSION 3.27)

set(target dlgcpp_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(headers
    point_tests.h
    position_tests.h
    size_tests.h
    dialogs/dialog_tests.h
    mocks/mocks.h
)
set(sources
    point_tests.cpp
    position_tests.cpp
    size_tests.cpp
    dialogs/dialog_tests.cpp
)

add_executable(${target} WIN32 ${sources} ${headers})
add_test(NAME ${target} COMMAND ${target})

target_link_libraries(${target} PRIVATE libdlgcpp)
target_link_libraries(${target} PRIVATE libutfw)

target_include_directories(${target} PRIVATE "$<TARGET_PROPERTY:libutfw,SOURCE_DIR>")

if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif()
