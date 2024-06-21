cmake_minimum_required(VERSION 3.27)

# ----- project globals

project(DLGCPP
    VERSION "1.0.0"
    HOMEPAGE_URL ""
    DESCRIPTION "Encapsulates Win32 DIALOG for C++"
    LANGUAGES CXX
)

option(DLGCPP_DEBUG "DLGCPP verbose debug output" OFF)
option(DLGCPP_DEMO "Build DLGCPP Demo" ON)
option(DLGCPP_TESTS "Build DLGCPP Tests" ON)

# ----- subprojects

add_subdirectory(include)
add_subdirectory(src)

if (DLGCPP_DEMO)
    add_subdirectory(apps/demo)
endif()

if (DLGCPP_TESTS)
    add_subdirectory(3rdparty/utframework)
    add_subdirectory(tests)
endif()

# ----- installer

include(GNUInstallDirs)
set(INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")

install(DIRECTORY ${INCLUDES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
