set(SOURCE_FILES
        src/main.cpp
        src/arguments.cpp
)

add_executable(app_encoder ${SOURCE_FILES})

target_link_libraries(app_encoder PRIVATE "-static" ColPicLib ColPicCore mbits::args )

