//
//  Monome.h
//  JuceMonome
//
//  Created by Andy on 26/04/2013.
//
//

#ifndef __JuceMonome__Monome__
#define __JuceMonome__Monome__

#include "../JuceLibraryCode/JuceHeader.h"
#include <monome.h>
#include "MonomeUtility.h"
#include "MonomeGui.h"

class MonomeThread   : public Thread
{
public:
    //==============================================================================
    MonomeThread(monome_t* _monome, void* _gui) : Thread ("MonomeListener")
    {
        monome = _monome;
        monome_register_handler(monome, MONOME_BUTTON_DOWN, handle_down, _gui);
        monome_register_handler(monome, MONOME_BUTTON_UP, handle_up, _gui);
    }
    
    ~MonomeThread()
    {
        monome_unregister_handler(monome, MONOME_BUTTON_DOWN);
        monome_unregister_handler(monome, MONOME_BUTTON_UP);
        
        monome_close(monome);
        
        stopThread (200);
    }
    
    void run()
    {
        monome_event_loop(monome);
    }
    
    static void handle_down(const monome_event_t *e, void *data)
    {
        static_cast<MonomeGui*>(data)->handleDown(e);
    }
    
    static void handle_up(const monome_event_t *e, void *data)
    {
        static_cast<MonomeGui*>(data)->handleUp(e);
    }
    
private:
    monome_t* monome;
};


/**Create a instance of Juce Monome and it will deal with everything within it, control the behaviour in MonomeGui
*/
class JuceMonome :  public Component,
                    public Button::Listener
{
public:
    JuceMonome()
    {
        StringPairArray monomesArray;
        MonomeUtility::getSerialPortPaths(monomesArray);
        
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
    ~JuceMonome()
    {}
    
    void paint (Graphics&)
    {}
    void resized()
    {}
    
    void buttonClicked(Button* button)
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
        }
    }
    
private:
    monome_t* monome;
    
    ComboBox monomeSelect;
    TextButton connect, disconnect;
    
    ScopedPointer<MonomeGui> monGui;
    
    //ScopedPointer<DrumPattern> monGui;
};
#endif /* defined(__JuceMonome__MonomeGui__) */
