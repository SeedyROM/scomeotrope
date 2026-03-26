#include "LedMeter.h"

#include <cmath>

// ============================================================================
// Segment table construction
// ============================================================================

LedMeter::LedMeter() { buildSegments(); }

void LedMeter::buildSegments() {
  // Build LED segments bottom-to-top.
  // Each segment is 2 dB wide.
  //
  // Normal mode — colour zones:
  //   -48..-18 dB  green
  //   -18..-6  dB  yellow
  //    -6..0   dB  red
  //
  // GR mode — single amber colour, range 0..-20 dB:
  //   LEDs are still stored bottom-to-top but paint() lights them
  //   from the top downward so increasing reduction fills down.
  //
  // Major labels at: -45, -30, -20, -15, -10, -6, -3, 0

  namespace C = ScomeotropeColors;

  segments.clear();

  if (grMode) {
    // GR mode: 0 dB at top (no reduction), -20 dB at bottom.
    // Segments stored bottom-to-top (most-negative first).
    const float grMinDb = -20.0f;
    const float grMaxDb = 0.0f;
    const float stepDb = 1.0f;

    auto isMajorGr = [](float db) -> bool {
      const float abs = std::fabs(db);
      return abs == 0.0f || abs == 3.0f || abs == 6.0f || abs == 10.0f ||
             abs == 15.0f || abs == 20.0f;
    };

    for (float db = grMinDb; db <= grMaxDb; db += stepDb) {
      segments.push_back({db, juce::Colour(C::meterOrange), isMajorGr(db)});
    }
  } else {
    // Normal signal-level mode
    auto isMajor = [](float db) -> bool {
      const float abs = std::fabs(db);
      return abs == 0.0f || abs == 3.0f || abs == 6.0f || abs == 10.0f ||
             abs == 15.0f || abs == 20.0f || abs == 30.0f || abs == 45.0f;
    };

    const float stepDb = 2.0f;
    for (float db = minDb; db <= maxDb; db += stepDb) {
      juce::Colour col;
      if (db >= -6.0f)
        col = juce::Colour(C::meterRed);
      else if (db >= -18.0f)
        col = juce::Colour(C::meterYellow);
      else
        col = juce::Colour(C::meterGreen);

      segments.push_back({db, col, isMajor(db)});
    }
  }
}

// ============================================================================
// Meter source & refresh
// ============================================================================

void LedMeter::setMeterSource(std::function<float()> source) {
  meterSource = std::move(source);
}

float LedMeter::peakToDb(float peak) {
  return 20.0f * std::log10(juce::jmax(peak, 1.0e-7f));
}

float LedMeter::dbToNormalized(float db) const {
  return juce::jlimit(0.0f, 1.0f, (db - minDb) / (maxDb - minDb));
}

void LedMeter::refresh() {
  if (!meterSource)
    return;

  const float rawValue = meterSource();

  // In GR mode the source already returns dB; in normal mode convert.
  const float targetDb = grMode ? rawValue : peakToDb(rawValue);

  // Ballistic smoothing: fast rise (~70ms), slower fall (~300ms)
  // At 24 Hz: rise coeff ~0.65, fall coeff ~0.12
  constexpr float riseCoeff = 0.65f;
  constexpr float fallCoeff = 0.12f;

  if (grMode) {
    // In GR mode, "rising" means more negative (more reduction).
    if (targetDb < displayPeakDb)
      displayPeakDb += (targetDb - displayPeakDb) * riseCoeff;
    else
      displayPeakDb += (targetDb - displayPeakDb) * fallCoeff;

    // Snap to ceiling (no reduction)
    if (displayPeakDb > -0.5f)
      displayPeakDb = 0.0f;
  } else {
    if (targetDb > displayPeakDb)
      displayPeakDb += (targetDb - displayPeakDb) * riseCoeff;
    else
      displayPeakDb += (targetDb - displayPeakDb) * fallCoeff;

    // Snap to floor
    if (displayPeakDb < minDb + 0.5f)
      displayPeakDb = minDb;
  }

  // Peak hold
  if (peakHoldEnabled) {
    if (grMode) {
      // In GR mode, peak hold tracks the most-negative (deepest reduction)
      if (targetDb < peakHoldDb) {
        peakHoldDb = targetDb;
        peakHoldCounter = 0;
      } else {
        ++peakHoldCounter;
        if (peakHoldCounter > peakHoldFrames) {
          peakHoldDb += 0.8f;
          if (peakHoldDb > 0.0f)
            peakHoldDb = 0.0f;
        }
      }
    } else {
      if (targetDb > peakHoldDb) {
        peakHoldDb = targetDb;
        peakHoldCounter = 0;
      } else {
        ++peakHoldCounter;
        if (peakHoldCounter > peakHoldFrames) {
          peakHoldDb -= 0.8f;
          if (peakHoldDb < minDb)
            peakHoldDb = minDb;
        }
      }
    }
  }

  repaint();
}

// ============================================================================
// Layout
// ============================================================================

