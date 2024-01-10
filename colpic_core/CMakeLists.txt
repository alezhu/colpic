
set(HEADER_FILES
        include/png_decoder.h
        include/png_encoder.h
        include/image_scaler.h
        include/thumbnail_creator.h
)

set(SOURCE_FILES
        src/png_decoder.cpp
        src/image_scaler.cpp
        src/thumbnail_creator.cpp
        src/png_encoder.cpp
        src/png_encoder.cpp
        src/rgb_converter.cpp
        include/rgb_converter.h
)

add_library(ColPicCore STATIC ${SOURCE_FILES})
target_link_libraries(ColPicCore PUBLIC "-static" ColPicLib spng::spng_static avir::avir)
target_include_directories(ColPicCore PUBLIC include)
