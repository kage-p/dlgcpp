cmake_minimum_required(VERSION 3.27)

set(target dlgdemo)

set(headers
    demo.h
)

set(sources
    demo.cpp
)

add_executable(${target} WIN32 ${sources} ${headers})

target_link_libraries(${target} libdlgcpp)

if (WIN32)
    target_link_options(${target} PRIVATE /ENTRY:mainCRTStartup)
endif()
