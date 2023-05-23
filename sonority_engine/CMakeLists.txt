cmake_minimum_required(VERSION 3.20)
project(sonority VERSION 1.0)

include(cmake/update_from_git.cmake)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CATCH_INSTALL_DOCS OFF)
set(CATCH_INSTALL_EXTRAS OFF)
set(BUILD_TESTING OFF)

include(cmake/update_from_git.cmake)
include(cmake/sonority_module_common.cmake)

update_from_git(juce "https://github.com/juce-framework/JUCE"
        "2b16c1b94c90d0db3072f6dc9da481a9484d0435")
add_subdirectory(${juce_SOURCE_DIR})

update_from_git(catch2 "https://github.com/catchorg/Catch2"
        "97c48e0c343d26d50764fafdc90b1e630fbd10ce")
add_subdirectory(${catch2_SOURCE_DIR})

update_from_git(choc "https://github.com/Tracktion/choc"
        "92401a359d2cbb631d09b33352f62377f58a3878")
include_directories(${choc_SOURCE_DIR})

update_from_git(libmysofa "https://github.com/hoene/libmysofa"
        "890400ebd092c574707d0c132124f8ff047e20e1")
set(BUILD_TESTS false)
add_subdirectory(${libmysofa_SOURCE_DIR})
include_directories(${libmysofa_SOURCE_DIR}/src)

add_library(common INTERFACE modules/sofa_renderer/src/sofa_renderer/SofaDodecRenderer.cpp modules/sofa_renderer/src/sofa_renderer/SofaDodecRenderer.h modules/ambisonic_processor/src/ambisonic_processor/AmbisonicEncoder.cpp modules/ambisonic_processor/src/ambisonic_processor/AmbisonicEncoder.h)
target_link_libraries(common INTERFACE mysofa-static)
link_libraries(common)

add_subdirectory(modules)
add_subdirectory(engine)