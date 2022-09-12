/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ConvolutionTestAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ConvolutionTestAudioProcessorEditor (ConvolutionTestAudioProcessor&);
    ~ConvolutionTestAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ConvolutionTestAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConvolutionTestAudioProcessorEditor)
};
