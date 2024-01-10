set(SOURCE_FILES
        src/main.cpp
)

add_subdirectory(ColPic_X64)

add_executable(app_compare ${SOURCE_FILES})
target_link_libraries(app_compare PRIVATE "-static" ColPicLib ColPicLib_X64)


get_property(ColPicLib_X64_dll TARGET ColPicLib_X64 PROPERTY IMPORTED_LOCATION)

add_custom_command(TARGET app_compare POST_BUILD        # Adds a post-build event to MyTest
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        ${ColPicLib_X64_dll}      # <--this is in-file
        $<TARGET_FILE_DIR:app_compare>)                 # <--this is out-file path
