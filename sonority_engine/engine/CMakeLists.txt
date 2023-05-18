set(BINARY sonority_engine)

set(LINK_JUCE
        juce::juce_gui_extra
        juce::juce_audio_devices
        juce::juce_audio_utils)

set(LINK_COMMON
        audio_engine)
        sonority_test_module
        sofa_renderer)

#########################################################
# SRC
#########################################################
set(SOURCES src/Sonority.h src/Sonority.cpp src/SonorityRTCallback.cpp src/SonorityRTCallback.h src/UnityNativeLogger.h src/UnityNativeLogger.cpp)

add_library(${BINARY})
target_sources(${BINARY} PRIVATE ${SOURCES})

target_link_libraries(${BINARY}
        PRIVATE ${LINK_JUCE} ${LINK_COMMON}
        PUBLIC ${MODULE_LINK_CONF})

target_compile_options(${BINARY} INTERFACE
        -Wall
        -Wextra
        -Werror)

set(COPY_SOURCE_PATH "${PROJECT_BINARY_DIR}/engine/libsonority_engine.dylib")
set(COPY_TARGET_PATH "${PROJECT_SOURCE_DIR}/../sonority_unity/Assets/Plugins/libsonority_engine.dylib")
add_custom_command(TARGET ${BINARY} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${COPY_SOURCE_PATH}
        ${COPY_TARGET_PATH})

#########################################################
# TEST
#########################################################
#set(BINARY_TST ${BINARY}_tst)
#add_executable(${BINARY_TST} ../modules/browser/src/browser/Browser.cpp ../modules/browser/src/browser/Browser.h)
#target_include_directories(${BINARY_TST} PRIVATE src)
#target_sources(${BINARY_TST} PRIVATE ${SOURCES})
#target_link_libraries(
#        ${BINARY_TST}
#        PRIVATE ${LINK_JUCE} Catch2::Catch2WithMain
#        PUBLIC ${MODULE_LINK_CONF})