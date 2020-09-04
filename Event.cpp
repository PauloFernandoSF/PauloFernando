//
//  Event.cpp
//  EONsimulator
//
//  Created by Raul Almeida on 27/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#include "Event.h"
#include "Connection.h"

////////////////////////////////////////////////////////////////////////
void Event::setTime(TIME t){
    time = t;
}

// ------------------------------------------------------- //
TIME Event::getTime(){
    return time;
}

// ------------------------------------------------------- //
void Event::setType(EventType et){
    type = et;
}

// ------------------------------------------------------- //
EventType Event::getType(){
    return type;
}

// ------------------------------------------------------- //
void Event::setNextEvent(Event* evt){
    nextEvent = evt;
}
// ------------------------------------------------------- //
Event* Event::getNextEvent(){
    return nextEvent;
}

// ------------------------------------------------------- //
void Event::setConnection(Connection* con){
    connection = con;
}

// ------------------------------------------------------- //
Connection* Event::getConnection(){
    return connection;
}

///////////////////////////////////////////////////////////////////////////////////////
void Event::setRequestEvent(Event* evt, TIME t){
    evt->time = t;
    evt->type = Req;
    evt->setNextEvent(NULL);
    evt->setConnection(NULL);
}

// -------------------------------------------------------------- //
void Event::setReleaseEvent(Event* evt, Connection *newConnection){
    evt->setTime(newConnection->getAssignment()->getDeactivationTime());
    evt->setType(Desc);
    evt->setNextEvent(NULL);
    evt->setConnection(newConnection);    
}

///////////////////////////////////////////////////////////////////////////////////////
void Event::print(){
    cout<<"Event:  Time="<<time<<" Type="<<type<<" ";
    if(connection != NULL)
        connection->print();
}

