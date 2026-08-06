# The Qt UI adapter: a humble shell around viewer_core. Built pragmatically
# (not via TDD) per AGENTS.md; all logic lives in the core.
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

set(CMAKE_AUTOMOC ON)

# WIN32 gives a GUI subsystem (no console window) on Windows; ignored elsewhere.
add_executable(viewer_app WIN32
    main.cpp
    MainWindow.cpp
    ViewportWidget.cpp
    InfoPanel.cpp
    QtView.cpp
    FileModelSource.cpp
)

target_link_libraries(viewer_app PRIVATE
    viewer_core
    Qt6::Widgets
    Qt6::OpenGLWidgets
)

# Windows: copy the Qt DLLs and platform plugin next to the executable after
# each build (Qt's own deployment tool), so viewer_app runs from anywhere with
# no PATH setup. Linux resolves shared libraries via the loader instead.
if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${Qt6_DIR}/../../../bin")
    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET viewer_app POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}" --no-translations "$<TARGET_FILE:viewer_app>"
            COMMENT "Deploying Qt runtime next to viewer_app")
    endif()
endif()
