/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize (300, 200);

	/* open the monome device */
	if ( !(monome = monome_open("/dev/tty.usbserial-m40h0506")))
    {
        DBG("Failed to open");
    }
    else
    {
        DBG("Opened ok");
    }
    
	monome_led_all(monome, 0);

    monGui = new MonomeGui(monome);
    addAndMakeVisible(monGui);
    monGui->setSize(getWidth(), getHeight());
    
    monThread = new MonomeThread(monome, monGui);
    monThread->startThread();
}

MainContentComponent::~MainContentComponent()
{
    if (monThread->isThreadRunning())
    {
        monThread->stopThread(100);
        delete monThread;
    }
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

