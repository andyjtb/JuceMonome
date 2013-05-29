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

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   :  public Component
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();
    
private:
    JuceMonome juceMonome;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // __MAINCOMPONENT_H_F14FD3C1__
