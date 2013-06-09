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
#include "MonomeBehaviour.h"

class MonomeThread;

class MonomeGui : public Component,
                  public Button::Listener,
                  public Slider::Listener
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
    
    // ToggleButton* getButtonGrid() { return &buttonGrid; }
    
private:
    monome_t* monome;
    ScopedPointer<MonomeThread> monThread;
    
    TextButton allB, clearB;
    Slider intensity;
    ToggleButton buttonGrid[8][8];
    
    OwnedArray<MonomeBehaviour> behaviours;
    ComboBox behaviourCombo;
};

#endif /* defined(__JuceMonome__MonomeGui__) */
