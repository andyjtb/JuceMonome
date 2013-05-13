/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef __MAINCOMPONENT_H_F14FD3C1__
#define __MAINCOMPONENT_H_F14FD3C1__

#include "../JuceLibraryCode/JuceHeader.h"
#include "MonomeThread.h"
#include "MonomeGui.h"
#include <monome.h>


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();
    
private:
    monome_t* monome;

//    unsigned int grid[8][8] = { [0 ... 7][0 ... 7] = 0 };

    MonomeThread* monThread;
    ScopedPointer<MonomeGui> monGui;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // __MAINCOMPONENT_H_F14FD3C1__
