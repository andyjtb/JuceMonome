/*
 *  AudioControl.h
 *  sdaMidiMeter
 *
 *  Created by Andy Brown on 12/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_AUDIOCONTROL
#define H_AUDIOCONTROL

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthAudioSource.h"

/**
 Top Level Audio Class
 */
class AudioControl  :   public AudioIODeviceCallback,
						public ChangeBroadcaster
{
public:
	//==============================================================================
    /**
     Constructor
     */
	AudioControl();
    
    /**
     Destructor
     */
	~AudioControl();
	
	/**
     Loads the file at the specified location
     */
    void loadFile (const ValueTree files);
    
    //AudioCallbacks================================================================
	/** Processes a block of incoming 
     @param inputChannelData A set of arrays containing the audio data for each
     incoming channel
     
     @param numInputChannels The number of pointers to channel data in the
     inputChannelData array.
     
     @param outputChannelData A set of arrays which need to be filled with the data
     that should be sent to each outgoing channel of the device.

     @param numOutputChannels The number of pointers to channel data in the
     outputChannelData array.
     
     @param numSamples The number of samples in each channel of the input and
     output arrays.
     */
	void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels,
                                float** outputChannelData,
                                int numOutputChannels,
                                int numSamples);
    /** Implementation of the AudioSource method.
     */
    void audioDeviceAboutToStart (AudioIODevice* device);
    /** Implementation of the AudioSource method.
     */
    void audioDeviceStopped();

    /** Displays the audio preferences settings page
     @param centerComponent The component to display the page in the middle of
     */
	void showAudioPreferences(Component* centerComponent);
    
    /** Sets the playback volume
    @param incomingVolume The volume between 0-1
     */
	void setVolume(double incomingVolume);
    /** Returns the current playback volume
     @return The volume
     */
    double getVolume();
	
    /** Returns a string containing all of the file extensions that the audio format manager is able to create readers for, i.e what audio file the application can play
     */
    String getRegisteredFormatWildcard() {return formatManager.getWildcardForAllFormats();}
    
    void setTempo (double incTempo) { tempo = incTempo; }
    void setCounting (bool c) { counting = c; }

    bool loadSingleSample (ValueTree sample, int index);
    void playSample (ValueTree sample);
    void stopSamples (ValueTree samples);
private:
    
    AudioDeviceManager audioDeviceManager;	// this wraps the actual audio device
	AudioSourcePlayer audioSourcePlayer;
	AudioFormatManager formatManager;

    ScopedPointer<SynthAudioSource> synthSource;
    
    MidiMessageCollector collector;
    
    double tickCounter, sampleRate, tempo;
    bool counting;
    
    Synthesiser synth;
};



#endif //H_AUDIO