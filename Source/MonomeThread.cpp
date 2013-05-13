//
//  MonomeThread.cpp
//  JuceMonome
//
//  Created by Andy on 20/04/2013.
//
//

#include "MonomeThread.h"

class MainContentComponent;

MonomeThread::MonomeThread(monome_t* _monome, MonomeGui* _gui) : Thread ("MonomeListener")
{
    statics::gui.set(_gui, false);
    
    monome = _monome;
    monome_register_handler(monome, MONOME_BUTTON_DOWN, handle_down, NULL);
    monome_register_handler(monome, MONOME_BUTTON_UP, handle_up, NULL);
}

MonomeThread::~MonomeThread()
{

}

void MonomeThread::run()
{
    monome_event_loop(monome);
}

void MonomeThread::handle_down(const monome_event_t *e, void *data) {
    statics::gui->handleDown(e,data);
}

void MonomeThread::handle_up(const monome_event_t *e, void *data) {
    statics::gui->handleUp(e,data);
}