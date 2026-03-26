#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <cmath>

namespace {

constexpr auto pluginStateRootType = "PluginStateRoot";
constexpr auto pluginStatePropertySchemaVersion = "schemaVersion";
constexpr auto pluginStatePropertyPluginVersion = "pluginVersion";
constexpr auto pluginStatePropertyActiveABSlot = "activeABSlot";
constexpr auto pluginStatePropertySlotAPresetName = "slotAPresetName";
constexpr auto pluginStatePropertySlotBPresetName = "slotBPresetName";
constexpr auto pluginStateChildSlotA = "slotAState";
constexpr auto pluginStateChildSlotB = "slotBState";
constexpr auto abSlotNameA = "A";
constexpr auto abSlotNameB = "B";

} // namespace

ScomeotropeAudioProcessor::ScomeotropeAudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", RuntimeParameters::createLayout())
,
      faustBridge(apvts)
{
  defaultPresetState = captureCurrentState();
  initialiseABSlotsFromCurrentState();
}

ScomeotropeAudioProcessor::~ScomeotropeAudioProcessor() {}

const juce::String ScomeotropeAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool ScomeotropeAudioProcessor::acceptsMidi() const {
  return false;
}

bool ScomeotropeAudioProcessor::producesMidi() const {
  return false;
}

bool ScomeotropeAudioProcessor::isMidiEffect() const {
  return false;
}

double ScomeotropeAudioProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int ScomeotropeAudioProcessor::getNumPrograms() {
  return 1;
}

int ScomeotropeAudioProcessor::getCurrentProgram() {
  return 0;
}

void ScomeotropeAudioProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String ScomeotropeAudioProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void ScomeotropeAudioProcessor::changeProgramName(int index,
                                               const juce::String &newName) {
  juce::ignoreUnused(index, newName);
}

void ScomeotropeAudioProcessor::prepareToPlay(double sampleRate,
                                           int samplesPerBlock) {
  currentSampleRate = sampleRate;
  inputMeterPeak.store(0.0f);
  outputMeterPeak.store(0.0f);
  inputMeterPeakL.store(0.0f);
  inputMeterPeakR.store(0.0f);
  outputMeterPeakL.store(0.0f);
  outputMeterPeakR.store(0.0f);
  meterSamplesSinceLastUpdate = 0;
  meterUpdateIntervalSamples = juce::jmax(256, samplesPerBlock);

faustBridge.prepare(sampleRate, samplesPerBlock);
}

void ScomeotropeAudioProcessor::releaseResources() {
  // Release resources here
}

juce::ValueTree ScomeotropeAudioProcessor::captureCurrentState() {
  auto state = apvts.copyState();
  sanitiseTransientState(state);
  return state;
}

juce::ValueTree ScomeotropeAudioProcessor::createWrappedPluginState(bool includeABState) {
  juce::ValueTree wrappedState(pluginStateRootType);
  wrappedState.setProperty(pluginStatePropertySchemaVersion, stateSchemaVersion, nullptr);
  wrappedState.setProperty(pluginStatePropertyPluginVersion, JucePlugin_VersionString, nullptr);
  syncActiveABSlotFromCurrentState();
  wrappedState.appendChild(captureCurrentState(), nullptr);

  if (includeABState) {
    wrappedState.setProperty(pluginStatePropertyActiveABSlot,
                             activeABSlot == ABSlot::A ? abSlotNameA : abSlotNameB,
                             nullptr);
    wrappedState.setProperty(pluginStatePropertySlotAPresetName, slotAPresetName, nullptr);
    wrappedState.setProperty(pluginStatePropertySlotBPresetName, slotBPresetName, nullptr);

    juce::ValueTree slotAWrapper(pluginStateChildSlotA);
    slotAWrapper.appendChild(slotAState.createCopy(), nullptr);
    wrappedState.appendChild(slotAWrapper, nullptr);

    juce::ValueTree slotBWrapper(pluginStateChildSlotB);
    slotBWrapper.appendChild(slotBState.createCopy(), nullptr);
    wrappedState.appendChild(slotBWrapper, nullptr);
  }

  return wrappedState;
}

