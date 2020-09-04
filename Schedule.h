//
//  Schedule.h
//  EONsimulator
//
//  Created by Raul Almeida on 27/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#ifndef __EONsimulator__Schedule__
#define __EONsimulator__Schedule__



#include <stdio.h>
#include <assert.h>
#include "Connection.h"
#include "Event.h"

////////////////////////////////////////////////////////////////////////////////////
class Schedule{
public:
    static void initialise();
    //
    static TIME getSimTime();
    //
    static Event* getCurrentEvent();
    static void scheduleEvent(Event* evt);
    //
    static bool isEmpty();
    //
    static void printAllEvents();
private:
    static TIME simTime;
    static Event* firstEvent;
protected:
    static void setSimTime(TIME t);
};

#endif /* defined(__EONsimulator__Schedule__) */