void LedMeter::resized() {
  const auto bounds = getLocalBounds().toFloat();

  // Reserve left margin for dB labels
  const float labelWidth = 28.0f;
  const float topPad = 4.0f;
  const float bottomPad = 4.0f;

  stripBounds = bounds;
  stripBounds.removeFromLeft(labelWidth);
  stripBounds.removeFromTop(topPad);
  stripBounds.removeFromBottom(bottomPad);
  stripBounds.removeFromRight(2.0f);

  const int numSegs = static_cast<int>(segments.size());
  if (numSegs > 0) {
    segmentGap = 1.0f;
    segmentHeight =
        (stripBounds.getHeight() - segmentGap * (numSegs - 1)) / numSegs;
    segmentHeight = juce::jmax(segmentHeight, 2.0f);
  }
}

// ============================================================================
// Painting
// ============================================================================

void LedMeter::paint(juce::Graphics &g) {
  namespace C = ScomeotropeColors;

  // ---- Background: dark inset panel ----
  {
    const auto fb = getLocalBounds().toFloat();
    g.setColour(juce::Colour(0xff0a0a0a));
    g.fillRoundedRectangle(fb, 3.0f);
    g.setColour(juce::Colour(0xff222222));
    g.drawRoundedRectangle(fb.reduced(0.5f), 3.0f, 1.0f);
  }

  if (segments.empty() || segmentHeight < 1.0f)
    return;

  const int numSegs = static_cast<int>(segments.size());
  const float ledWidth = stripBounds.getWidth();
  const float ledCorner = juce::jmin(2.0f, segmentHeight * 0.3f);

  // ---- Draw LED segments (bottom to top) ----
  for (int i = 0; i < numSegs; ++i) {
    const auto &seg = segments[static_cast<size_t>(i)];

    // Y position: segment 0 is at the bottom
    const float y =
        stripBounds.getBottom() - (i + 1) * segmentHeight - i * segmentGap;
    const auto ledRect =
        juce::Rectangle<float>(stripBounds.getX(), y, ledWidth, segmentHeight);

    bool isLit = false;
    bool isPeakHold = false;

    if (grMode) {
      // GR mode: light from top downward.
      // displayPeakDb is negative (e.g. -10). Light segments whose
      // thresholdDb >= displayPeakDb (i.e. from 0 down to the GR depth).
      isLit = (displayPeakDb < -0.01f) && (seg.thresholdDb >= displayPeakDb);
      isPeakHold = peakHoldEnabled && !isLit && (peakHoldDb < -0.01f) &&
                   (seg.thresholdDb >= peakHoldDb) &&
                   (seg.thresholdDb <
                    displayPeakDb); // only below the current lit region
    } else {
      // Normal mode: light from bottom upward.
      isLit = (displayPeakDb > seg.thresholdDb);
      isPeakHold =
          peakHoldEnabled && !isLit && (peakHoldDb >= -40.0f) &&
          (peakHoldDb >= seg.thresholdDb) &&
          (i + 1 >= numSegs ||
           peakHoldDb < segments[static_cast<size_t>(i + 1)].thresholdDb);
    }

    if (isLit) {
      // Lit LED: full colour with a subtle glow
      g.setColour(seg.litColour);
      g.fillRoundedRectangle(ledRect, ledCorner);

      // Inner highlight for that plastic-LED shine
      g.setColour(seg.litColour.brighter(0.4f).withAlpha(0.5f));
      g.fillRoundedRectangle(ledRect.reduced(1.0f, 1.0f), ledCorner);
    } else if (isPeakHold) {
      // Peak hold indicator: dimmer version of the segment colour
      g.setColour(seg.litColour.withAlpha(0.7f));
      g.fillRoundedRectangle(ledRect, ledCorner);
    } else {
      // Unlit LED: very dark hint of the colour (like an unlit LED in daylight)
      g.setColour(seg.litColour.withAlpha(0.06f));
      g.fillRoundedRectangle(ledRect, ledCorner);
    }

    // ---- dB label (left of strip) ----
    if (seg.isMajor) {
      const float labelX = 2.0f;
      const float labelW = stripBounds.getX() - labelX - 2.0f;
      const auto labelRect = juce::Rectangle<float>(labelX, y - 1.0f, labelW,
                                                    segmentHeight + 2.0f);

      g.setColour(juce::Colour(C::labelText).withAlpha(0.6f));
      g.setFont(juce::Font(9.0f, juce::Font::plain));

      juce::String labelText;
      if (seg.thresholdDb == 0.0f)
        labelText = "0";
      else
        labelText = juce::String(juce::roundToInt(seg.thresholdDb));

      g.drawText(labelText, labelRect, juce::Justification::centredRight,
                 false);
    }
  }

  // ---- Channel label (above strip) ----
  if (channelLabel.isNotEmpty()) {
    g.setColour(juce::Colour(C::sectionTitle));
    g.setFont(juce::Font(10.0f, juce::Font::bold));
    g.drawText(channelLabel,
               stripBounds.withHeight(12.0f).translated(0.0f, -14.0f),
               juce::Justification::centred, false);
  }
}
