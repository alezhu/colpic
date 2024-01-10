set(SOURCE_FILES
        src/main.cpp
        src/arguments.cpp
)

add_executable(app_decoder ${SOURCE_FILES})

target_link_libraries(app_decoder PRIVATE "-static" ColPicLib  ColPicCore mbits::args )

