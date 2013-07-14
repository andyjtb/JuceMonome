//
//  SampleWindow.cpp
//  JuceMonome
//
//  Created by Andy on 04/07/2013.
//
//

#include "SampleWindow.h"
#include <functional>

SampleWindow::SampleWindow (ValueTree sampTree, AudioControl* audioCont)
{
    audioControl.set(audioCont, false);
    
    numSamples = 8;
    
    samplesTree = sampTree;
    
    DBG("Num " << samplesTree.getNumChildren());
    
    bool makeChildren;
    samplesTree.getNumChildren() > 0 ? makeChildren = false : makeChildren = true;
    
    for (int i = 0; i < numSamples; i++)
    {
        //INIT Valuetree
        if (makeChildren)
        {
            ValueTree sample("SAMPLE");
            sample.setProperty("Name", "Sample"+String(i), 0);
            samplesTree.addChild(sample, i, 0);
        }
        
        //INIT COMPONENTS
        filenameComponent[i] = new FilenameComponent("Sample " + String(i+1), File(sampTree.getChild(i).getProperty("File").toString()), true, false, false, audioControl->getRegisteredFormatWildcard(), "", "Sample " + String(i+1));
        filenameComponent[i]->addListener(this);
        addAndMakeVisible(filenameComponent[i]);
        
        sampleName[i] = new TextEditor(samplesTree.getChild(i).getProperty("Name").toString());
        
        if (sampleName[i]->getName() != String::empty)
            sampleName[i]->setText(sampleName[i]->getName());
        else
            sampleName[i]->setTextToShowWhenEmpty("Sample "+String(i), Colours::lightgrey);
        
        addAndMakeVisible(sampleName[i]);
        
        addAndMakeVisible (midiNum[i] = new Slider (String::empty));
        midiNum[i]->setRange (1, 127, 1);
        midiNum[i]->setSliderStyle (Slider::IncDecButtons);
        midiNum[i]->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
        if (samplesTree.getChild(i).getProperty("Number"))
            midiNum[i]->setValue(samplesTree.getChild(i).getProperty("Number"));
        else
            midiNum[i]->setValue(i+1);
        
        addAndMakeVisible(testButton[i] = new TextButton ());
        testButton[i]->setButtonText("Test");
        testButton[i]->addListener(this);
        testButton[i]->setEnabled(true);
    }
    
    addAndMakeVisible (&save);
    save.setButtonText ("Save");
    save.addListener (this);
    
    addAndMakeVisible (&cancel);
    cancel.setButtonText ("Cancel");
    cancel.addListener (this);
    
    setSize (620, 350);
}

SampleWindow::~SampleWindow()
{

}

//==============================================================================
void SampleWindow::paint (Graphics& g)
{
    g.fillAll (Colours::white);
    
    g.setColour (Colours::black);
    g.setFont (Font (15.0000f, Font::plain));

}

void SampleWindow::resized()
{
    int y = 25;
    
    for (int i = 0 ; i < numSamples; i++) {
        filenameComponent[i]->setBounds(95, y, 225, 25);
        sampleName[i]->setBounds(0, y, 80, 25);
        midiNum[i]->setBounds(330, y, 75, 20);
        testButton[i]->setBounds(420, y, 75, 25);
        
        if (i == 3)
            save.setBounds(510, y, 100, 25);
        
        if (i == 4)
            cancel.setBounds(510, y, 100, 25);
        
        y += 40;
    }
}

void SampleWindow::filenameComponentChanged (FilenameComponent *fileComponentThatHasChanged)
{
    int i = -1;
    for (i = 0; i < numSamples; i++)
        if (fileComponentThatHasChanged == filenameComponent[i])
            break;
    
    if (i != -1)
    {
        if (sampleName[i]->getText() == String::empty)
            samplesTree.getChild(i).setProperty("Name", "Sample" + String(i), 0);
        else
            samplesTree.getChild(i).setProperty("Name", sampleName[i]->getText(), 0);
        
        samplesTree.getChild(i).setProperty("Number", midiNum[i]->getValue(), 0);
        samplesTree.getChild(i).setProperty("File", filenameComponent[i]->getCurrentFile().getFullPathName(), 0);
        
        if (audioControl->loadSingleSample(samplesTree.getChild(i), i))
            testButton[i]->setEnabled(true);
    }
}

void SampleWindow::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == &save)
    {
        
        for (int i = 0; i < numSamples; i++)
        {
            ValueTree sample = samplesTree.getChild(i);
            
            sample.setProperty("Name", sampleName[i]->getText(), 0);
            sample.setProperty("Number", midiNum[i]->getValue(), 0);
        
            if (filenameComponent[i]->getCurrentFile().exists())
                sample.setProperty("File", filenameComponent[i]->getCurrentFile().getFullPathName(), 0);
            else
                sample.setProperty("File", String::empty, 0);
        }
        
        ScopedPointer<XmlElement> samples;
        samples = samplesTree.createXml();
        
        File settings ("~/Desktop/Programming/Monome/Settings.xml");
        samples->writeToFile(settings, "");
        
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
    
    else
    {
        //Test Button
        int i = -1;
        for (i = 0; i < numSamples; i++)
        {
            if (buttonThatWasClicked == testButton[i])
            {
                break;
            }
        }
        
        if (i != -1)
        {
            audioControl->playSample(samplesTree.getChild(i));
        }
    }
}