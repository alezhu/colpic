set(SOURCE_FILES
        src/main.cpp
)

add_executable(app_png_resize ${SOURCE_FILES})

target_link_libraries(app_png_resize PRIVATE "-static" ColPicLib ColPicCore mbits::args spng::spng_static avir::avir)

