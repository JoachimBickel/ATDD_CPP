cmake_minimum_required(VERSION 3.16)

project(ATDD_CPP
    VERSION 0.1.0
    DESCRIPTION "Cross-platform 3D viewer"
    LANGUAGES CXX)

# Uncomment/set the following two variables to compile in a Rechenraum/rr_dep environment
#set(Qt6_ROOT  "C:/Qt/6.8.3/msvc2022_64")
#set(BOOST_ROOT "C:/Users/Simon/Desktop/src/deps/boost/1.90.0-rr2/build-vs2026-x64-static")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory(core)
add_subdirectory(adapters/qt)
add_subdirectory(tests)
