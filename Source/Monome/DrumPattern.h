//
//  DrumPattern.h
//  JuceMonome
//
//  Created by Andy on 01/06/2013.
//
//

#ifndef __JuceMonome__DrumPattern__
#define __JuceMonome__DrumPattern__

#include "../JuceLibraryCode/JuceHeader.h"
#include <monome.h>
#include "Monome.h"
#include "MonomeUtility.h"

class MonomeThread;

class DrumPattern : public Component,
                    public Timer,
                    public Button::Listener
{
public:
    DrumPattern(monome_t* _monome)
    {
        addAndMakeVisible (&bpmSlider);
        bpmSlider.setRange (1, 360, 0.01);
        bpmSlider.setSliderStyle (Slider::IncDecButtons);
        bpmSlider.setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
        bpmSlider.setValue(120, dontSendNotification);
        
        addAndMakeVisible (&startButton);
        startButton.setButtonText ("Start");
        startButton.setConnectedEdges (Button::ConnectedOnRight);
        startButton.addListener (this);
        
        addAndMakeVisible (&stopButton);
        stopButton.setButtonText ("Stop");
        stopButton.setConnectedEdges (Button::ConnectedOnLeft);
        stopButton.addListener (this);
        
        monome = _monome;
        
        monThread = new MonomeThread (monome, this);
        monThread->startThread();
    }
    ~DrumPattern(){}
    
    void resized()
    {
        bpmSlider.setBounds (16, 32, 150, 24);
        startButton.setBounds (192, 32, 60, 25);
        stopButton.setBounds (251, 32, 60, 25);
    }
    
    void paint (Graphics& g)
    {
        g.setColour (Colours::black);
        g.setFont (Font (15.0000f, Font::plain));
        g.drawText ("BPM:",
                    0, 0, 40, 30,
                    Justification::centred, true);
    }
    
    void timerCallback()
    {
        
    }
    
    void buttonClicked (Button* b)
    {
        
    }
    
    void handleDown (monome_event* e)
    {
        
    }
    
    void handleUp (monome_event* e)
    {
        
    }
private:
    monome_t* monome;
    ScopedPointer<MonomeThread> monThread;
    
    Slider bpmSlider;
    TextButton startButton;
    TextButton stopButton;

};

#endif /* defined(__JuceMonome__DrumPattern__) */
