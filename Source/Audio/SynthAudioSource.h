//
//  SynthAudioSource.h
//  JuceMonome
//
//  Created by Andy on 13/07/2013.
//
//

#ifndef __JuceMonome__SynthAudioSource__
#define __JuceMonome__SynthAudioSource__

#include "../JuceLibraryCode/JuceHeader.h"


// This is an audio source that streams the output of our demo synth.
class SynthAudioSource  : public AudioSource
{
public:
    // the synth itself!
    Synthesiser& synth;
    
    MidiMessageCollector& collector;
    
    //==============================================================================
    SynthAudioSource (Synthesiser& _synth, MidiMessageCollector& _collector) : synth(_synth), collector(_collector)
    {
        // add some voices to our synth, to play the sounds..
        for (int i = 8; --i >= 0;)
        {
            synth.addVoice (new SamplerVoice());    // and these ones play the sampled sounds
        }
        
    }
    
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate)
    {
        synth.setCurrentPlaybackSampleRate (sampleRate);
    }
    
    void releaseResources()
    {
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
    {
        // the synth always adds its output to the audio buffer, so we have to clear it
        // first..
        bufferToFill.clearActiveBufferRegion();
        
        // fill a midi buffer with incoming messages from the midi input.
        MidiBuffer incomingMidi;
        collector.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);
   
        if (incomingMidi.getNumEvents() != 0)
            DBG("Num events = " << incomingMidi.getNumEvents() << " Time " << incomingMidi.getLastEventTime());
        
        // and now get the synth to process the midi events and generate its output.
        synth.renderNextBlock (*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);
    }
};

#endif /* defined(__JuceMonome__SynthAudioSource__) */
