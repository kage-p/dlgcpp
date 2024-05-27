cmake_minimum_required(VERSION 3.27)

# ----- project globals

project(DLGCPP
    VERSION "1.0.0"
    HOMEPAGE_URL ""
    DESCRIPTION "Encapsulates Win32 DIALOG for C++"
    LANGUAGES CXX
)

option(DLGCPP_DEMO "Build DLGCPP Demo" OFF)

# ----- subprojects

add_subdirectory(src)

if (DLGCPP_DEMO)
    add_subdirectory(apps/demo)
endif()
