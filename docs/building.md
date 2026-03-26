# Building Scomeotrope

## Prerequisites

- CMake 3.22+
- C++17 compiler (Clang/GCC/MSVC)
- Ninja (recommended)
- just (recommended)
- Faust (only needed when changing DSP)


## Quick Start

```bash
just build
just release
just run
```

## CMake Options

| Option | Default | Description |
|---|---|---|
| `SCOMEOTROPE_COPY_AFTER_BUILD` | `ON` | Copy plugin to system directories |
| `SCOMEOTROPE_USE_MARCH_NATIVE` | `ON` | Machine-specific optimization for local builds |
| `SCOMEOTROPE_ENABLE_IPO` | `ON` | Enable LTO/IPO in Release when available |
| `SCOMEOTROPE_ENABLE_CODEGEN` | `ON` | Run Faust codegen if `faust` is on PATH |
| `SCOMEOTROPE_ENABLE_VST2` | `OFF` | Enable VST2 if SDK checkout exists |
| `SCOMEOTROPE_FORMATS` | format list | Space-separated plugin formats |

## CI / Distributable Builds

```bash
cmake -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DSCOMEOTROPE_COPY_AFTER_BUILD=OFF \
  -DSCOMEOTROPE_USE_MARCH_NATIVE=OFF \
  -DSCOMEOTROPE_ENABLE_CODEGEN=OFF

cmake --build build --config Release --parallel
```

## Optimization Notes

- Clang/GCC: `-O3 -ffast-math -ffp-contract=fast -fno-math-errno -funroll-loops`
- x86_64 local: `-march=native`
- x86_64 CI/distribution: `-msse4.2`
- Clang denormal handling: `-fdenormal-fp-math=positive-zero`
- MSVC: `/O2 /fp:fast` (+ `/arch:AVX2` when enabled)

## CI Pipeline

The generated GitHub workflow builds:

- macOS universal
- macOS legacy x86_64
- Windows
- Linux (+ pluginval VST3 validation)

It also supports:

- `workflow_dispatch` release runs
- draft release packaging for `v*` tags
- tag verification/creation for manual release dispatch
