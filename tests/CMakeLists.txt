# Header-only Boost.Test keeps the toolchain requirement to just Boost headers
# (no compiled Boost library to link), which is the most portable choice for a
# small cross-platform project.
find_package(Boost REQUIRED)

add_executable(viewer_tests
    test_main.cpp
)

target_include_directories(viewer_tests PRIVATE ${Boost_INCLUDE_DIRS})

add_test(NAME viewer_tests COMMAND viewer_tests)
