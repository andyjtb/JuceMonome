//
//  MonomeGui.cpp
//  JuceMonome
//
//  Created by Andy on 26/04/2013.
//
//

#include "MonomeGui.h"

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
    allB.setButtonText("allB");
    allB.addListener(this);
    allB.setConnectedEdges(2);
    
    addAndMakeVisible(&clearB);
    clearB.setButtonText("Clear");
    clearB.addListener(this);
    clearB.setConnectedEdges(1);
    
    addAndMakeVisible(&effects);
    effects.addListener(this);
    effects.addItem("Dim", 1);
    effects.addItem("Moving Box", 2);
    
    addAndMakeVisible(&effectsToggle);
    effectsToggle.setButtonText("Effects Toggle");
    effectsToggle.addListener(this);
    
    addAndMakeVisible(&speed);
    speed.addListener(this);
    speed.setRange(10, 100);
    speed.setValue(50);
    speed.setSliderStyle(Slider::LinearBar);
    speed.setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    
    dim = movingUp = true;
    movingX = -1;
    lastPressed = 0;
    //startTimer(1, 50);
}

MonomeGui::~MonomeGui()
{
    
}

void MonomeGui::resized()
{
    intensity.setBounds(0, 0, getWidth(), 25);
    
    int x = 5;
    
    for (int i = 0; i < 8; i++)
    {
        int y = 40;
        x += 15;
        for (int j = 0; j < 8; j++)
        {
            buttonGrid[i][j].setBounds(x, y, 15, 15);
            y += 15;
        }
        
    }
    
    allB.setBounds(75, getHeight()-30, 50,20);
    clearB.setBounds(125, getHeight()-30, 50, 20);
    
    effects.setBounds(getWidth()/2, getHeight()/2-50, 100, 20);
    effectsToggle.setBounds(getWidth()/2, getHeight()/2-20, 100, 20);
    speed.setBounds(getWidth()/2, getHeight()/2+40, 100, 20);
}

void MonomeGui::comboBoxChanged (ComboBox *comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &effects)
    {
        if (isTimerRunning(effects.getSelectedId()))
            effectsToggle.setToggleState(true, true);
        else
            effectsToggle.setToggleState(false, true);
    }
}

void MonomeGui::timerCallback(int timerId)
{
    if (timerId == 1)
    {
        if (intensity.getValue() == 1)
            dim = false;
        
        if (intensity.getValue() >= 15)
            dim = true;
        
        if (dim)
            intensity.setValue(intensity.getValue()-1);
        else
            intensity.setValue(intensity.getValue()+1);
    }
    
    else if (timerId == 2)
    {
        if (movingX == 7)
        {
            movingUp = false;
        }
        if (movingX < 1) {
            movingUp = true;
        }
        
        if (movingUp)
        {
            buttonGrid[movingX][lastPressed].setToggleState(true, true);
            movingX++;
        }
        else
        {
           buttonGrid[movingX][lastPressed].setToggleState(false, true);
            movingX--;
        }
 
    }
}

void MonomeGui::handleDown(const monome_event_t *e, void *data)
{
    const MessageManagerLock mm;
    
    //int x = e->grid.x;
    lastPressed = e->grid.y;
    
    //buttonGrid[x][y].setToggleState(!buttonGrid[x][y].getToggleState(), true);
}

void MonomeGui::handleUp(const monome_event_t *e, void *data)
{
    
}

GridPosition MonomeGui::getGridPosition (String name)
{
    //String name = button->getName();
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
    
    else if (_button == &effectsToggle)
    {
        if (effectsToggle.getToggleState())
        {
            startTimer(effects.getSelectedId(), speed.getValue());
            if (effects.getSelectedId() == 2) {
                clear();
                movingX = 0;
                buttonGrid[movingX][0].setToggleState(true, true);
            }
        }
        else
            stopTimer(effects.getSelectedId());
    }
    else
    {
    GridPosition currentPosition = getGridPosition(_button->getName());
    ToggleButton* button = &buttonGrid[currentPosition.x][currentPosition.y];
    
    if (button->getToggleState())
        monome_led_on(monome, currentPosition.x, currentPosition.y);
    else
        monome_led_off(monome, currentPosition.x, currentPosition.y);
    }
}
