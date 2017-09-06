cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

if (CMAKE_COMPILER_IS_GNUCC)
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets REQUIRED)

set(HEADERS main_window.h)
set(SOURCES main.cpp main_window.cpp envview.qrc)
set(OTHERS envview.png install.make)

#file(GLOB HEADERS *.h)
#file(GLOB SOURCES *.cpp *.ui *.qrc)

#include_directories(${CMAKE_SOURCE_DIR})

add_executable(envview ${SOURCES} ${HEADERS} ${OTHERS})
target_link_libraries(envview Qt5::Widgets)

install(TARGETS envview DESTINATION bin)
include(CPack)