juce::ValueTree ScomeotropeAudioProcessor::migrateStateTree(juce::ValueTree savedTree) const {
  auto schemaVersion = savedTree.getProperty(pluginStatePropertySchemaVersion);
  if (!schemaVersion.isInt() || static_cast<int>(schemaVersion) < 1)
    savedTree.setProperty(pluginStatePropertySchemaVersion, stateSchemaVersion, nullptr);

  if (!savedTree.hasProperty(pluginStatePropertyPluginVersion))
    savedTree.setProperty(pluginStatePropertyPluginVersion, JucePlugin_VersionString, nullptr);

  if (!savedTree.hasProperty(pluginStatePropertyActiveABSlot))
    savedTree.setProperty(pluginStatePropertyActiveABSlot, abSlotNameA, nullptr);

  if (!savedTree.hasProperty(pluginStatePropertySlotAPresetName))
    savedTree.setProperty(pluginStatePropertySlotAPresetName, "Init", nullptr);

  if (!savedTree.hasProperty(pluginStatePropertySlotBPresetName))
    savedTree.setProperty(pluginStatePropertySlotBPresetName, "Init", nullptr);

  return savedTree;
}

juce::ValueTree
ScomeotropeAudioProcessor::extractPluginStateFromSavedTree(const juce::ValueTree &savedTree) const {
  if (!savedTree.isValid())
    return {};

  if (savedTree.hasType(apvts.state.getType()))
    return savedTree.createCopy();

  if (!savedTree.hasType(pluginStateRootType))
    return {};

  auto migratedTree = migrateStateTree(savedTree.createCopy());
  auto pluginState = migratedTree.getChildWithName(apvts.state.getType());
  return pluginState.isValid() ? pluginState.createCopy() : juce::ValueTree{};
}

void ScomeotropeAudioProcessor::applyStateToApvts(const juce::ValueTree &stateToApply) {
  if (!stateToApply.isValid())
    return;

  const juce::ScopedValueSetter<bool> applyingState(isApplyingABState, true);
  apvts.replaceState(stateToApply.createCopy());
}

void ScomeotropeAudioProcessor::initialiseABSlotsFromCurrentState() {
  auto currentState = captureCurrentState();
  slotAState = currentState.createCopy();
  slotBState = currentState.createCopy();
  activeABSlot = ABSlot::A;
  slotAPresetName = "Init";
  slotBPresetName = "Init";
}

void ScomeotropeAudioProcessor::syncActiveABSlotFromCurrentState() {
  if (isApplyingABState)
    return;

  getMutableABState(activeABSlot) = captureCurrentState();
}

void ScomeotropeAudioProcessor::sanitiseTransientState(juce::ValueTree &state) const {
  juce::ignoreUnused(state);
}

void ScomeotropeAudioProcessor::setActivePresetName(const juce::String &presetName) {
  if (activeABSlot == ABSlot::A)
    slotAPresetName = presetName;
  else
    slotBPresetName = presetName;
}

juce::ValueTree &ScomeotropeAudioProcessor::getMutableABState(ABSlot slot) {
  return slot == ABSlot::A ? slotAState : slotBState;
}

const juce::ValueTree &ScomeotropeAudioProcessor::getABState(ABSlot slot) const {
  return slot == ABSlot::A ? slotAState : slotBState;
}

void ScomeotropeAudioProcessor::setActiveABSlot(ABSlot slot) {
  if (slot == activeABSlot)
    return;

  syncActiveABSlotFromCurrentState();
  activeABSlot = slot;
  applyStateToApvts(getABState(activeABSlot));
}

void ScomeotropeAudioProcessor::copyABSlot(ABSlot from, ABSlot to) {
  syncActiveABSlotFromCurrentState();
  getMutableABState(to) = getABState(from).createCopy();
  if (from == ABSlot::A)
    slotBPresetName = slotAPresetName;
  else
    slotAPresetName = slotBPresetName;

  if (activeABSlot == to)
    applyStateToApvts(getABState(to));
}

void ScomeotropeAudioProcessor::swapABSlots() {
  syncActiveABSlotFromCurrentState();
  auto previousA = slotAState.createCopy();
  auto previousName = slotAPresetName;
  slotAState = slotBState.createCopy();
  slotBState = previousA;
  slotAPresetName = slotBPresetName;
  slotBPresetName = previousName;
  applyStateToApvts(getABState(activeABSlot));
}

