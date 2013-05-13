//
//  MonomeGui.h
//  JuceMonome
//
//  Created by Andy on 26/04/2013.
//
//

#ifndef __JuceMonome__MonomeGui__
#define __JuceMonome__MonomeGui__

#include "../JuceLibraryCode/JuceHeader.h"
#include <monome.h>

struct GridPosition
{
    int x,y;
};

class MonomeGui : public Component,
                  public Button::Listener,
                  public Slider::Listener,
                  public MultiTimer,
public ComboBox::Listener
{
public:
    MonomeGui(monome_t* _monome);
    ~MonomeGui();
    
    void paint();
    void resized();
    
    void handleDown (const monome_event_t *e, void *data);
    void handleUp (const monome_event_t *e, void *data);
    
    GridPosition getGridPosition (String name);
    
    void sliderValueChanged (Slider* slider);
    void buttonClicked (Button* button);
    
    void timerCallback (int timerId);
    
    void comboBoxChanged (ComboBox *comboBoxThatHasChanged);
    
    void clear();
    void all();
    
private:
    monome_t* monome;
    
    ComboBox effects;
    ToggleButton effectsToggle;
    
    TextButton allB, clearB;
    
    Slider intensity, speed;
    ToggleButton buttonGrid[8][8];
    bool dim, movingUp;
    int movingX, lastPressed;
};

#endif /* defined(__JuceMonome__MonomeGui__) */
