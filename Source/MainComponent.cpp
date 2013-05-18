/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainContentComponent::MainContentComponent()
{
    StringPairArray monomesArray;
    getSerialPortPaths(monomesArray);
    
    if (monomesArray.size() > 0)
    {
        setSize (360, 250);
        
        addAndMakeVisible(&monomeSelect);
        monomeSelect.setBounds(0, 0, 200, 25);
        
        StringArray keys = monomesArray.getAllKeys();
        
        for (int i = 1; i <= monomesArray.size(); i++)
        {
            monomeSelect.addItem(monomesArray[keys[i-1]], i);
        }
        
        addAndMakeVisible(&connect);
        connect.setButtonText("Connect");
        connect.addListener(this);
        connect.setBounds(220, 0, 60, 20);
        connect.setConnectedEdges(Button::ConnectedOnRight);
        
        addAndMakeVisible(&disconnect);
        disconnect.setButtonText("Disconnect");
        disconnect.addListener(this);
        disconnect.setBounds(280, 0, 60, 20);
        disconnect.setConnectedEdges(Button::ConnectedOnLeft);
        
        if (monomesArray[keys[0]].contains("m40h"))
        {
            monomeSelect.setSelectedId(1);
            buttonClicked(&connect);
        }
    }
}

MainContentComponent::~MainContentComponent()
{
    
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xffeeddff));
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

}

void MainContentComponent::buttonClicked(Button* button)
{
    if (button == &connect)
    {
        /* open the monome device */
        String monomeToConnect = monomeSelect.getItemText(monomeSelect.getSelectedItemIndex());
        
        if ( !(monome = monome_open(monomeToConnect.toUTF8())))
        {
            DBG("Failed to open");
        }
        else
        {
            DBG("Opened ok");
            monome_led_all(monome, 0);
            
            monomeSelect.setEnabled(false);
            connect.setEnabled(false);
            
            monGui = new MonomeGui(monome);
            addAndMakeVisible(monGui);
            monGui->setBounds(0, 40, getWidth(), getHeight()-40);
        }
    }
    
    else if (button == &disconnect)
    {
        monomeSelect.setEnabled(true);
        connect.setEnabled(true);
        
        if (monGui != nullptr)
        {
            monGui->setVisible(false);
            monGui->clear();
        }
        
        monome_close(monome);
    }
}
