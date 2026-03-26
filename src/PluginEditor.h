#pragma once

#include "PluginProcessor.h"
#include "components/brand/TopBar.h"

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

  static constexpr int minEditorWidth = 760;
  static constexpr int minEditorHeight = 520;
ScomeotropeAudioProcessor &audioProcessor;

PluginLookAndFeel pluginLnf;
TopBar topBar;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScomeotropeAudioProcessorEditor)
};
