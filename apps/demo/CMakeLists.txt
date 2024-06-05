cmake_minimum_required(VERSION 3.27)

set(target dlgdemo)

set(headers
    demo.h
)

set(sources
    demo.cpp
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
