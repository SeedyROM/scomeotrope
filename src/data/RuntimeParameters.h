#pragma once

#include "FaustParams.h"

namespace RuntimeParamIDs = FaustParamIDs;

namespace RuntimeParameters {
inline juce::AudioProcessorValueTreeState::ParameterLayout createLayout() {
  return FaustParams::createLayout();
}
} // namespace RuntimeParameters

