#pragma once

#include "ui/ScomeotropeColors.h"
#include <juce_gui_basics/juce_gui_basics.h>

#include <functional>

/**
 * Late-70s / early-80s style vertical LED ladder meter.
 *
 * A stack of small rounded-rect LEDs that light up from bottom to top:
 *   - Green:  normal levels  (-45 to -18 dB)
 *   - Yellow: warm zone      (-18 to  -6 dB)
 *   - Red:    hot / clip     ( -6 to   0 dB)
 *
 * Driven by a meter source function returning linear peak amplitude.
 * Call refresh() at ~24 Hz from the editor timer.
 *
 * dB labels are painted alongside each major segment.
 */
class LedMeter : public juce::Component {
public:
  LedMeter();
  ~LedMeter() override = default;

  void paint(juce::Graphics &g) override;
  void resized() override;

  /** Supply a function that returns linear peak amplitude (0..1+). */
  void setMeterSource(std::function<float()> source);

  /** Call at ~24 Hz to update the meter with ballistic smoothing. */
  void refresh();

  /** Optional: show a peak-hold indicator. Default = true. */
  void setPeakHoldEnabled(bool enabled) { peakHoldEnabled = enabled; }

  /** Optional: set the label to draw above the meter strip (e.g. "L", "R"). */
  void setChannelLabel(const juce::String &label) { channelLabel = label; }

  /**
   * Enable gain-reduction meter mode.
   *
   * In GR mode the meter source returns dB directly (0 = no reduction,
   * negative = more reduction). LEDs light from the top downward in a
   * single amber colour, and the linear-to-dB conversion is skipped.
   */
  void setGainReductionMode(bool enabled) { grMode = enabled; if (enabled) { setPeakHoldEnabled(false); buildSegments(); } }

private:
  // ---- Segment definition ----
  struct LedSegment {
    float thresholdDb; // dB value this segment represents
    juce::Colour litColour;
    bool isMajor; // draw a dB label next to it
  };

  std::vector<LedSegment> segments; // bottom-to-top order (most negative first)

  void buildSegments();

  // ---- State ----
  std::function<float()> meterSource;
  float displayPeakDb = -100.0f; // smoothed display value in dB
  float peakHoldDb = -100.0f;    // peak hold value in dB
  int peakHoldCounter = 0;       // frames since last peak hold update

  bool peakHoldEnabled = true;
  bool grMode = false;
  juce::String channelLabel;

  // ---- Layout cache ----
  juce::Rectangle<float> stripBounds; // the actual LED strip area
  float segmentHeight = 0.0f;
  float segmentGap = 1.0f;

  // ---- Constants ----
  static constexpr float minDb = -48.0f;
  static constexpr float maxDb = 0.0f;
  static constexpr int peakHoldFrames = 48; // ~2 seconds at 24Hz

  // ---- Helpers ----
  static float peakToDb(float peak);
  float dbToNormalized(float db) const;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LedMeter)
};
