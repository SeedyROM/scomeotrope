// Scomeotrope — Tape Channel Emulation
// Edit this file, then run `just codegen` to regenerate
// the JUCE parameter bridge in src/dsp/generated/.
//
// Models a vintage tape channel with:
//   - Sidechain-modulated tape noise (hiss tracks low-frequency energy)
//   - Feed-forward compressor with parallel dry/wet mix
//   - 12AX7 triode saturation stage (asymmetric soft-clip)
//   - Vibrato via modulated delay line (wow/flutter character)
//   - High-frequency flutter from a secondary faster LFO
//   - Master output gain with a final saturation pass
//
// Signal flow (stereo):
//   Input L/R
//     -> [noise mix]        add tape hiss, modulated by bass sidechain
//     -> [compressor]       dynamics section (FFcompressor)
//     -> [tube saturation]  12AX7 triode waveshaper
//     -> [vibrato]          slow LFO-modulated delay (wow)
//     -> [flutter]          fast fixed-rate LFO delay (flutter)
//     -> [output gain]      final level trim
//     -> [tube saturation]  second pass for tape-head warmth

import("stdfaust.lib");

//==========================================================================
// Modulated Delay Line
//==========================================================================
// Fractional delay line used for both vibrato and flutter.
// The LFO signal biases the read pointer around the centre of the
// delay buffer. At zero modulation the delay is exactly half the
// buffer length (transparent). As modulation increases the read
// pointer sweeps further, producing pitch wobble.
//
// Parameters:
//   lfo        — bipolar modulation signal (-1..+1), typically from os.osc
//   lfo_amount — depth of modulation (0..1), scales the sweep range
//
// Internal constants:
//   max_delay     — buffer length in samples (512 ≈ 11.6ms @ 44.1kHz)
//   half_max_delay — centre tap, the neutral delay with no modulation

mod_delay_line(lfo, lfo_amount) = de.fdelay5(max_delay, delay_tap)
with {
    max_delay = 512;
    half_max_delay = max_delay / 2;

    // The delay tap oscillates around the centre point.
    // lfo is bipolar (-1..+1), so the sweep is symmetric.
    // lfo_amount * half_max_delay sets the maximum deviation in samples.
    delay_tap = half_max_delay + (lfo * (lfo_amount * half_max_delay));
};

//==========================================================================
// Vibrato (Machine Decay)
//==========================================================================
// Slow pitch modulation simulating tape speed irregularities (wow).
// A low-frequency sine oscillator drives the modulated delay line.
// The dry/wet mixer uses constant-power crossfade to avoid volume dips.
//
// Parameters:
//   Speed  — LFO rate in Hz (0.0001–13Hz). Low values = gentle wow,
//            high values = Leslie-style vibrato.
//   Amount — LFO depth (0–1). How far the pitch deviates.
//   Mix    — Dry/wet blend (0 = fully dry, 1 = fully wet).
//   Bypass — When checked, disables the effect (mix forced to 0).

vibrato = hgroup("Machine Decay", out)
with {
    lfo_freq   = vslider("[0][id:vibrato_speed] Speed", 1.0, 0.0001, 13.0, 0.01)
                 : si.smooth(0.99);
    lfo_amount = vslider("[1][id:vibrato_amount] Amount", 0.5, 0.00, 1.0, 0.01)
                 : si.smooth(0.99);
    dry_wet    = vslider("[2][id:vibrato_mix] Mix", 1.0, 0.0, 1.0, 0.01)
                 : si.smooth(0.999);
    bypass     = 1 - checkbox("[3][id:vibrato_bypass] Bypass");

    // Sine LFO — smooth pitch wobble, no harmonics
    lfo = os.osc(lfo_freq);

    // Constant-power crossfade preserves perceived loudness across the
    // entire wet/dry range (unlike linear crossfade which dips at 50%).
    // bypass multiplies dry_wet so checked bypass = 0 = fully dry.
    out = ef.dryWetMixerConstantPower(dry_wet * bypass, mod_delay_line(lfo, lfo_amount));
};

