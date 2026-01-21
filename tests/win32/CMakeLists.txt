cmake_minimum_required(VERSION 3.27)

set(platform_src
    dialogs/dialog_impl_tests.cpp
    dialogs/dialog_impl_tests.h
    utility/convert_tests.cpp
    utility/convert_tests.h
)

# prefix paths
list(TRANSFORM platform_src PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

set(platform_includes
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(platform_deps
    comctl32
    gdiplus
)

set(platform_src ${platform_src} PARENT_SCOPE)
set(platform_includes ${platform_includes} PARENT_SCOPE)
set(platform_deps ${platform_deps} PARENT_SCOPE)

