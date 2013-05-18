/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef __MAINCOMPONENT_H_F14FD3C1__
#define __MAINCOMPONENT_H_F14FD3C1__

#include "../JuceLibraryCode/JuceHeader.h"

//Juce class
#include "Monome.h"
//Libmonome
#include <monome.h>


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   :  public Component,
                                public Button::Listener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();
    
    void buttonClicked (Button* button);
    
private:
    monome_t* monome;
    
    ComboBox monomeSelect;
    TextButton connect, disconnect;
    
    ScopedPointer<MonomeGui> monGui;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // __MAINCOMPONENT_H_F14FD3C1__
