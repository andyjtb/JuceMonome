//
//  MonomeGui.cpp
//  JuceMonome
//
//  Created by Andy on 26/04/2013.
//
//

#include "Monome.h"

MonomeGui::MonomeGui(monome_t* _monome)
{
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
    
    monThread = new MonomeThread (monome, this);
    monThread->startThread();
}

MonomeGui::~MonomeGui()
{
    
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
    
    intensity.setBounds(20, getHeight()-60, x-5, 25);
    allB.setBounds(35, getHeight()-30, 50,20);
    clearB.setBounds(85, getHeight()-30, 50, 20);
}


void MonomeGui::handleDown(const monome_event_t *e)
{
    const MessageManagerLock mm;
    
    ToggleButton* b = &buttonGrid[e->grid.x][e->grid.y];
    b->setToggleState(!b->getToggleState(), true);
}

void MonomeGui::handleUp(const monome_event_t *e)
{
    
}

GridPosition MonomeGui::getGridPosition (String name)
{
    GridPosition pos;
    
    pos.x = name.fromFirstOccurrenceOf("x=", false, true).upToFirstOccurrenceOf(",", false, true).getIntValue();
    pos.y = name.fromFirstOccurrenceOf("y=", false, true).getIntValue();
    
    return pos;
}

void MonomeGui::sliderValueChanged(Slider *slider)
{
    if (slider == &intensity)
    {
        monome_led_intensity(monome, int(slider->getValue()));
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
    
    else
    {
        GridPosition currentPosition = getGridPosition(_button->getName());
        
        if (_button->getToggleState())
            monome_led_on(monome, currentPosition.x, currentPosition.y);
        else
            monome_led_off(monome, currentPosition.x, currentPosition.y);
    }
}
