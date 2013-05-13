//
//  MonomeThread.h
//  JuceMonome
//
//  Created by Andy on 20/04/2013.
//
//

#ifndef __JuceMonome__MonomeThread__
#define __JuceMonome__MonomeThread__

#include "../JuceLibraryCode/JuceHeader.h"
#include "MonomeGui.h"
#include <monome.h>

namespace statics
{
    static OptionalScopedPointer<MonomeGui> gui;
}

class MonomeThread   : public Thread
{
public:
    //==============================================================================
    MonomeThread(monome_t* _monome, MonomeGui* _gui);
    ~MonomeThread();
    
    void run();
    
    static void handle_down(const monome_event_t *e, void *data);
    static void handle_up(const monome_event_t *e, void *data);

private:
    monome_t* monome;
//    OptionalScopedPointer<MonomeGui> gui;
};
#endif /* defined(__JuceMonome__MonomeThread__) */
