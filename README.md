# Scomeotrope

Lo-fi tape channel emulation.

[![Build](https://github.com/SeedyROM/scomeotrope/actions/workflows/build.yml/badge.svg)](https://github.com/SeedyROM/scomeotrope/actions/workflows/build.yml)

<img width="759" height="620" alt="Screenshot 2026-03-25 at 11 34 02 PM" src="https://github.com/user-attachments/assets/0f01a859-209a-49b9-940c-c6ee74925cb8" />
<br />
<br />

Scomeotrope is a stereo tape channel plugin that layers sidechain-modulated tape hiss, a feed-forward compressor with parallel dry/wet mix, 12AX7 triode saturation, and modulated delay lines for wow and flutter. It includes front-panel controls (Input Drive, Noise, Compressor Threshold/Ratio/Mix, Vibrato Rate/Depth, Flutter, Tube Drive, Output Gain) and produces the warm, unstable character of a well-worn tape path. The DSP is written in [Faust](https://faust.grame.fr/) and the UI is built with [JUCE](https://juce.com/).

The tape noise model uses a bass-frequency sidechain envelope to modulate hiss level, so the noise floor rises and falls with low-frequency program material — mimicking the behavior of a real tape transport where head contact pressure and bias interact with signal level.

## Platforms & Formats

| Platform | Architectures | Formats |
|---|---|---|
| macOS | Universal (arm64 + x86_64) | VST3, AU, Standalone |
| macOS Legacy | x86_64 (10.13+) | VST3, AU, Standalone |
| Windows | x86_64 | VST3, Standalone |
| Linux | x86_64 | VST3, Standalone |

## Downloads

Grab the latest build from [GitHub Releases](https://github.com/SeedyROM/scomeotrope/releases). CI builds run on every push to `main` and pull request. When a `v*` tag is pushed, a draft release is created automatically with per-platform zips (macOS Universal, macOS Legacy, Windows, Linux).

## Building

Requires CMake 3.22+ and a C++17 compiler. JUCE is fetched automatically during configure.

```bash
cmake -B build -G Ninja
cmake --build build --config Release
```

See [docs/building.md](docs/building.md) for platform-specific prerequisites, CMake options, and the `just` task runner.

## Faust DSP

The tape channel DSP lives in `dsp/scomeotrope.dsp`. A codegen script compiles the Faust source to C++ and generates a bridge layer that maps Faust parameters to JUCE's `AudioProcessorValueTreeState`. The generated files are committed to git, so you don't need Faust installed for normal builds; Faust is only required when you intentionally regenerate the DSP outputs.

See [docs/faust-codegen.md](docs/faust-codegen.md) for details on the codegen pipeline and how to modify the DSP.

## License

[AGPLv3](LICENSE)