//==========================================================================
// Compressor (Dynamics)
//==========================================================================
// Feed-forward compressor based on Faust's FFcompressor_N_chan.
// Models the dynamics section of a tape channel strip — smooths
// transients and adds sustain before the signal hits the tape
// (saturation stage).
//
// Parameters:
//   Strength  — compression ratio/amount (0–1.5)
//   Threshold — level above which compression engages (dB)
//   Knee      — soft-knee width (dB). Wider = more gradual onset.
//   Attack    — how fast the compressor clamps down (seconds)
//   Release   — how fast the compressor lets go (seconds)
//   Mix       — parallel compression blend (0 = dry, 1 = fully compressed)
//   Make Up   — post-compression gain to restore level (dB, converted to linear)
//   GR meter  — output-only bargraph showing gain reduction in dB

compressor = hgroup("Dynamics", out)
with {
    strength = vslider("[0][id:comp_strength] Strength", 0.5, 0.0, 1.5, 0.001);
    thresh   = vslider("[1][id:comp_threshold][unit:dB] Threshold", -39.89, -90, 0, 0.01);
    knee     = vslider("[2][id:comp_knee][unit:dB] Knee", 8.0, 0.0, 40, 0.01);
    att      = vslider("[3][id:comp_attack][unit:sec] Attack", 0.021, 0.005, 0.5, 0.0001);
    rel      = vslider("[4][id:comp_release][unit:sec] Release", 0.022, 0.005, 0.10, 0.0001);
    dry_wet  = vslider("[5][id:comp_mix] Mix", 1.0, 0.0, 1.0, 0.0001);

    // Make-up gain compensates for the level reduction caused by compression.
    // Stored in dB on the UI, converted to a linear multiplier for the audio path.
    make_up_gain = vslider("[6][id:comp_makeup][unit:dB] Make Up", 16.65, 0.1, 32.0, 0.01)
                   : ba.db2linear;

    // Gain-reduction meter: taps the compressor's internal GR signal.
    // The 20 - vbargraph trick offsets the display so 0dB = no reduction,
    // negative values = active compression. `attach` passes audio through
    // while routing the metering value to the UI.
    meter = _ <: (_, 20 - vbargraph("[7][id:comp_gr][unit:dB][tooltip: gain reduction in dB] GR", -20, 0) : attach);

    // FFcompressor_N_chan: feed-forward compressor with configurable
    // strength, threshold, attack, release, knee, and pre-filter.
    // Arguments: strength, thresh, att, rel, knee, prefilter, link, meter, N_channels
    //   prefilter = 1: enables RMS-like pre-filter for smoother detection
    //   link = 0.888: stereo link factor (0 = dual-mono, 1 = fully linked)
    //   2: number of channels (stereo)
    eff = co.FFcompressor_N_chan(strength, thresh, att, rel, knee, 1, 0.888, meter, 2);

    // Constant-power dry/wet followed by make-up gain on both channels.
    out = ef.dryWetMixerConstantPower(dry_wet, eff)
          : (_ * make_up_gain, _ * make_up_gain);
};

//==========================================================================
// Tube Saturation Stage
//==========================================================================
// Models a 12AX7 triode stage with:
//   1. Asymmetric soft-clip transfer function:
//      - Positive swing (grid conduction): clips harder (k=0.6)
//      - Negative swing (cutoff region): clips gently (k=0.15)
//      - Produces 2nd-harmonic-dominant spectrum (H2 ~4dB above H3)
//   2. Feedback lowpass for frequency-dependent saturation:
//      - Lows pass through cleaner (feedback reduces net drive)
//      - Highs get more harmonic content (feedback attenuated)
//      - Models plate impedance / coupling cap interaction
//   3. DC blocker to remove asymmetry-induced offset
//
// This replaces a simple ma.tanh waveshaper with a more harmonically
// complex and frequency-aware saturation, giving the tape channel a
// warmer, more "analog" second-harmonic character.

// Asymmetric waveshaper: rational soft-clip with different
// coefficients for positive (grid conduction) and negative (cutoff).
// The rational form x/(1+k*|x|) is cheaper than tanh and gives
// controllable asymmetry by using different k values per polarity.
triode_clip(x) = clipped / drive
with {
    drive = 1.15;   // subtle overdrive — this is a channel strip, not a guitar amp
    k_pos = 0.6;    // harder clip on positive (grid conduction region)
    k_neg = 0.15;   // gentle clip on negative (cutoff region)
    xd = x * drive;

    // select2(condition, false_branch, true_branch):
    //   xd >= 0 → positive branch: soft clip toward ceiling
    //   xd <  0 → negative branch: xd<0, so -k_neg*xd is positive (denominator > 1)
    clipped = select2(xd >= 0,
        xd / (1.0 - k_neg * xd),    // negative half-wave
        xd / (1.0 + k_pos * xd));   // positive half-wave
};

