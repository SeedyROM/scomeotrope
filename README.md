# Scomeotrope

A lo-fi saturation plugin

## About

- **Author**: Zack Kollar (me@seedyrom.io)
- **Company**: SeedyROM
- **Formats**: VST2, VST3, AU, Standalone

## Architecture Included

- **Preset + A/B state architecture** is included by default.
- **Preset storage path**: `~/Library/Application Support/SeedyROM/Scomeotrope/Presets` (platform equivalent).
- **Preset extension**: `.scomeotropepreset`.
- **Advanced UI profile**: `enabled`.

## Theming

The advanced UI uses color constants (`src/ui/ScomeotropeColors.h`) and a custom look-and-feel (`src/ui/PluginLookAndFeel.*`).


## Prerequisites

- CMake 3.22+
- C++17 compiler
- [just](https://github.com/casey/just) (recommended)
- [Faust](https://faust.grame.fr/downloads/) (only needed when editing DSP)


## Build

```bash
just build
just release
just run
```

Manual:

```bash
cmake -B build -G Ninja
cmake --build build --config Release
```

## CI/Distribution Build

```bash
cmake -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DSCOMEOTROPE_COPY_AFTER_BUILD=OFF \
  -DSCOMEOTROPE_USE_MARCH_NATIVE=OFF \
  -DSCOMEOTROPE_ENABLE_CODEGEN=OFF
```

## Presets and A/B

- A/B state architecture exists in the processor backend.
- No presets are pre-populated in the advanced UI template shell.
- Preset behavior is intended to be defined by the plugin implementation.

## Advanced UI

Enabled in this generated project:

- Themed top bar with preset/A-B/options controls wired as placeholders.
- Empty content area below top bar for product-specific UI.
- Color constants and look-and-feel infrastructure ready for customization.


## Project Structure

```
scomeotrope/
├── CMakeLists.txt
├── justfile
├── src/
│   ├── PluginProcessor.h/cpp
│   ├── PluginEditor.h/cpp
│   ├── presets/
│   │   └── PluginPresetManager.h/cpp
│   ├── data/
│   │   ├── PluginParameters.h
│   │   └── RuntimeParameters.h
│   ├── components/
│   │   ├── brand/TopBar.h/cpp
│   │   └── controls/RotaryKnob.h/cpp
│   └── ui/ScomeotropeColors.h, PluginLookAndFeel.h/cpp
├── dsp/scomeotrope.dsp
├── src/dsp/generated/
├── scripts/codegen.py
└── scripts/element_dev.sh
```

## License

[Your License Here]
