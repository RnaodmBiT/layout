cmake_minimum_required(VERSION 3.2)
project(tk-layout)

option(TK_BUILD_SHARED "TK_BUILD_SHARED" ON)

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/*.hpp)
add_definitions(-DTK_BUILD_LAYOUT)

set(BUILD_MODE STATIC)
if (TK_BUILD_SHARED)
    set(BUILD_MODE SHARED)
    add_definitions(-DTK_SHARED)
endif()

add_library(tk-layout ${BUILD_MODE} ${SOURCES} ${HEADERS})
add_dependencies(tk-layout tk-core)