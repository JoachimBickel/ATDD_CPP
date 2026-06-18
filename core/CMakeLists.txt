# The Qt-free, fully testable core. Depends on nothing external (std only).
add_library(viewer_core STATIC
    src/io/ObjImporter.cpp
    src/app/ViewerService.cpp
)

# PUBLIC so every consumer (tests now, the Qt app later) gets the headers.
target_include_directories(viewer_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