juce::StringArray ScomeotropeAudioProcessor::getAvailablePresetNames() const {
  juce::StringArray names;
  for (const auto &preset :
       PluginPresetManager::getAvailablePresets(defaultPresetState, apvts.state.getType()))
    names.add(preset.name);
  return names;
}

bool ScomeotropeAudioProcessor::hasDistinctABState() const {
  return !slotAState.isEquivalentTo(slotBState);
}

juce::String ScomeotropeAudioProcessor::getActivePresetName() const {
  return activeABSlot == ABSlot::A ? slotAPresetName : slotBPresetName;
}

juce::String ScomeotropeAudioProcessor::getDisplayedPresetName() {
  syncActiveABSlotFromCurrentState();

  const auto activePresetName = getActivePresetName();
  const auto currentState = captureCurrentState();
  const auto presetState =
      PluginPresetManager::loadPresetState(activePresetName, defaultPresetState, apvts.state.getType());

  if (!presetState.isValid())
    return activePresetName;

  return currentState.isEquivalentTo(presetState) ? activePresetName : "Custom";
}

bool ScomeotropeAudioProcessor::isActivePresetFactory() const {
  return PluginPresetManager::isFactoryPreset(
      getActivePresetName(), defaultPresetState, apvts.state.getType());
}

bool ScomeotropeAudioProcessor::loadPreset(const juce::String &presetName) {
  auto presetState =
      PluginPresetManager::loadPresetState(presetName, defaultPresetState, apvts.state.getType());
  if (!presetState.isValid())
    return false;

  getMutableABState(activeABSlot) = presetState.createCopy();
  setActivePresetName(presetName);
  applyStateToApvts(getABState(activeABSlot));
  return true;
}

bool ScomeotropeAudioProcessor::saveUserPreset(const juce::String &presetName) {
  syncActiveABSlotFromCurrentState();
  auto wrappedState = createWrappedPluginState(false);
  if (!PluginPresetManager::saveUserPreset(presetName, wrappedState))
    return false;

  setActivePresetName(presetName);
  return true;
}

bool ScomeotropeAudioProcessor::deleteActiveUserPreset() {
  auto presetName = getActivePresetName();
  if (presetName.isEmpty() ||
      PluginPresetManager::isFactoryPreset(presetName, defaultPresetState, apvts.state.getType()))
    return false;

  if (!PluginPresetManager::deleteUserPreset(presetName))
    return false;

  setActivePresetName("Init");
  return true;
}

void ScomeotropeAudioProcessor::revealPresetDirectory() const {
  PluginPresetManager::revealPresetDirectory();
}

void ScomeotropeAudioProcessor::clearABState() {
  syncActiveABSlotFromCurrentState();
  slotBState = slotAState.createCopy();
  slotBPresetName = slotAPresetName;
  activeABSlot = ABSlot::A;
}

void ScomeotropeAudioProcessor::updatePeakMeter(std::atomic<float> &meterState,
                                              float blockPeak,
                                              int numSamples) noexcept {
  const auto heldPeak = meterState.load(std::memory_order_relaxed);
  if (blockPeak >= heldPeak) {
    meterState.store(blockPeak, std::memory_order_relaxed);
    return;
  }

  const auto decayTimeSeconds = 0.16f;
  const auto decay =
      std::exp(-static_cast<float>(numSamples) /
               static_cast<float>(juce::jmax(1.0, currentSampleRate * decayTimeSeconds)));
  meterState.store(heldPeak * decay, std::memory_order_relaxed);
}

bool ScomeotropeAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const {
  const auto mainInput = layouts.getMainInputChannelSet();
  const auto mainOutput = layouts.getMainOutputChannelSet();

  if (mainOutput != juce::AudioChannelSet::stereo())
    return false;

  if (mainInput != juce::AudioChannelSet::mono() && mainInput != juce::AudioChannelSet::stereo())
    return false;

  return true;
}

void ScomeotropeAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                          juce::MidiBuffer &midiMessages) {
  juce::ignoreUnused(midiMessages);
  juce::ScopedNoDenormals noDenormals;

  const auto totalNumInputChannels = getTotalNumInputChannels();
  const auto totalNumOutputChannels = getTotalNumOutputChannels();

  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  auto getMaxPeak = [&buffer](int numChannels) {
    auto peak = 0.0f;
    for (int ch = 0; ch < numChannels; ++ch)
      peak = juce::jmax(peak, buffer.getMagnitude(ch, 0, buffer.getNumSamples()));
    return peak;
  };

  auto getChannelPeak = [&buffer](int ch) {
    if (ch < buffer.getNumChannels())
      return juce::jlimit(0.0f, 1.2f, buffer.getMagnitude(ch, 0, buffer.getNumSamples()));
    return 0.0f;
  };

  meterSamplesSinceLastUpdate += buffer.getNumSamples();
  const bool shouldMeasurePeaks = meterSamplesSinceLastUpdate >= meterUpdateIntervalSamples;
  if (shouldMeasurePeaks) {
    updatePeakMeter(inputMeterPeak,
                    juce::jlimit(0.0f, 1.2f, getMaxPeak(totalNumInputChannels)),
                    meterSamplesSinceLastUpdate);
    updatePeakMeter(inputMeterPeakL, getChannelPeak(0), meterSamplesSinceLastUpdate);
    updatePeakMeter(inputMeterPeakR, getChannelPeak(totalNumInputChannels > 1 ? 1 : 0),
                    meterSamplesSinceLastUpdate);
  }

faustBridge.process(buffer, totalNumInputChannels, totalNumOutputChannels);
  gainReductionDb.store(faustBridge.getCompGr(), std::memory_order_relaxed);
  gainReductionDbL.store(faustBridge.getCompGrL(), std::memory_order_relaxed);
  gainReductionDbR.store(faustBridge.getCompGrR(), std::memory_order_relaxed);
if (shouldMeasurePeaks) {
    updatePeakMeter(outputMeterPeak,
                    juce::jlimit(0.0f, 1.2f, getMaxPeak(totalNumOutputChannels)),
                    meterSamplesSinceLastUpdate);
    updatePeakMeter(outputMeterPeakL, getChannelPeak(0), meterSamplesSinceLastUpdate);
    updatePeakMeter(outputMeterPeakR, getChannelPeak(totalNumOutputChannels > 1 ? 1 : 0),
                    meterSamplesSinceLastUpdate);
    meterSamplesSinceLastUpdate = 0;
  }
}

bool ScomeotropeAudioProcessor::hasEditor() const {
  return true;
}

juce::AudioProcessorEditor *ScomeotropeAudioProcessor::createEditor() {
  return new ScomeotropeAudioProcessorEditor(*this);
}

void ScomeotropeAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
  auto state = createWrappedPluginState(true);
  std::unique_ptr<juce::XmlElement> xml(state.createXml());
  copyXmlToBinary(*xml, destData);
}

void ScomeotropeAudioProcessor::setStateInformation(const void *data,
                                                 int sizeInBytes) {
  std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
  if (xml == nullptr)
    return;

  auto restoredTree = juce::ValueTree::fromXml(*xml);
  auto pluginState = extractPluginStateFromSavedTree(restoredTree);
  if (!pluginState.isValid())
    return;

  applyStateToApvts(pluginState);

  if (restoredTree.hasType(pluginStateRootType)) {
    auto migratedTree = migrateStateTree(restoredTree.createCopy());
    auto restoredSlotAWrapper = migratedTree.getChildWithName(pluginStateChildSlotA);
    auto restoredSlotBWrapper = migratedTree.getChildWithName(pluginStateChildSlotB);

    auto restoredSlotA = restoredSlotAWrapper.getChildWithName(apvts.state.getType());
    auto restoredSlotB = restoredSlotBWrapper.getChildWithName(apvts.state.getType());

    if (restoredSlotA.isValid() && restoredSlotB.isValid()) {
      slotAState = restoredSlotA.createCopy();
      slotBState = restoredSlotB.createCopy();
      slotAPresetName = migratedTree[pluginStatePropertySlotAPresetName].toString();
      slotBPresetName = migratedTree[pluginStatePropertySlotBPresetName].toString();
      activeABSlot = migratedTree[pluginStatePropertyActiveABSlot].toString() == abSlotNameB
                         ? ABSlot::B
                         : ABSlot::A;
      applyStateToApvts(getABState(activeABSlot));
      return;
    }
  }

  initialiseABSlotsFromCurrentState();
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new ScomeotropeAudioProcessor();
}
