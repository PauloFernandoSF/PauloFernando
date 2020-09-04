//
//  Event.h
//  EONsimulator
//
//  Created by Raul Almeida on 27/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#ifndef __EONsimulator__Event__
#define __EONsimulator__Event__

#include <stdio.h>
class Connection;
#include "Def.h"


enum EventType {UNKNOWN, Req, Desc, Exp, Comp};

////////////////////////////////////////////////////////////////////////////////////
class Event{
public:
    void setTime(TIME t);
    TIME getTime();
    void setType(EventType et);
    EventType getType();
    void setNextEvent(Event*);
    Event* getNextEvent();
    void setConnection(Connection* con);
    Connection* getConnection();
    //
    static void setRequestEvent(Event* evt, TIME t);
    static void setReleaseEvent(Event *evt, Connection* newConnection);
    //
    void print();
private:
    TIME time;
    EventType type;
    Event *nextEvent;
    Connection* connection;
};

#endif /* defined(__EONsimulator__Event__) */
