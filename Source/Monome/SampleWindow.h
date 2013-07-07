//
//  SampleWindow.h
//  JuceMonome
//
//  Created by Andy on 04/07/2013.
//
//

#ifndef __JuceMonome__SampleWindow__
#define __JuceMonome__SampleWindow__

#include "../JuceLibraryCode/JuceHeader.h"

class SampleWindow  : public Component,
                      public ButtonListener
{
public:
    //==============================================================================
    SampleWindow (ValueTree sampTree);
    ~SampleWindow();
    
    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    
    String testFile(ValueTree sampTree, int i);
    
private:
    //===========================================================================
    
    int numSamples;
    
    ValueTree samplesTree;
    
    ScopedPointer<FilenameComponent> filenameComponent[8];
    
    TextButton save;
    TextButton cancel;
};

#endif /* defined(__JuceMonome__SampleWindow__) */
