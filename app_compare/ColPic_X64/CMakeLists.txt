add_library(ColPicLib_X64 SHARED IMPORTED GLOBAL)
set_property(TARGET ColPicLib_X64 PROPERTY
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/ColPic_X64.dll")
set_property(TARGET ColPicLib_X64 PROPERTY
        IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/ColPic_X64.lib")

target_include_directories(ColPicLib_X64 INTERFACE include)
