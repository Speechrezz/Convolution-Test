/*
  ==============================================================================

    MyConv.h
    Created: 22 Jul 2022 10:12:29am
    Author:  thesp

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace xynth
{
// =============================================================================

class Convolution
{
public:
    Convolution();
    ~Convolution();

    void prepare(juce::dsp::ProcessSpec);
    void process(const juce::dsp::ProcessContextReplacing<float>& context);
    int getLatency();

private:
    juce::dsp::Convolution conv;

};

// =============================================================================
}