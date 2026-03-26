#pragma once

#include "PluginProcessor.h"
#include "components/brand/TopBar.h"
#include "components/controls/PowerButton.h"
#include "components/controls/RotaryKnob.h"
#include "components/meters/LedMeter.h"

#include "data/RuntimeParameters.h"
#include "ui/PluginLookAndFeel.h"
#include <juce_audio_processors/juce_audio_processors.h>

class ScomeotropeAudioProcessorEditor : public juce::AudioProcessorEditor
, private juce::Timer
{
public:
  ScomeotropeAudioProcessorEditor(ScomeotropeAudioProcessor &);
  ~ScomeotropeAudioProcessorEditor() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  void timerCallback() override;
  void showOptionsMenu();
  void promptSavePreset();
  void refreshPresetControls();

  void drawSectionPanel(juce::Graphics &g, juce::Rectangle<int> bounds,
                        const juce::String &title);

  static constexpr int minEditorWidth = 760;
  static constexpr int minEditorHeight = 620;
  ScomeotropeAudioProcessor &audioProcessor;

  PluginLookAndFeel pluginLnf;
  TopBar topBar;

  // --- Dynamics controls ---
  RotaryKnob strengthKnob;
  RotaryKnob thresholdKnob;
  RotaryKnob kneeKnob;
  RotaryKnob attackKnob;
  RotaryKnob releaseKnob;
  RotaryKnob compMixKnob;
  RotaryKnob makeupKnob;

  // --- LED VU meters (input / output) ---
  LedMeter inputMeterL;
  LedMeter inputMeterR;
  LedMeter outputMeterL;
  LedMeter outputMeterR;

  // --- Machine Decay (vibrato) controls ---
  RotaryKnob speedKnob;
  RotaryKnob amountKnob;
  RotaryKnob vibratoMixKnob;
  PowerButton vibratoBypassButton;
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>
      vibratoBypassAttachment;

  // --- Tape Character controls ---
  RotaryKnob noiseKnob;
  RotaryKnob flutterKnob;
  RotaryKnob outputGainKnob;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScomeotropeAudioProcessorEditor)
};
