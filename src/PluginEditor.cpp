#include "PluginEditor.h"
#include "BinaryData.h"
#include "PluginProcessor.h"

#include "ui/ScomeotropeColors.h"

#include <memory>
ScomeotropeAudioProcessorEditor::ScomeotropeAudioProcessorEditor(
    ScomeotropeAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
setLookAndFeel(&pluginLnf);
addAndMakeVisible(topBar);

topBar.onSelectSlotA = [this] {
    audioProcessor.setActiveABSlot(ScomeotropeAudioProcessor::ABSlot::A);
    topBar.setActiveABSlot(true);
    refreshPresetControls();
  };
  topBar.onSelectSlotB = [this] {
    audioProcessor.setActiveABSlot(ScomeotropeAudioProcessor::ABSlot::B);
    topBar.setActiveABSlot(false);
    refreshPresetControls();
  };
  topBar.onPresetSelected = [this](const juce::String &presetName) {
    if (audioProcessor.loadPreset(presetName))
      refreshPresetControls();
  };
  topBar.onShowOptionsMenu = [this] { showOptionsMenu(); };
  topBar.setActiveABSlot(audioProcessor.getActiveABSlot() == ScomeotropeAudioProcessor::ABSlot::A);
  refreshPresetControls();

  setResizable(true, true);
  setResizeLimits(minEditorWidth, minEditorHeight, 1072, 644);
  setSize(minEditorWidth, minEditorHeight);

  startTimerHz(24);
}

ScomeotropeAudioProcessorEditor::~ScomeotropeAudioProcessorEditor() {
stopTimer();
  setLookAndFeel(nullptr);
}

void ScomeotropeAudioProcessorEditor::timerCallback() {
  // Refresh metered knobs here at ~24 Hz. For example:
  //   myKnob.refreshMeter();
  // To wire a knob to a meter source, call in the constructor:
  //   myKnob.setMeterSource([&p] { return p.getOutputMeterPeak(); });
}

void ScomeotropeAudioProcessorEditor::refreshPresetControls() {
  topBar.setPresetNames(audioProcessor.getAvailablePresetNames());
  topBar.setSelectedPresetName(audioProcessor.getDisplayedPresetName());
  topBar.setActiveABSlot(audioProcessor.getActiveABSlot() == ScomeotropeAudioProcessor::ABSlot::A);
}

void ScomeotropeAudioProcessorEditor::promptSavePreset() {
  auto dialog = std::make_unique<juce::AlertWindow>(
      "Save Preset", "Enter a preset name.", juce::AlertWindow::NoIcon);
  dialog->addTextEditor("presetName", audioProcessor.getActivePresetName(), "Preset name");
  dialog->addButton("Save", 1);
  dialog->addButton("Cancel", 0);

  auto *dialogPtr = dialog.release();
  dialogPtr->enterModalState(
      true,
      juce::ModalCallbackFunction::create([this, dialogPtr](int result) {
        std::unique_ptr<juce::AlertWindow> owner(dialogPtr);
        if (result == 1 &&
            audioProcessor.saveUserPreset(dialogPtr->getTextEditorContents("presetName").trim()))
          refreshPresetControls();
      }),
      true);
}

void ScomeotropeAudioProcessorEditor::showOptionsMenu() {
  juce::PopupMenu menu;
  menu.setLookAndFeel(&pluginLnf);
  menu.addItem(1, "Copy A to B");
  menu.addItem(2, "Copy B to A");
  menu.addItem(3, "Clear A/B", audioProcessor.hasDistinctABState());
  menu.addSeparator();
  menu.addItem(4, "Save Preset...");
  menu.addItem(5,
               "Delete Current Preset",
               !audioProcessor.getActivePresetName().isEmpty() &&
                   !audioProcessor.isActivePresetFactory());
  menu.addItem(6, "Reveal Presets Folder");

  menu.showMenuAsync(
      juce::PopupMenu::Options().withTargetComponent(topBar.getOptionsTargetComponent()),
      [this](int result) {
        if (result == 1)
          audioProcessor.copyABSlot(ScomeotropeAudioProcessor::ABSlot::A,
                                    ScomeotropeAudioProcessor::ABSlot::B);
        else if (result == 2)
          audioProcessor.copyABSlot(ScomeotropeAudioProcessor::ABSlot::B,
                                    ScomeotropeAudioProcessor::ABSlot::A);
        else if (result == 3)
          audioProcessor.clearABState();
        else if (result == 4)
          promptSavePreset();
        else if (result == 5)
          audioProcessor.deleteActiveUserPreset();
        else if (result == 6)
          audioProcessor.revealPresetDirectory();

        topBar.setActiveABSlot(audioProcessor.getActiveABSlot() ==
                               ScomeotropeAudioProcessor::ABSlot::A);
        refreshPresetControls();
      });
}
void ScomeotropeAudioProcessorEditor::paint(juce::Graphics &g) {
g.fillAll(juce::Colour(ScomeotropeColors::background));
}

void ScomeotropeAudioProcessorEditor::resized() {
  auto bounds = getLocalBounds();
  topBar.setBounds(bounds.removeFromTop(50));

}