// Tube stage with feedback lowpass.
// A one-pole lowpass filter in the feedback path creates frequency-
// dependent saturation: low frequencies see reduced net drive (the
// feedback subtracts from the input, so lows pass through cleaner)
// while highs see full drive (feedback is attenuated above the
// cutoff, so they get the full nonlinearity). This models the
// plate impedance / coupling capacitor interaction in a real triode.
//
// Parameters:
//   x — input signal
//
// Internal:
//   fb_pole  — one-pole coefficient for ~1kHz cutoff
//   fb_lp    — lowpass-filtered output (feedback signal)
//   tube_out — waveshaped output with DC offset removed
tube_stage(x) = tube_out
letrec {
    // Feedback lowpass: tracks the output with a ~1kHz time constant.
    // fb_pole = e^(-2*pi*fc/SR) gives the one-pole coefficient.
    'fb_lp = fb_lp * fb_pole + tube_out * (1.0 - fb_pole)
    with {
        fb_pole = exp(-2.0 * ma.PI * 1000.0 / ma.SR);
    };

    // The waveshaper sees (input - 0.3 * lowpassed_output).
    // At low frequencies fb_lp tracks tube_out closely, so 30% is
    // subtracted → reduced effective drive → cleaner lows.
    // At high frequencies fb_lp can't follow → less subtraction →
    // full drive → more harmonics on highs.
    // DC blocker removes any offset from the asymmetric clipping.
    'tube_out = triode_clip(x - 0.3 * fb_lp) : fi.dcblocker;
};

//==========================================================================
// Tape Noise (Sidechain-Modulated)
//==========================================================================
// Models tape hiss that responds to the signal's low-frequency content,
// mimicking real tape behaviour where head magnetization from bass
// energy modulates the noise floor.
//
// Two noise sources are blended:
//   1. Pink noise  — broad-spectrum hiss with -3dB/octave rolloff,
//                    lowpassed at 3kHz to remove harsh highs. This is
//                    the steady-state tape hiss.
//   2. Sparse noise — irregular crackle/pop texture from randomly-timed
//                     impulses (sparse_noise), lowpassed at 1.5kHz.
//                     Models oxide irregularities and micro-dropouts.
//
// The sidechain envelope follower extracts bass energy (below ~300Hz)
// from the input signal and uses it to modulate the noise amplitude.
// Louder bass = more noise, just like a real tape machine where
// higher magnetization levels raise the noise floor.
//
// Parameters:
//   noise_amount — master noise level (0–0.5)
//
// Sidechain internals:
//   sc_lp        — 2nd-order Butterworth lowpass at 300Hz, extracts bass
//   sc_env       — envelope follower with ~50ms attack, ~200ms release
//   noise_mod    — final modulation factor (base level + sidechain boost)

// Sidechain envelope follower for bass energy detection.
// Extracts energy below ~300Hz and smooths it into a slow envelope.
// Attack is fast enough to catch kick drums; release is slow enough
// to provide a smooth, natural noise modulation without pumping.
sc_envelope(x) = sc_env
letrec {
    'sc_env = sc_env + (target - sc_env) * (1.0 - pole)
    with {
        // 2nd-order lowpass at 300Hz isolates bass/low-mid energy.
        // abs() rectifies; this is the instantaneous bass amplitude.
        target = fi.lowpass(2, 300, x) : abs;

        // Asymmetric attack/release: fast enough to track kick transients
        // (~50ms), slow enough release (~200ms) to avoid noise pumping.
        is_attack = (target > sc_env);
        att_pole = ba.tau2pole(0.050);   // 50ms attack
        rel_pole = ba.tau2pole(0.200);   // 200ms release
        pole = select2(is_attack, rel_pole, att_pole);
    };
};

// The noise injection function. Takes a mono input signal, adds
// noise modulated by the bass sidechain, returns the noisy signal.
// Pink noise is added, sparse noise is subtracted, creating a more
// complex and realistic hiss texture than simple white noise.
noise_amount = vslider("[1][id:noise_amount] Noise", 0.15, 0.0, 0.5, 0.0001);

