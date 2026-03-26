#include "PluginEditor.h"
#include "BinaryData.h"
#include "PluginProcessor.h"

#include "ui/ScomeotropeColors.h"

#include <memory>
ScomeotropeAudioProcessorEditor::ScomeotropeAudioProcessorEditor(
    ScomeotropeAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p),
      // --- Dynamics knobs ---
      strengthKnob(p.apvts, RuntimeParamIDs::compStrength, "STRENGTH"),
      thresholdKnob(p.apvts, RuntimeParamIDs::compThreshold, "THRESHOLD", " dB"),
      kneeKnob(p.apvts, RuntimeParamIDs::compKnee, "KNEE", " dB"),
      attackKnob(p.apvts, RuntimeParamIDs::compAttack, "ATTACK", " s"),
      releaseKnob(p.apvts, RuntimeParamIDs::compRelease, "RELEASE", " s"),
      compMixKnob(p.apvts, RuntimeParamIDs::compMix, "MIX"),
      makeupKnob(p.apvts, RuntimeParamIDs::compMakeup, "MAKE UP", " dB"),
      // --- Machine Decay knobs ---
      speedKnob(p.apvts, RuntimeParamIDs::vibratoSpeed, "SPEED", " Hz"),
      amountKnob(p.apvts, RuntimeParamIDs::vibratoAmount, "AMOUNT"),
      vibratoMixKnob(p.apvts, RuntimeParamIDs::vibratoMix, "MIX"),
      // --- Tape Character knobs ---
      noiseKnob(p.apvts, RuntimeParamIDs::noiseAmount, "NOISE"),
      flutterKnob(p.apvts, RuntimeParamIDs::flutter, "FLUTTER"),
      outputGainKnob(p.apvts, RuntimeParamIDs::outputGain, "OUTPUT GAIN") {
  setLookAndFeel(&pluginLnf);
  addAndMakeVisible(topBar);

  // --- Add dynamics controls ---
  addAndMakeVisible(strengthKnob);
  addAndMakeVisible(thresholdKnob);
  addAndMakeVisible(kneeKnob);
  addAndMakeVisible(attackKnob);
  addAndMakeVisible(releaseKnob);
  addAndMakeVisible(compMixKnob);
  addAndMakeVisible(makeupKnob);

  // --- LED VU meters (input) ---
  addAndMakeVisible(inputMeterL);
  addAndMakeVisible(inputMeterR);
  inputMeterL.setChannelLabel("L");
  inputMeterR.setChannelLabel("R");
  inputMeterL.setMeterSource([&p] { return p.getInputMeterPeakL(); });
  inputMeterR.setMeterSource([&p] { return p.getInputMeterPeakR(); });

  // --- GR meters (gain reduction, top-down amber) ---
  addAndMakeVisible(grMeterL);
  addAndMakeVisible(grMeterR);
  grMeterL.setGainReductionMode(true);
  grMeterR.setGainReductionMode(true);
  grMeterL.setChannelLabel("L");
  grMeterR.setChannelLabel("R");
  grMeterL.setMeterSource([&p] { return p.getGainReductionDbL(); });
  grMeterR.setMeterSource([&p] { return p.getGainReductionDbR(); });

  // --- Machine Decay controls ---
  addAndMakeVisible(speedKnob);
  addAndMakeVisible(amountKnob);
  addAndMakeVisible(vibratoMixKnob);

  addAndMakeVisible(vibratoBypassButton);
  vibratoBypassAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
          p.apvts, RuntimeParamIDs::vibratoBypass,
          vibratoBypassButton.getToggle());

  // When bypass is ON, disable the vibrato knobs
  vibratoBypassButton.setOnClick([this] {
    const bool bypassed = vibratoBypassButton.getToggle().getToggleState();
    speedKnob.setEnabled(!bypassed);
    amountKnob.setEnabled(!bypassed);
    vibratoMixKnob.setEnabled(!bypassed);
    speedKnob.setAlpha(bypassed ? 0.35f : 1.0f);
    amountKnob.setAlpha(bypassed ? 0.35f : 1.0f);
    vibratoMixKnob.setAlpha(bypassed ? 0.35f : 1.0f);
  });
  // Apply initial bypass state
  {
    const bool bypassed = vibratoBypassButton.getToggle().getToggleState();
    speedKnob.setEnabled(!bypassed);
    amountKnob.setEnabled(!bypassed);
    vibratoMixKnob.setEnabled(!bypassed);
    speedKnob.setAlpha(bypassed ? 0.35f : 1.0f);
    amountKnob.setAlpha(bypassed ? 0.35f : 1.0f);
    vibratoMixKnob.setAlpha(bypassed ? 0.35f : 1.0f);
  }

  // --- Tape Character controls ---
  addAndMakeVisible(noiseKnob);
  addAndMakeVisible(flutterKnob);
  addAndMakeVisible(outputGainKnob);

  // --- Output meter on Output Gain knob ---
  outputGainKnob.setMeterSource([&p] { return p.getOutputMeterPeak(); });

  // --- TopBar callbacks ---
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
  setResizeLimits(minEditorWidth, minEditorHeight, 1072, minEditorHeight);
  setSize(minEditorWidth, minEditorHeight);

  startTimerHz(24);
}

