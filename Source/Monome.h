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

class MonomeThread;

struct GridPosition
{
    int x,y;
};

class MonomeGui : public Component,
                  public Button::Listener,
                  public Slider::Listener
{
public:
    MonomeGui(monome_t* _monome);
    ~MonomeGui();
    
    void paint();
    void resized();
    
    void handleDown (const monome_event_t *e);
    void handleUp (const monome_event_t *e);
    
    GridPosition getGridPosition (String name);
    
    void sliderValueChanged (Slider* slider);
    void buttonClicked (Button* button);
    
    void clear();
    void all();
    
private:
    monome_t* monome;
    ScopedPointer<MonomeThread> monThread;
    
    TextButton allB, clearB;
    Slider intensity;
    ToggleButton buttonGrid[8][8];
};

class MonomeThread   : public Thread
{
public:
    //==============================================================================
    MonomeThread(monome_t* _monome, MonomeGui* _gui) : Thread ("MonomeListener")
    {
        monome = _monome;
        monome_register_handler(monome, MONOME_BUTTON_DOWN, handle_down, _gui);
        monome_register_handler(monome, MONOME_BUTTON_UP, handle_up, _gui);
    }
    
    ~MonomeThread()
    {
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


#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>

static void getSerialPortPaths(StringPairArray& SerialPortPaths)
{
	io_iterator_t matchingServices;
	mach_port_t         masterPort;
    CFMutableDictionaryRef  classesToMatch;
	io_object_t     modemService;
	char deviceFilePath[512];
	char deviceFriendly[1024];
    if (KERN_SUCCESS != IOMasterPort(MACH_PORT_NULL, &masterPort))
    {
        DBG("SerialPort::getSerialPortPaths : IOMasterPort failed");

    }
    classesToMatch = IOServiceMatching(kIOSerialBSDServiceValue);
    if (classesToMatch == NULL)
	{
		DBG("SerialPort::getSerialPortPaths : IOServiceMatching failed");
	
	}
	CFDictionarySetValue(classesToMatch, CFSTR(kIOSerialBSDTypeKey), CFSTR(kIOSerialBSDRS232Type));
	if (KERN_SUCCESS != IOServiceGetMatchingServices(masterPort, classesToMatch, &matchingServices))
	{
		DBG("SerialPort::getSerialPortPaths : IOServiceGetMatchingServices failed");
    
	}
	while ((modemService = IOIteratorNext(matchingServices)))
	{
		CFTypeRef   deviceFilePathAsCFString;
		CFTypeRef   deviceFriendlyAsCFString;
		deviceFilePathAsCFString = IORegistryEntryCreateCFProperty(modemService,CFSTR(kIODialinDeviceKey), kCFAllocatorDefault, 0);
		deviceFriendlyAsCFString = IORegistryEntryCreateCFProperty(modemService,CFSTR(kIOTTYDeviceKey), kCFAllocatorDefault, 0);
		if(deviceFilePathAsCFString)
		{
			if(CFStringGetCString((const __CFString*)deviceFilePathAsCFString, deviceFilePath, 512, kCFStringEncodingASCII)
               && CFStringGetCString((const __CFString*)deviceFriendlyAsCFString, deviceFriendly, 1024, kCFStringEncodingASCII) )
				SerialPortPaths.set(deviceFriendly, deviceFilePath);
			CFRelease(deviceFilePathAsCFString);
			CFRelease(deviceFriendlyAsCFString);
		}
	}
	IOObjectRelease(modemService);
	
}

#endif /* defined(__JuceMonome__MonomeGui__) */
