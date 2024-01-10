set(SOURCE_FILES
        src/main.cpp
        src/arguments.cpp
        src/gcode_handler.cpp
)

set(OUTPUT_NAME "elegoo_neptune_thumbnails_prusa_az")
include(generate_product_version)
generate_product_version(ProductVersionFiles
        NAME ${OUTPUT_NAME}
        ICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/product.ico"
        VERSION_MAJOR 1
        VERSION_MINOR 0
        VERSION_PATCH 2
        VERSION_REVISION 0
        COMPANY_NAME alezhu)

add_executable(app_converter ${SOURCE_FILES} ${ProductVersionFiles})
set_target_properties(app_converter PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})

target_link_libraries(app_converter PRIVATE "-static" ColPicLib ColPicCore mbits::args aklomp::base64 )

