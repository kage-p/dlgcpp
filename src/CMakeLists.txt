cmake_minimum_required(VERSION 3.27)

set(target libdlgcpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    add_subdirectory(win32)
endif()

# define other platforms here

add_library(${target} STATIC ${platform_src})

# requires unicode
target_compile_definitions(${target} PRIVATE UNICODE _UNICODE)

# set include paths
target_include_directories(${target} PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(${target} PRIVATE ${platform_includes})

# set dependencies
target_link_libraries(${target} ${platform_deps})

if (DLGCPP_DEBUG)
    target_compile_definitions(${target} PRIVATE DLGCPP_DEBUG)
endif()
