/*
 *  AudioControl.cpp
 *  sdaMidiMeter
 *
 *  Created by Andy Brown on 12/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AudioControl.h"



AudioControl::AudioControl()
{
    tickCounter = 0;
    tempo = 120;
    sampleRate = 44100;
    counting = false;
    
    collector.reset(sampleRate);
    
    const String error (audioDeviceManager.initialise (0, /* number of input channels */
													   2, /* number of output channels */
													   0, /* no XML settings.. */
													   true  /* select default device on failure */));	
    if (error.isNotEmpty())
    {
        AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                     ProjectInfo::projectName,
                                     "Couldn't open an output device!\n\n" + error);
    }
    else
    {
        synthSource = new SynthAudioSource(synth, collector);
        
		audioSourcePlayer.setSource(synthSource);
    
        // start the IO device pulling its data from our callback..
        audioDeviceManager.addAudioCallback (this);
    }
    
    // Sets format manager to be able to read wav aiff flac ogg and OS specific (aac m4a wmv) etc
    formatManager.registerBasicFormats();
    
    
    
}
AudioControl::~AudioControl()
{
    audioSourcePlayer.setSource (nullptr);

    audioDeviceManager.removeAudioCallback(this);

}

void AudioControl::loadFile (const ValueTree files)
{
    for (int i = 0; i < synth.getNumSounds(); i++)
        synth.removeSound(i);
    
	//this is called when the user changes the filename in the file chooser box
    for (int i = 0; i < files.getNumChildren(); i++)
    {
        File audioFile = files.getChild(i).getProperty("File").toString();
        if(audioFile.existsAsFile())
        {
            ScopedPointer<AudioFormatReader> reader;
            reader = formatManager.createReaderFor (audioFile);
            
            if (reader != nullptr)
            {
                synth.setCurrentPlaybackSampleRate(reader->sampleRate);
                
                collector.reset(reader->sampleRate);
                
                SamplerSound* sampler = new SamplerSound (files.getChild(i).getProperty("Name").toString(),
                                                          *reader,int64(files.getChild(i).getProperty("Number")),
                                                          files.getChild(i).getProperty("Number"), 0.5, 0.5,
                                                          1);
                
                synth.addSound(sampler);
                sendChangeMessage();

            }
            else
            {
                //Reader failed
                DBG("Reader failed on " << audioFile.getFileName());
            }
        }
    }
}

bool AudioControl::loadSingleSample (ValueTree sample, int index)
{
    synth.removeSound(index);
    DBG("Name = " << sample.getProperty("Name").toString());
    
    File audioFile = sample.getProperty("File").toString();
    if(audioFile.existsAsFile())
    {
        ScopedPointer<AudioFormatReader> reader;
        reader = formatManager.createReaderFor (audioFile);
        
        if (reader != nullptr)
        {
            synth.setCurrentPlaybackSampleRate(reader->sampleRate);
            
            collector.reset(reader->sampleRate);
            
            SamplerSound* sampler = new SamplerSound (sample.getProperty("Name").toString(),
                                                      *reader,int64(sample.getProperty("Number")),
                                                      sample.getProperty("Number"), 0.5, 0.5,
                                                      1);
            
            synth.addSound(sampler);
        }
        else
            return false;
    }
    else
        return false;
}

void AudioControl::stopSamples(ValueTree samples)
{
    for (int i = 0; i < samples.getNumChildren(); i++)
    {
        MidiMessage off = MidiMessage::noteOff(1, samples.getChild(i).getProperty("Number"));
        
    }
}

void AudioControl::playSample(ValueTree sample)
{
    DBG("Play " << File(sample.getProperty("File").toString()).getFileName() <<" " <<sample.getProperty("Number").toString());
    
    MidiMessage message = MidiMessage::noteOn(1, sample.getProperty("Number"), 1.0f);
    
    message.setTimeStamp(Time::getCurrentTime().toMilliseconds());
    
    collector.addMessageToQueue(message);
}

void AudioControl::setVolume(double incomingVolume)
{
	audioSourcePlayer.setGain(incomingVolume);
}

double AudioControl::getVolume()
{
    return audioSourcePlayer.getGain();
}

//AudioCallbacks================================================================
void AudioControl::audioDeviceIOCallback (const float** inputChannelData,
										  int numInputChannels,
										  float** outputChannelData,
										  int numOutputChannels,
										  int numSamples)
{   
//    
//    float *outL;
//    float *outR;
//    
//    outL = outputChannelData[0]; 
//    outR = outputChannelData[1];

    audioSourcePlayer.audioDeviceIOCallback(inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples);
    
    if (counting) {
        double const samplesPerBeat = sampleRate * 60 / tempo;
        
        tickCounter += numSamples;
        
        if (tickCounter >= samplesPerBeat) {
            DBG("Beat");
            tickCounter = 0;
        }
    }

}


void AudioControl::audioDeviceAboutToStart (AudioIODevice* device)
{
	audioSourcePlayer.audioDeviceAboutToStart (device);
}

void AudioControl::audioDeviceStopped()
{
	audioSourcePlayer.audioDeviceStopped();
}



void AudioControl::showAudioPreferences(Component* centerComponent)
{
    AudioDeviceSelectorComponent audioSettingsComp (audioDeviceManager,
                                                    0, 2, 2, 2,
                                                    true, true, true, true);
    audioSettingsComp.setSize (500, 250);
    DialogWindow::showModalDialog ("Audio Settings", &audioSettingsComp, centerComponent, Colour (0xffeeddff), true);
    
}

