cmake_minimum_required(VERSION 3.22)

project(Scomeotrope VERSION 0.1.0)

include(CheckIPOSupported)

set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ON CACHE INTERNAL "")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(SCOMEOTROPE_COPY_AFTER_BUILD "Copy plugin to system dirs after build" ON)
option(SCOMEOTROPE_USE_MARCH_NATIVE "Use -march=native (disable for distributable builds)" ON)
option(SCOMEOTROPE_ENABLE_IPO "Enable interprocedural optimization/LTO in Release" ON)
option(SCOMEOTROPE_ENABLE_CODEGEN "Run Faust codegen (requires faust on PATH)" ON)
option(SCOMEOTROPE_ENABLE_VST2 "Enable legacy VST2 builds when SDK is available" OFF)
set(SCOMEOTROPE_FORMATS "VST VST3 AU Standalone" CACHE STRING "Space-separated list of plugin formats to build")

if(SCOMEOTROPE_ENABLE_VST2)
    set(VST2_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/vst-2.4-sdk/vstsdk2.4" CACHE PATH "Path to VST2 SDK")
    if(EXISTS "${VST2_SDK_PATH}/pluginterfaces/vst2.x/aeffect.h")
        message(STATUS "VST2 SDK found at: ${VST2_SDK_PATH}")
    else()
        message(FATAL_ERROR "SCOMEOTROPE_ENABLE_VST2=ON but VST2 SDK not found at: ${VST2_SDK_PATH}")
    endif()
endif()
include(FetchContent)
FetchContent_Declare(
    JUCE
    URL https://github.com/juce-framework/JUCE/archive/refs/tags/7.0.12.tar.gz
)
FetchContent_MakeAvailable(JUCE)

if(SCOMEOTROPE_ENABLE_VST2)
    juce_set_vst2_sdk_path("${VST2_SDK_PATH}")
endif()
set(FAUST_GEN_DIR "${CMAKE_SOURCE_DIR}/src/dsp/generated")

if(SCOMEOTROPE_ENABLE_CODEGEN)
    find_program(FAUST_EXECUTABLE faust)
    if(FAUST_EXECUTABLE)
        message(STATUS "Faust found: ${FAUST_EXECUTABLE} - codegen enabled")

        set(FAUST_DSP "${CMAKE_SOURCE_DIR}/dsp/scomeotrope.dsp")
        set(FAUST_CODEGEN "${CMAKE_SOURCE_DIR}/scripts/codegen.py")

        set(FAUST_GENERATED_FILES
            ${FAUST_GEN_DIR}/FaustDefs.h
            ${FAUST_GEN_DIR}/FaustDSP.h
            ${FAUST_GEN_DIR}/FaustParams.h
            ${FAUST_GEN_DIR}/FaustBridge.h
        )

        add_custom_command(
            OUTPUT ${FAUST_GENERATED_FILES}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${FAUST_GEN_DIR}
            COMMAND python3 ${FAUST_CODEGEN} ${FAUST_DSP} --output ${FAUST_GEN_DIR}
            DEPENDS ${FAUST_DSP} ${FAUST_CODEGEN}
            COMMENT "Generating Faust DSP and parameter bridge from scomeotrope.dsp..."
            VERBATIM
        )

        add_custom_target(faust_codegen DEPENDS ${FAUST_GENERATED_FILES})
    else()
        message(STATUS "Faust not found - codegen disabled, using committed generated files")
    endif()
else()
    message(STATUS "Faust codegen disabled (SCOMEOTROPE_ENABLE_CODEGEN=OFF)")
endif()
separate_arguments(SCOMEOTROPE_FORMATS_LIST UNIX_COMMAND "${SCOMEOTROPE_FORMATS}")

if(NOT SCOMEOTROPE_ENABLE_VST2)
    list(REMOVE_ITEM SCOMEOTROPE_FORMATS_LIST VST)
endif()
list(REMOVE_DUPLICATES SCOMEOTROPE_FORMATS_LIST)

juce_add_plugin(Scomeotrope
    COMPANY_NAME "SeedyROM"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD ${SCOMEOTROPE_COPY_AFTER_BUILD}
    PLUGIN_MANUFACTURER_CODE Seed
    PLUGIN_CODE Scom
    FORMATS ${SCOMEOTROPE_FORMATS_LIST}
    PRODUCT_NAME "Scomeotrope"
)

if(TARGET faust_codegen)
    add_dependencies(Scomeotrope faust_codegen)
endif()
if(SCOMEOTROPE_ENABLE_IPO)
    check_ipo_supported(RESULT PLUGIN_IPO_SUPPORTED OUTPUT PLUGIN_IPO_ERROR)
    if(PLUGIN_IPO_SUPPORTED)
        set_property(TARGET Scomeotrope PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
        message(STATUS "IPO/LTO enabled for Release builds")
    else()
        message(STATUS "IPO/LTO unavailable: ${PLUGIN_IPO_ERROR}")
    endif()
endif()

juce_add_binary_data(ScomeotropeAssets SOURCES
    assets/images/logo.png
)

target_sources(Scomeotrope PRIVATE
    src/PluginProcessor.cpp
    src/PluginEditor.cpp
    src/components/brand/TopBar.cpp
    src/components/meters/LedMeter.cpp
    src/presets/PluginPresetManager.cpp
    src/components/controls/RotaryKnob.cpp
    src/ui/PluginLookAndFeel.cpp
    )

target_include_directories(Scomeotrope PRIVATE
    ${FAUST_GEN_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_compile_definitions(Scomeotrope PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(Scomeotrope PRIVATE
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-ffast-math>
        $<$<CONFIG:Release>:-ffp-contract=fast>
        $<$<CONFIG:Release>:-fno-math-errno>
        $<$<CONFIG:Release>:-funroll-loops>
    )

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
        if(SCOMEOTROPE_USE_MARCH_NATIVE)
            target_compile_options(Scomeotrope PRIVATE
                $<$<CONFIG:Release>:-march=native>
            )
            message(STATUS "Target: x86_64 (using -march=native for AVX2/FMA)")
        else()
            target_compile_options(Scomeotrope PRIVATE
                $<$<CONFIG:Release>:-msse4.2>
            )
            message(STATUS "Target: x86_64 (using SSE4.2 baseline for distribution)")
        endif()
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(Scomeotrope PRIVATE
            $<$<CONFIG:Release>:-fdenormal-fp-math=positive-zero>
        )
    endif()
elseif(MSVC)
    target_compile_options(Scomeotrope PRIVATE
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:Release>:/fp:fast>
        $<$<CONFIG:Release>:/Oi>
        $<$<CONFIG:Release>:/Gy>
        $<$<CONFIG:Release>:/Gw>
    )
    if(SCOMEOTROPE_USE_MARCH_NATIVE)
        target_compile_options(Scomeotrope PRIVATE
            $<$<CONFIG:Release>:/arch:AVX2>
        )
    endif()
endif()

target_link_libraries(Scomeotrope PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_graphics
    juce::juce_dsp
    ScomeotropeAssets
PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
