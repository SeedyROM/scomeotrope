# Scomeotrope — Faust/JUCE audio plugin
# Run `just` or `just --list` to see available recipes.

build_type := "Debug"
build_dir := "build"
dsp_source := "dsp/scomeotrope.dsp"
gen_dir := "src/dsp/generated"

plugin_formats := "VST VST3 AU Standalone"
plugin_enable_vst2 := "OFF"
plugin_copy_after_build := "ON"
plugin_enable_codegen := "OFF"

default:
    @just --list

codegen:
    python3 scripts/codegen.py {{dsp_source}} --output {{gen_dir}}


configure *flags:
    @enable_vst2="{{plugin_enable_vst2}}"; formats='{{plugin_formats}}'; enable_codegen="{{plugin_enable_codegen}}"; for flag in {{flags}}; do if [ "$flag" = "--enable-vst2" ]; then enable_vst2=ON; formats="VST VST3 AU Standalone"; elif [ "$flag" = "--enable-codegen" ]; then enable_codegen=ON; else echo "Unknown flag: $flag" >&2; exit 1; fi; done; cmake -B {{build_dir}} -G Ninja -DCMAKE_BUILD_TYPE={{build_type}} -DSCOMEOTROPE_FORMATS="$formats" -DSCOMEOTROPE_COPY_AFTER_BUILD={{plugin_copy_after_build}} -DSCOMEOTROPE_USE_MARCH_NATIVE=ON -DSCOMEOTROPE_ENABLE_VST2=$enable_vst2 -DSCOMEOTROPE_ENABLE_CODEGEN=$enable_codegen

build *flags:
    @just build_type={{build_type}} plugin_formats="{{plugin_formats}}" plugin_enable_vst2={{plugin_enable_vst2}} plugin_copy_after_build={{plugin_copy_after_build}} plugin_enable_codegen={{plugin_enable_codegen}} configure {{flags}}; cmake --build {{build_dir}} --config {{build_type}}

configure-codegen: codegen
    just plugin_enable_codegen=ON configure

build-codegen: configure-codegen
    cmake --build {{build_dir}} --config {{build_type}}


release *flags:
    just build_type=Release build {{flags}}

clean:
    rm -rf {{build_dir}}

rebuild: clean build

submodules:
    git submodule update --init --recursive

setup: submodules rebuild

run *flags:
    just build_type={{build_type}} plugin_formats="{{plugin_formats}}" plugin_enable_vst2={{plugin_enable_vst2}} plugin_copy_after_build={{plugin_copy_after_build}} plugin_enable_codegen={{plugin_enable_codegen}} build {{flags}}
    open {{build_dir}}/Scomeotrope_artefacts/{{build_type}}/Standalone/Scomeotrope.app

element *flags:
    just build_type={{build_type}} plugin_formats="{{plugin_formats}}" plugin_enable_vst2={{plugin_enable_vst2}} plugin_copy_after_build={{plugin_copy_after_build}} plugin_enable_codegen={{plugin_enable_codegen}} build {{flags}}
    @bash scripts/element_dev.sh

reconfigure: clean configure build
