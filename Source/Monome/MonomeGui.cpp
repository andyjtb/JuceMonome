//
//  MonomeGui.cpp
//  JuceMonome
//
//  Created by Andy on 26/04/2013.
//
//

#include "Monome.h"

class DrumPattern;

MonomeGui::MonomeGui(monome_t* _monome)
{
    samplesTree = ValueTree("SAMPLES");
    
    addAndMakeVisible(&intensity);
    intensity.setRange(0, 15,1);
    intensity.setValue(intensity.getMaximum());
    intensity.addListener(this);
    intensity.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    
    monome = _monome;
    
    for (int i = 0; i < monome_get_cols(monome); i++)
    {
        for (int j = 0; j < monome_get_rows(monome); j++)
        {
            ToggleButton* button = &buttonGrid[i][j];
            String name = ("x=" + String(i));
            name << ",y=" << j;
            button->setName(name);
            button->addListener(this);
            addAndMakeVisible(button);
        }
    }
    
    addAndMakeVisible(&allB);
    allB.setButtonText("All");
    allB.addListener(this);
    allB.setConnectedEdges(2);
    
    addAndMakeVisible(&clearB);
    clearB.setButtonText("Clear");
    clearB.addListener(this);
    clearB.setConnectedEdges(1);
    
    addAndMakeVisible(&startB);
    startB.setButtonText("Start");
    startB.addListener(this);
    startB.setConnectedEdges(2);
    
    addAndMakeVisible(&stopB);
    stopB.setButtonText("Stop");
    stopB.addListener(this);
    stopB.setConnectedEdges(1);
    
    addAndMakeVisible(&samplesB);
    samplesB.setButtonText("Load Samples");
    samplesB.addListener(this);
    
    addAndMakeVisible(&bpmSlider);
    bpmSlider.setRange (1, 360, 1
                        );
    bpmSlider.setSliderStyle (Slider::IncDecButtons);
    bpmSlider.setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    bpmSlider.setValue(120, dontSendNotification);
    
    monThread = new MonomeThread (monome, this);
    monThread->startThread();
    
    x = 0;
    numTimers = 2;
}

MonomeGui::~MonomeGui()
{
}

void MonomeGui::paint(Graphics& g)
{
    g.setColour (Colours::black);
    g.setFont (Font (12.0000f, Font::plain));
    g.drawText ("Monome grid",
                buttonGrid[0][0].getX()-3, buttonGrid[0][0].getY()-15, 80, 16,
                Justification::centred, true);
    
    g.drawText ("LED Brightness",
                intensity.getX(), intensity.getY()-15, 80, 16,
                Justification::centred, true);
}

void MonomeGui::resized()
{
    int x = 5;
    
    for (int i = 0; i < 8; i++)
    {
        int y = 10;
        x += 15;
        for (int j = 0; j < 8; j++)
        {
            buttonGrid[i][j].setBounds(x, y, 15, 15);
            y += 15;
        }
        
    }
    
    intensity.setBounds(20, getHeight()-60, x-5, 15);
    allB.setBounds(35, getHeight()-30, 50,20);
    clearB.setBounds(85, getHeight()-30, 50, 20);
    
    startB.setBounds(x + 30, buttonGrid[0][0].getY(), 50, 20);
    stopB.setBounds(x + 80, buttonGrid[0][0].getY(), 50, 20);
    bpmSlider.setBounds(x + 30, buttonGrid[0][0].getY() + 30, 140, 20);
    samplesB.setBounds(x+ 45, bpmSlider.getY() + 30, 100, 20);
    
}


void MonomeGui::handleDown(const monome_event_t *e)
{
    const MessageManagerLock mm;
    
    ToggleButton* b = &buttonGrid[e->grid.x][e->grid.y];
    b->setToggleState( ! b->getToggleState(), true);
//    behaviours[behaviourCombo.getSelectedId()-1]->buttonDown(b);
}

void MonomeGui::handleUp(const monome_event_t *e)
{
    const MessageManagerLock mm;
    
    ToggleButton* b = &buttonGrid[e->grid.x][e->grid.y];
    //    b->setToggleState(!b->getToggleState(), true);
    //     b->setToggleState(false, true);
//    behaviours[behaviourCombo.getSelectedId()-1]->buttonUp(b);
}

void MonomeGui::sliderValueChanged(Slider *slider)
{
    if (slider == &intensity)
    {
        monome_led_intensity(monome, int(slider->getValue()));
    }
    
    else if (slider == &bpmSlider)
    {
        for (int i = 0; i < numTimers; i++) {
            stopTimer(i);
            startTimer(i, 60000/bpmSlider.getValue());
        }
    }
}

void MonomeGui::clear()
{
    for (int i = 0; i < monome_get_cols(monome); i++)
    {
        for (int j = 0; j < monome_get_rows(monome); j++)
        {
            buttonGrid[i][j].setToggleState(false, true);
        }
    }
}

void MonomeGui::all()
{
    for (int i = 0; i < monome_get_cols(monome); i++)
    {
        for (int j = 0; j < monome_get_rows(monome); j++)
        {
            buttonGrid[i][j].setToggleState(true, true);
        }
    }
}

void MonomeGui::buttonClicked (Button* _button)
{
    if (_button == &clearB)
    {
        clear();
    }
    else if (_button == &allB)
    {
        all();
    }
    
    else if (_button == & startB)
        for (int i = 0; i < numTimers; i++)
            startTimer(i, 60000/bpmSlider.getValue());
    
    else if (_button == &stopB)
        for (int i = 0; i < numTimers; i++)
            stopTimer(i);
    
    else if (_button == &samplesB)
    {
        
        //Display auth dialog
        SampleWindow* sampleWindow;
        sampleWindow = new SampleWindow(samplesTree);
        
        DialogWindow::LaunchOptions o;
        
        o.content.setOwned (sampleWindow);
        o.content->setSize (sampleWindow->getWidth(), sampleWindow->getHeight());
        
        o.dialogTitle                   = "Select Samples";
        o.dialogBackgroundColour        = Colours::lightgrey;
        o.escapeKeyTriggersCloseButton  = true;
        o.useNativeTitleBar             = false;
        o.resizable                     = false;
        
        if (o.runModal() != 0 )
        {
            DBG ("SAVE");
        }
    }
    
    else
    {
        GridPosition currentPosition;
        MonomeUtility::getGridPosition(_button->getName(), currentPosition);
        
        if (_button->getToggleState())
            monome_led_on(monome, currentPosition.x, currentPosition.y);
        else
            monome_led_off(monome, currentPosition.x, currentPosition.y);
    }
}

void MonomeGui::timerCallback(int timer)
{
    if (timer == 0) {
        //Going across
        if (x > 7) {
            x = 0;
        }
        
        for (int y = 0; y < monome_get_rows(monome); y++)
            lightOn(x, y);
        
        startTimer(1, 200);
        
    }
    
    if (timer == 1)
    {
        if (x > 7) {
            x = 0;
        }
        
        for (int y = 0; y < monome_get_rows(monome); y++)
        {
            if (! buttonGrid[x][y].getToggleState()) {
                lightOff(x, y);
            }
        }
        
        x++;
        
        stopTimer(1);
    }
}
