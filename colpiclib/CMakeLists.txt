
set(HEADER_FILES
        include/global.h
        include/encoder.h
        include/colors_handler.h
        include/color_table_item.h
        include/color_table.h
        include/str_codec.h
)

set(SOURCE_FILES
        src/encoder.cpp
        src/colors_handler.cpp
        src/color_table_item.cpp
        src/color_table.cpp
        src/str_codec.cpp
)

add_library(ColPicLib STATIC ${SOURCE_FILES})
target_link_libraries(ColPicLib PUBLIC "-static")
target_include_directories(ColPicLib PUBLIC include)
