/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ConvolutionTestAudioProcessor::ConvolutionTestAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    auto url = juce::URL("https://api.gumroad.com/v2/licenses/verify")
        .withParameter("product_permalink", "Rezonator")
        .withParameter("license_key", "34199715-152A466F-80096F99-517FEDE2");

    DBG(getResultText(url));
}

ConvolutionTestAudioProcessor::~ConvolutionTestAudioProcessor()
{
}

//==============================================================================
const juce::String ConvolutionTestAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ConvolutionTestAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ConvolutionTestAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ConvolutionTestAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ConvolutionTestAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ConvolutionTestAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ConvolutionTestAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ConvolutionTestAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ConvolutionTestAudioProcessor::getProgramName (int index)
{
    return {};
}

void ConvolutionTestAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ConvolutionTestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getNumInputChannels();

    conv.prepare(spec);
    setLatencySamples(conv.getLatency());
}

void ConvolutionTestAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ConvolutionTestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ConvolutionTestAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto audioBlock = juce::dsp::AudioBlock<float>(buffer).getSubsetChannelBlock(0, (size_t)totalNumInputChannels);
    auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);

    conv.process(context);
}

//==============================================================================
bool ConvolutionTestAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ConvolutionTestAudioProcessor::createEditor()
{
    return new ConvolutionTestAudioProcessorEditor (*this);
}

//==============================================================================
void ConvolutionTestAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ConvolutionTestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::String ConvolutionTestAudioProcessor::getResultText(const juce::URL& url)
{
    using namespace juce;
    StringPairArray responseHeaders;
    int statusCode = 0;


    if (auto stream = url.withPOSTData("product_permalink=Rezonator")
        .withPOSTData("license_key=34199715-152A466F-80096F99-517FEDE2")
        .createInputStream(URL::InputStreamOptions(URL::ParameterHandling::inAddress)
        .withConnectionTimeoutMs(10000)
        .withResponseHeaders(&responseHeaders)
        .withStatusCode(&statusCode)))
    {
        if (statusCode != 0)
        {
            auto resultJSON = juce::JSON::fromString(stream->readEntireStreamAsString());
            DBG("success: " << resultJSON["success"].toString());
        }

        return (statusCode != 0 ? "Status code: " + String(statusCode) + newLine : String())
            + "Response headers: " + newLine
            + responseHeaders.getDescription() + newLine
            + "----------------------------------------------------" + newLine
            + stream->readEntireStreamAsString();
    }

    if (statusCode != 0)
        return "Failed to connect, status code = " + String(statusCode);

    return "Failed to connect!";
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ConvolutionTestAudioProcessor();
}
