cmake_minimum_required(VERSION 3.27)

set(target_name libdlgcpp)

set(headers
    dlgcpp.h
    control.h
    dialog.h
    utility.h
)

set(sources    
    control.cpp
    dialog.cpp
    utility.cpp
)

add_library(${target_name} STATIC ${sources} ${headers})