// Pink noise source: -3dB/octave spectrum, lowpassed to remove
// frequencies above 3kHz (tape hiss rolls off in the highs).
// Scaled to 0.05 base amplitude before the noise_amount control.
pnoise = no.pink_noise * 0.05 : fi.lowpass(1, 3000);

// Sparse noise source: random impulse train with density modulated
// by white noise (0.3 + noise*2.0 gives ~0.3–2.3 impulses/sec).
// 2nd-order lowpass at 1.5kHz softens the impulses into gentle bumps.
// Models oxide irregularities and micro-dropouts on aged tape.
snoise = no.sparse_noise(0.3 + no.noise * 2.0) : fi.lowpass(2, 1500) * 0.05;

// Noise injection with sidechain modulation.
// The sidechain envelope scales the noise amplitude: quiet input →
// minimal noise (just the base 0.3 floor), loud bass → up to 1.3x
// the set noise amount. This replicates how tape hiss rises with
// magnetization level.
noise(x) = x + (pnoise * mod_level) - (snoise * mod_level)
with {
    // Bass envelope from the input signal (0..~1 range)
    bass_env = sc_envelope(x);

    // Modulation factor: base noise floor (0.3) + sidechain contribution.
    // The sidechain adds up to 1.0 on top of the base, so the total
    // range is 0.3–1.3 × noise_amount. This ensures some hiss is
    // always present (like real tape at rest) but it breathes with
    // the signal.
    noise_mod = 0.3 + min(bass_env, 1.0);

    // Final noise level: user control × sidechain modulation
    mod_level = noise_amount * noise_mod;
};

//==========================================================================
// Flutter
//==========================================================================
// High-frequency tape speed irregularity.
// While vibrato models slow wow (< 13Hz), flutter models the faster
// mechanical jitter from capstan eccentricity and reel tension
// variations, typically around 15–25Hz.
//
// Uses the same mod_delay_line as vibrato but with a fixed 17Hz LFO
// and a separate depth control. The 17Hz rate sits in the typical
// flutter range for consumer-grade tape machines.
//
// Parameters:
//   flutter_amount — depth of the flutter modulation (0–0.032)

flutter_amount = vslider("[2][id:flutter] Flutter", 0.010, 0.0, 0.032, 0.001);

// Fixed 17Hz sine LFO driving a modulated delay line.
// The small max depth (0.032) keeps the pitch deviation subtle —
// flutter should be felt more than heard.
flutter = mod_delay_line(os.osc(17), flutter_amount);

//==========================================================================
// Output Gain
//==========================================================================
// Final level trim before the second saturation pass.
// Allows the user to push the signal harder into the final tube stage
// for more harmonic content, or back off for a cleaner output.
//
// Range 1.0–12.0 (linear gain, roughly 0dB to +21.6dB).

output_gain = vslider("[3][id:output_gain] Output Gain", 1.6, 1.0, 12.0, 0.001);

//==========================================================================
// Effect Chain
//==========================================================================
// The complete stereo tape channel effect chain.
// Each stage is applied to both L and R channels in parallel.
//
// Order rationale:
//   1. Noise — added first so it gets compressed/saturated with the signal
//              (like real tape where noise is inherent to the medium)
//   2. Compressor — dynamics processing smooths transients before saturation
//   3. Tube saturation (1st pass) — warm harmonic generation
//   4. Vibrato — pitch modulation (post-saturation so harmonics wobble too)
//   5. Flutter — subtle high-rate pitch jitter
//   6. Output gain — level trim before final saturation
//   7. Tube saturation (2nd pass) — catches any peaks from gain staging,
//      adds cumulative warmth (like multiple gain stages in a console)

eff = (noise, noise)
    : compressor
    : (tube_stage, tube_stage)
    : (vibrato, vibrato)
    : (flutter, flutter)
    : (_ * output_gain, _ * output_gain)
    : (tube_stage, tube_stage);

//==========================================================================
// Process
//==========================================================================
// Top-level stereo process block.
// The hgroup wraps the entire UI for the host to display as
// "CHL03: Tape Channel" (the plugin's display name).
// Bypass is handled by the DAW host, so we just wire straight through.

process = _, _ : hgroup("CHL03: Tape Channel", eff);
