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
#include "AudioControl.h"

class SampleWindow  : public Component,
                      public ButtonListener,
                      public FilenameComponentListener
{
public:
    //==============================================================================
    SampleWindow (ValueTree sampTree, AudioControl* audioCont);
    ~SampleWindow();
    
    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    
    void filenameComponentChanged (FilenameComponent *fileComponentThatHasChanged);
    
private:
    //===========================================================================
    
    int numSamples;
    
    ValueTree samplesTree;
    
    ScopedPointer<TextEditor> sampleName[8];
    ScopedPointer<FilenameComponent> filenameComponent[8];
    ScopedPointer<Slider> midiNum[8];
    ScopedPointer<TextButton> testButton[8];
    
    OptionalScopedPointer<AudioControl> audioControl;
    
    TextButton save;
    TextButton cancel;
};

#endif /* defined(__JuceMonome__SampleWindow__) */
