//
//  MonomeGui.h
//  JuceMonome
//
//  Created by Andy on 01/06/2013.
//
//

#ifndef __JuceMonome__MonomeGui__
#define __JuceMonome__MonomeGui__

#include "../JuceLibraryCode/JuceHeader.h"
#include <monome.h>
#include "Monome.h"
#include "MonomeUtility.h"

#include "SampleWindow.h"

class MonomeThread;

class MonomeGui : public Component,
                  public Button::Listener,
                  public Slider::Listener,
                  public MultiTimer
{
public:
    MonomeGui(monome_t* _monome);
    ~MonomeGui();
    
    void paint(Graphics& g);
    void resized();
    
    void handleDown (const monome_event_t *e);
    void handleUp (const monome_event_t *e);
    
    void sliderValueChanged (Slider* slider);
    void buttonClicked (Button* button);
    
    void clear();
    void all();
    
    void lightOn (int x, int y) { //buttonGrid[x][y].setToggleState(true, true);
                                    monome_led_on(monome, x, y);}
    void lightOff (int x, int y) { //buttonGrid[x][y].setToggleState(false, true);
                                    monome_led_off(monome, x, y); }
    void toggleLight (int x, int y) { buttonGrid[x][y].setToggleState(!buttonGrid[x][y].getToggleState(), true); }
    
    void timerCallback (int timer);
    
private:
    monome_t* monome;
    ScopedPointer<MonomeThread> monThread;
    
    TextButton allB, clearB, startB, stopB, samplesB;
    Slider intensity, bpmSlider;
    ToggleButton buttonGrid[8][8];
    
//    
//    OwnedArray<MonomeBehaviour> behaviours;
    ComboBox behaviourCombo;
    
    int x, numTimers;
    
    ValueTree samplesTree;
    
};

#endif /* defined(__JuceMonome__MonomeGui__) */
