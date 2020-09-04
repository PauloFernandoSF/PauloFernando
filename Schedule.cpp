//
//  Schedule.cpp
//  EONsimulator
//
//  Created by Raul Almeida on 27/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#include "Schedule.h"

Event* Schedule::firstEvent = NULL;
TIME Schedule::simTime = 0;

////////////////////////////////////////////////////////////////////////////////
void Schedule::initialise(){
    firstEvent = NULL;
    simTime = 0.0;
}

////////////////////////////////////////////////////////////////////////////////
void Schedule::setSimTime(TIME sTime){
    assert(sTime >= simTime); //Simulation time cannot be rewarded;
    simTime = sTime;
}

// -------------------------------------------------------------- //
TIME Schedule::getSimTime(){
    return simTime;
}

////////////////////////////////////////////////////////////////////////////////
Event* Schedule::getCurrentEvent(){
    Event* curEvt = firstEvent;
    if(firstEvent != NULL){
        Schedule::setSimTime(curEvt->getTime());
        firstEvent = firstEvent->getNextEvent();
    }
    return curEvt;
}

////////////////////////////////////////////////////////////////////////////////
void Schedule::scheduleEvent(Event* evt){
    Event *evtAux = firstEvent, *evtAnt = NULL;
    while(evtAux != NULL){
        if(evt->getTime() < evtAux->getTime())
            break;
        else{
            evtAnt = evtAux;
            evtAux = evtAux->getNextEvent();
        }
    }
    evt->setNextEvent(evtAux);
    if(evtAnt == NULL)
        firstEvent = evt;
    else
        evtAnt->setNextEvent(evt);


}

/*
void Schedule::defineNextEventOfCon(){
    Connection *con = evt->Connection;
    //Recalcula Te (tempo de expans„o) e Tc (tempo de compress„o) e os compara com o tempo para desativaÁ„o
    //SÛ pode haver expans„o se o n˙mero de slots for menor do que Topology::getNumSlots()
    TIME Te, Tc, evtTime;
    EventType evtType = UNKNOWN;
    if( (ExpComp == true) && ((con->getLastSlot() - con->getFirstSlot() + 1) < Topology::getNumSlots()) ) //Se o n˙mero de slots utilizados for menor do que Topology::getNumSlots()
        Te = Def::simTime + General::exponential(laE);
    else
        Te = Def::MAX_DOUBLE;
    //SÛ pode haver compress„o se o n˙mero de slots for maior ou igual a 2, pois 1 È o n˙moer mÌnimo do slots que podem estar ativos para uma conex„o
    if( (ExpComp == true) && (con->getLastSlot() - con->getFirstSlot() + 1 > 1) )
        Tc = Def::simTime + General::exponential(muC);
    else
        Tc = Def::MAX_DOUBLE;
    //Checa o prÛximo evento dentre expans„o, compress„o e desativaÁ„o
    if(Te < Tc){
        if(Te < con->getTimeDesc()){
            evtTime = Te;
            evtType = Exp;
        }
        else{
            evtTime = con->getTimeDesc();
            evtType = Desc;
        }
    }
    else{
        if(Tc < con->getTimeDesc()){
            evtTime = Tc;
            evtType = Comp;
        }
        else{
            evtTime = con->getTimeDesc();
            evtType = Desc;
        }
    }
    //
    evt->time = evtTime;
    evt->type = evtType;
}
 */

////////////////////////////////////////////////////////////////////////////////
bool Schedule::isEmpty(){
    return (firstEvent == 0);
}
////////////////////////////////////////////////////////////////////////////////

void Schedule::printAllEvents(){
    /*
    Event *auxEvent = firstEvent;
    while(auxEvent != NULL){
        auxEvent->print();
        cout<<endl;
        auxEvent = auxEvent->getNextEvent();
    }
     */

}
