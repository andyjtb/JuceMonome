//
//  MonomeBehaviours.h
//  JuceMonome
//
//  Created by Andy on 19/05/2013.
//
//

#ifndef JuceMonome_Behaviours_h
#define JuceMonome_Behaviours_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "MonomeUtility.h"

class MonomeBehaviour  //public MultiTimer,
{
public:
    MonomeBehaviour(String _name)
    {
        name = _name;
    }
    
    virtual ~MonomeBehaviour() {}
    
    virtual void buttonDown(ToggleButton* buttonPressed) = 0;
    virtual void buttonUp(ToggleButton* buttonPressed) = 0;
    
//    virtual void timerCallback(int timerId) = 0;
    
//    virtual void acivate() = 0;
//    virtual void stop() = 0;
    
    String getName() { return name; }
    
protected:
    String name;
};

class MonomeToggle : public MonomeBehaviour
{
public:
    MonomeToggle() : MonomeBehaviour("Toggle")
    {
        
    }
    
    ~MonomeToggle() {}
    
    void buttonDown(ToggleButton* b)
    {
        b->setToggleState(!b->getToggleState(), true);
    }
    
    void buttonUp (ToggleButton* buttonPressed)
    {

    }
};

class MonomeHold : public MonomeBehaviour
{
public:
    MonomeHold() : MonomeBehaviour("Hold")
    {
        
    }
    
    ~MonomeHold() {}
    
    void buttonDown(ToggleButton* b)
    {
        b->setToggleState(true, true);
    }
    
    void buttonUp (ToggleButton* b)
    {
        b->setToggleState(false, true);
    }
};

class MonomeGui;
//class MonomeSquare : public MonomeBehaviour
//{
//public:
//    MonomeSquare() : MonomeBehaviour("Square")
//    {
//        
//    }
//    
//    ~MonomeSquare() {}
//    
//    void buttonDown(ToggleButton* b)
//    {
//        if (parent != nullptr)
//        {
//            ToggleButton buttonGrid[8][8] = parent->getButtonGrid();
//        }
//        b->setToggleState(true, true);
//    }
//    
//    void buttonUp (ToggleButton* b)
//    {
//        b->setToggleState(false, true);
//    }
//    
//    void setParent (MonomeGui* _parent)
//    {
//        parent = _parent;
//    }
//    
//private:
//    MonomeGui* parent;
//};

#endif
