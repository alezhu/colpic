cmake_minimum_required(VERSION 3.25)
project(colpic)

set(CMAKE_CXX_STANDARD 20)

message("Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeHelpers")

if (MSVC)
    if (CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE INTERNAL "")
    else ()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE INTERNAL "")
    endif ()
endif ()

find_package(mbits-args)
find_package(base64)
find_package(SPNG)
find_package(avir)
#find_package(libvips)
#find_package(libx264)
#find_package(OpenCV)

add_subdirectory(colpiclib)
add_subdirectory(colpic_core)
add_subdirectory(app_compare)
add_subdirectory(app_converter)
add_subdirectory(app_decoder)
add_subdirectory(app_encoder)
add_subdirectory(app_png_resize)
