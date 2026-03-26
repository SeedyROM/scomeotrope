# Faust Codegen Pipeline

Scomeotrope uses Faust for DSP and generates JUCE bridge headers for parameter/state integration.

## Outputs

Generated into `src/dsp/generated/`:

- `FaustDefs.h`
- `FaustDSP.h`
- `FaustParams.h`
- `FaustBridge.h`

These files are committed so CI builds do not require Faust.

## Run Codegen

```bash
just codegen
```

or

```bash
python3 scripts/codegen.py dsp/scomeotrope.dsp --output src/dsp/generated
```

## Build Integration

When `SCOMEOTROPE_ENABLE_CODEGEN=ON`, CMake re-runs codegen when either:

- `dsp/scomeotrope.dsp` changes
- `scripts/codegen.py` changes

If `faust` is not installed, CMake falls back to committed generated headers.

## Performance Details in Generated Bridge

- Stable parameter IDs from explicit Faust metadata (`[id:...]`) when available.
- Cached APVTS raw parameter pointers for lower process-block overhead.
- In-place processing fast path when layout allows.
- Scratch-buffer fallback for non-ideal host channel layout.

## Compiler Flags Used for Faust C++ Generation

```bash
faust -lang cpp -cn ScomeotropeDSP -scn "" -vec -vs 32 -lv 1 -ftz 0 -mcd 0 -single -uim
```

## Post-Processing

After generating `FaustDSP.h`, the script:

1. injects `#include "FaustDefs.h"`
2. suppresses unused `sample_rate` warnings in Faust helper SIG methods
