//
//  MonomeUtility.h
//  JuceMonome
//
//  Created by Andy on 19/05/2013.
//
//

#ifndef JuceMonome_MonomeUtility_h
#define JuceMonome_MonomeUtility_h

#include "../JuceLibraryCode/JuceHeader.h"

struct GridPosition
{
    int x,y;
};

namespace MonomeUtility
{
    static void getGridPosition (String name, GridPosition& pos)
    {
        pos.x = name.fromFirstOccurrenceOf("x=", false, true).upToFirstOccurrenceOf(",", false, true).getIntValue();
        pos.y = name.fromFirstOccurrenceOf("y=", false, true).getIntValue();
    }
    
#if JUCE_MAC
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
#endif //Juce mac
    
#if JUCE_WINDOWS
#include <windows.h>
#include <stdio.h>
    
    //the enumneration functions require Setupapi.lib and Setupapi.dll
    //- if this dependency is not wanted, undef USE_SETUPAPI, and the
    //SerialPort::getSerialPortPaths() will simply return an empty StringPairArray
#define USE_SETUPAPI
#ifdef USE_SETUPAPI
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
    // The following define is from ntddser.h in the DDK. It is
    // needed for serial port enumeration.
#ifndef GUID_CLASS_COMPORT
    DEFINE_GUID(GUID_CLASS_COMPORT, 0x86e0d1e0L, 0x8089, 0x11d0, 0x9c, 0xe4, \
                0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73);
#endif
#endif
    
    static void SerialPort::getSerialPortPaths(StringPairArray& SerialPortPaths)
    {
#ifdef USE_SETUPAPI
        HDEVINFO hDevInfo;
        SP_DEVINFO_DATA DeviceInfoData;
        DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        DWORD dwDetDataSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) + 256;
        SP_DEVICE_INTERFACE_DETAIL_DATA *pDetData = (SP_DEVICE_INTERFACE_DETAIL_DATA*) new char[dwDetDataSize];
        pDetData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
        SP_DEVICE_INTERFACE_DATA ifcData;
        ifcData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
        DWORD i;
        GUID *guidDev = (GUID*) &GUID_CLASS_COMPORT;
        if(INVALID_HANDLE_VALUE == (hDevInfo = SetupDiGetClassDevs(guidDev, 0, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE )))
        {
            DBG("SerialPort::getSerialPortPaths :: SetupDiGetClassDevs failed");
        }
        for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,&DeviceInfoData);i++)
        {
            DWORD DataT;
            char buffer[1024];
            DWORD buffersize = 1024;
            DWORD buffersizerequired=0;
            SetupDiEnumDeviceInterfaces(hDevInfo, NULL, guidDev, i, &ifcData);
            SetupDiGetDeviceInterfaceDetail(hDevInfo, &ifcData, pDetData, dwDetDataSize, NULL, &DeviceInfoData);
            SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_FRIENDLYNAME, &DataT, (PBYTE)buffer, buffersize, &buffersizerequired);
            SerialPortPaths.set(buffer, pDetData->DevicePath);		
        }
        SetupDiDestroyDeviceInfoList(hDevInfo);
        delete[] pDetData;
#endif// USE_SETUPAPI
    }
#endif //Juce windows
}

#endif
