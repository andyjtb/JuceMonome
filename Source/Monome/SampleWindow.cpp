//
//  SampleWindow.cpp
//  JuceMonome
//
//  Created by Andy on 04/07/2013.
//
//

#include "SampleWindow.h"
#include <functional>

SampleWindow::SampleWindow (ValueTree sampTree)
{
    numSamples = 8;
    
    samplesTree = sampTree;
    
    for (int i = 0; i < numSamples; i++)
    {
        filenameComponent[i] = new FilenameComponent("Sample " + String(i+1), File(sampTree.getChild(i).getProperty("File").toString()), true, false, false, "*.mp3", testFile(sampTree, i), "Sample " + String(i+1));
        addAndMakeVisible(filenameComponent[i]);
    }
    
    addAndMakeVisible (&save);
    save.setButtonText ("Save");
    save.addListener (this);
    
    addAndMakeVisible (&cancel);
    cancel.setButtonText ("Cancel");
    cancel.addListener (this);
    
    setSize (520, 350);
}

SampleWindow::~SampleWindow()
{

}

String SampleWindow::testFile (ValueTree sampTree, int i)
{
    File testFile = sampTree.getChild(i).getProperty("File").toString();
    
    if (testFile.existsAsFile()) {
        return testFile.getFileName();
    }
    else
        return String::empty;
}

//==============================================================================
void SampleWindow::paint (Graphics& g)
{
    g.fillAll (Colours::white);
    
    g.setColour (Colours::black);
    g.setFont (Font (15.0000f, Font::plain));
    
    int y = 20;
    for (int i = 0; i < numSamples; i++)
    {
        g.drawText ("Sample " + String(i+1),
                0, y, 80, 30,
                Justification::centred, true);
        
        y+= 40;
    }

}

void SampleWindow::resized()
{
    int x = 95;
    int y = 25;
    
    for (int i = 0 ; i < numSamples; i++) {
        filenameComponent[i]->setBounds(x, y, 225, 25);
        
        if (i == 3)
            save.setBounds(370, y, 150, 25);
        
        if (i == 4)
            cancel.setBounds(370, y, 150, 25);
        
        y += 40;
    }
}

void SampleWindow::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == &save)
    {
        
        for (int i = 0; i < numSamples; i++)
        {
            if (filenameComponent[i]->getCurrentFile().existsAsFile()) {
                samplesTree.getChild(i).setProperty("File", filenameComponent[i]->getCurrentFile().getFullPathName(), 0);
            }
        }
        
        DialogWindow* dw = findParentComponentOfClass<DialogWindow>();
        if (dw != nullptr)
            dw->exitModalState (1);
        
    }
    else if (buttonThatWasClicked == &cancel)
    {
        DialogWindow* dw = findParentComponentOfClass<DialogWindow>();
        if (dw != nullptr)
            dw->exitModalState (0);
    }
}