ScomeotropeAudioProcessorEditor::~ScomeotropeAudioProcessorEditor() {
  stopTimer();
  setLookAndFeel(nullptr);
}

void ScomeotropeAudioProcessorEditor::timerCallback() {
  outputGainKnob.refreshMeter();
  inputMeterL.refresh();
  inputMeterR.refresh();
  grMeterL.refresh();
  grMeterR.refresh();
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

void ScomeotropeAudioProcessorEditor::drawSectionPanel(
    juce::Graphics &g, juce::Rectangle<int> bounds, const juce::String &title) {
  namespace C = ScomeotropeColors;
  const auto fb = bounds.toFloat();
  constexpr float cornerRadius = 6.0f;

  // Background
  g.setColour(juce::Colour(C::panelBg));
  g.fillRoundedRectangle(fb, cornerRadius);

  // Border
  g.setColour(juce::Colour(C::panelBorder));
  g.drawRoundedRectangle(fb.reduced(0.5f), cornerRadius, 1.0f);

  // Section title
  if (title.isNotEmpty()) {
    g.setColour(juce::Colour(C::sectionTitle));
    g.setFont(juce::Font(11.0f, juce::Font::bold));
    g.drawText(title, bounds.getX() + 12, bounds.getY() + 2,
               bounds.getWidth() - 24, 24, juce::Justification::centredLeft);
  }
}

void ScomeotropeAudioProcessorEditor::paint(juce::Graphics &g) {
  namespace C = ScomeotropeColors;
  g.fillAll(juce::Colour(C::background));

  auto bounds = getLocalBounds();
  bounds.removeFromTop(50); // TopBar
  bounds.reduce(12, 12);    // Content padding

  const int sectionGap = 8;

  // --- Top row: DYNAMICS ---
  const int topRowHeight = juce::jlimit(260, 400, (bounds.getHeight() * 55) / 100);
  auto topRow = bounds.removeFromTop(topRowHeight);
  bounds.removeFromTop(sectionGap);

  drawSectionPanel(g, topRow, "DYNAMICS");

  // --- Bottom row: MACHINE DECAY | TAPE CHARACTER ---
  auto bottomRow = bounds;
  const int totalW = bottomRow.getWidth() - sectionGap;
  const int halfW = totalW / 2;

  auto machineDecaySection = bottomRow.removeFromLeft(halfW);
  bottomRow.removeFromLeft(sectionGap);
  auto tapeCharSection = bottomRow;

  drawSectionPanel(g, machineDecaySection, "MACHINE DECAY");
  drawSectionPanel(g, tapeCharSection, "TAPE CHARACTER");
}

void ScomeotropeAudioProcessorEditor::resized() {
  auto bounds = getLocalBounds();
  topBar.setBounds(bounds.removeFromTop(50));

  bounds.reduce(12, 12); // Content padding
  const int sectionGap = 8;
  const int sectionPadding = 8;
  const int titleHeight = 24;

  // --- Top row: DYNAMICS ---
  const int topRowHeight = juce::jlimit(260, 400, (bounds.getHeight() * 55) / 100);
  auto topRow = bounds.removeFromTop(topRowHeight);
  bounds.removeFromTop(sectionGap);

  // --- Dynamics section layout ---
  {
    auto area = topRow.reduced(sectionPadding);
    area.removeFromTop(titleHeight);

    const int vGap = 6;
    const int knobRowH = 110;
    const int meterW = 42;
    const int meterGap = 6;

    // Input LED meters on the left edge (L + R side by side)
    {
      auto meterArea = area.removeFromLeft(meterW * 2 + meterGap);
      area.removeFromLeft(meterGap);
      auto meterL = meterArea.removeFromLeft(meterW);
      meterArea.removeFromLeft(meterGap);
      auto meterR = meterArea;
      inputMeterL.setBounds(meterL);
      inputMeterR.setBounds(meterR);
    }

    // GR LED meters on the right edge (L + R side by side)
    {
      auto meterArea = area.removeFromRight(meterW * 2 + meterGap);
      area.removeFromRight(meterGap);
      auto meterR = meterArea.removeFromRight(meterW);
      meterArea.removeFromRight(meterGap);
      auto meterL = meterArea;
      grMeterL.setBounds(meterL);
      grMeterR.setBounds(meterR);
    }

    // Remaining centre area for knob rows
    // First knob row: Strength | Threshold | Knee | Attack | Release
    {
      auto knobRow = area.removeFromTop(knobRowH);
      area.removeFromTop(vGap);

      const int hGap = 4;
      const int numKnobs = 5;
      const int knobW = (knobRow.getWidth() - hGap * (numKnobs - 1)) / numKnobs;

      strengthKnob.setBounds(knobRow.removeFromLeft(knobW));
      knobRow.removeFromLeft(hGap);
      thresholdKnob.setBounds(knobRow.removeFromLeft(knobW));
      knobRow.removeFromLeft(hGap);
      kneeKnob.setBounds(knobRow.removeFromLeft(knobW));
      knobRow.removeFromLeft(hGap);
      attackKnob.setBounds(knobRow.removeFromLeft(knobW));
      knobRow.removeFromLeft(hGap);
      releaseKnob.setBounds(knobRow);
    }

    // Second knob row: Mix | Make Up (centred, 2 knobs wide)
    {
      auto knobRow = area;
      const int hGap = 4;
      const int knobW = 110;
      const int totalKnobsW = knobW * 2 + hGap;
      const int startX = knobRow.getX() + (knobRow.getWidth() - totalKnobsW) / 2;

      compMixKnob.setBounds(startX, knobRow.getY(), knobW, knobRow.getHeight());
      makeupKnob.setBounds(startX + knobW + hGap, knobRow.getY(), knobW, knobRow.getHeight());
    }
  }

  // --- Bottom row: MACHINE DECAY | TAPE CHARACTER ---
  auto bottomRow = bounds;
  const int totalW = bottomRow.getWidth() - sectionGap;
  const int halfW = totalW / 2;

  auto machineDecaySection = bottomRow.removeFromLeft(halfW);
  bottomRow.removeFromLeft(sectionGap);
  auto tapeCharSection = bottomRow;

  // --- Machine Decay section ---
  {
    // Bypass PowerButton: 10x10 in the section title bar top-right
    {
      const int btnSize = 10;
      vibratoBypassButton.setBounds(
          machineDecaySection.getRight() - sectionPadding - btnSize,
          machineDecaySection.getY() + sectionPadding,
          btnSize, btnSize);
    }

    auto area = machineDecaySection.reduced(sectionPadding);
    area.removeFromTop(titleHeight);

    // Speed | Amount | Mix — capped width, centred
    {
      const int numKnobs = 3;
      const int maxKnobW = 100;
      const int hGap = 16;
      const int knobW = juce::jmin(maxKnobW,
                                    (area.getWidth() - hGap * (numKnobs - 1)) / numKnobs);
      const int totalKnobsW = knobW * numKnobs + hGap * (numKnobs - 1);
      auto centred = area.withSizeKeepingCentre(totalKnobsW, area.getHeight());

      speedKnob.setBounds(centred.removeFromLeft(knobW));
      centred.removeFromLeft(hGap);
      amountKnob.setBounds(centred.removeFromLeft(knobW));
      centred.removeFromLeft(hGap);
      vibratoMixKnob.setBounds(centred.removeFromLeft(knobW));
    }
  }

  // --- Tape Character section (horizontal layout) ---
  {
    auto area = tapeCharSection.reduced(sectionPadding);
    area.removeFromTop(titleHeight);

    const int numKnobs = 3;
    const int maxKnobW = 100;
    const int hGap = 16;
    const int knobW = juce::jmin(maxKnobW,
                                  (area.getWidth() - hGap * (numKnobs - 1)) / numKnobs);
    const int totalKnobsW = knobW * numKnobs + hGap * (numKnobs - 1);
    auto centred = area.withSizeKeepingCentre(totalKnobsW, area.getHeight());

    noiseKnob.setBounds(centred.removeFromLeft(knobW));
    centred.removeFromLeft(hGap);
    flutterKnob.setBounds(centred.removeFromLeft(knobW));
    centred.removeFromLeft(hGap);
    outputGainKnob.setBounds(centred.removeFromLeft(knobW));
  }
}
