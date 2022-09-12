/*
  ==============================================================================

    MyConv.cpp
    Created: 22 Jul 2022 10:12:29am
    Author:  thesp

  ==============================================================================
*/

#include "MyConv.h"

namespace xynth
{
Convolution::Convolution() : 
    conv(juce::dsp::Convolution::NonUniform{ 2048 })
    //conv(juce::dsp::Convolution::Latency{ 2048 })
{
}

Convolution::~Convolution()
{
}

void Convolution::prepare(juce::dsp::ProcessSpec spec)
{
    conv.loadImpulseResponse(BinaryData::ir1_wav, BinaryData::ir1_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::no, 0);
    conv.prepare(spec);
}

void Convolution::process(const juce::dsp::ProcessContextReplacing<float>& context)
{
    conv.process(context);
    //DBG(conv.getLatency() << ", " << conv.getCurrentIRSize());
}

int Convolution::getLatency()
{
    return conv.getLatency();
}

}