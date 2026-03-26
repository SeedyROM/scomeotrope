#pragma once

#include "data/RuntimeParameters.h"
#include "presets/PluginPresetManager.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

#include <atomic>

#include "FaustBridge.h"

class ScomeotropeAudioProcessor : public juce::AudioProcessor {
public:
  enum class ABSlot { A, B };

  ScomeotropeAudioProcessor();
  ~ScomeotropeAudioProcessor() override;

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;

  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  static constexpr int stateSchemaVersion = 1;

  juce::AudioProcessorValueTreeState apvts;

FaustBridge &getFaustBridge() { return faustBridge; }

float getInputMeterPeak() const { return inputMeterPeak.load(); }
  float getOutputMeterPeak() const { return outputMeterPeak.load(); }
  float getInputMeterPeakL() const { return inputMeterPeakL.load(); }
  float getInputMeterPeakR() const { return inputMeterPeakR.load(); }
  float getOutputMeterPeakL() const { return outputMeterPeakL.load(); }
  float getOutputMeterPeakR() const { return outputMeterPeakR.load(); }
  float getGainReductionDb() const { return gainReductionDb.load(std::memory_order_relaxed); }
  ABSlot getActiveABSlot() const { return activeABSlot; }
  bool hasDistinctABState() const;
  juce::StringArray getAvailablePresetNames() const;
  juce::String getActivePresetName() const;
  juce::String getDisplayedPresetName();
  bool isActivePresetFactory() const;
  bool loadPreset(const juce::String &presetName);
  bool saveUserPreset(const juce::String &presetName);
  bool deleteActiveUserPreset();
  void revealPresetDirectory() const;
  void clearABState();
  void setActiveABSlot(ABSlot slot);
  void copyABSlot(ABSlot from, ABSlot to);
  void swapABSlots();

private:
  juce::ValueTree captureCurrentState();
  juce::ValueTree createWrappedPluginState(bool includeABState);
  juce::ValueTree extractPluginStateFromSavedTree(const juce::ValueTree &savedTree) const;
  juce::ValueTree migrateStateTree(juce::ValueTree savedTree) const;
  void applyStateToApvts(const juce::ValueTree &stateToApply);
  void initialiseABSlotsFromCurrentState();
  void syncActiveABSlotFromCurrentState();
  void sanitiseTransientState(juce::ValueTree &state) const;
  void setActivePresetName(const juce::String &presetName);
  juce::ValueTree &getMutableABState(ABSlot slot);
  const juce::ValueTree &getABState(ABSlot slot) const;
  void updatePeakMeter(std::atomic<float> &meterState, float blockPeak, int numSamples) noexcept;

FaustBridge faustBridge;

juce::ValueTree defaultPresetState;
  juce::ValueTree slotAState;
  juce::ValueTree slotBState;
  ABSlot activeABSlot = ABSlot::A;
  juce::String slotAPresetName{"Init"};
  juce::String slotBPresetName{"Init"};
  bool isApplyingABState = false;
  std::atomic<float> inputMeterPeak{0.0f};
  std::atomic<float> outputMeterPeak{0.0f};
  std::atomic<float> inputMeterPeakL{0.0f};
  std::atomic<float> inputMeterPeakR{0.0f};
  std::atomic<float> outputMeterPeakL{0.0f};
  std::atomic<float> outputMeterPeakR{0.0f};
  std::atomic<float> gainReductionDb{0.0f};
  int meterSamplesSinceLastUpdate = 0;
  int meterUpdateIntervalSamples = 512;
  double currentSampleRate = 44100.0;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScomeotropeAudioProcessor)
};
