cmake_minimum_required(VERSION 3.18)
project(OctoFishstick C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK4 REQUIRED gtk4)
pkg_check_modules(GEE REQUIRED gee-0.8)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GOBJECT REQUIRED gobject-2.0)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(JSONC REQUIRED json-c)

include_directories(
    ${GTK4_INCLUDE_DIRS}
    ${GEE_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${GOBJECT_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
    ${JSONC_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/external/live-chart-c/c-emitted
)

link_directories(
    ${GTK4_LIBRARY_DIRS}
    ${GEE_LIBRARY_DIRS}
    ${GLIB_LIBRARY_DIRS}
    ${GOBJECT_LIBRARY_DIRS}
    ${CAIRO_LIBRARY_DIRS}
    ${JSONC_LIBRARY_DIRS}
)

add_definitions(
    ${GTK4_CFLAGS_OTHER}
    ${GEE_CFLAGS_OTHER}
    ${GLIB_CFLAGS_OTHER}
    ${GOBJECT_CFLAGS_OTHER}
    ${CAIRO_CFLAGS_OTHER}
    ${JSONC_CFLAGS_OTHER}
)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})
link_directories(${CURL_LIBRARY_DIRS})

add_library(livechart STATIC IMPORTED)
set_target_properties(livechart PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/live-chart-c/liblivechart.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/external/live-chart-c"
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.c"
)

add_executable(OctoFishstick ${SOURCES})

target_link_libraries(OctoFishstick
    ${GTK4_LIBRARIES}
    ${GEE_LIBRARIES}
    ${GLIB_LIBRARIES}
    ${GOBJECT_LIBRARIES}
    ${CAIRO_LIBRARIES}
    ${CURL_LIBRARIES}
    ${JSONC_LIBRARIES}
    livechart
    m
)
