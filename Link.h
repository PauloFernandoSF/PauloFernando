//
//  Link.h
//  EONsimulator
//
//  Created by Raul C. Almeida Jr. on 25/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#ifndef EONsimulator_Link_h
#define EONsimulator_Link_h

#include <assert.h>

class Topology;
//#include "Topology.h"
class Signal;

#define slotFree 0
#define slotUsed 1

enum LinkCostType{minHops, minLength};

class Link{
public:
    Link(int orNode, int deNode, double length, int numSec);
    Link();
    ~Link();
    void initialise();
    //
    void setOrNode(int);
    int getOrNode();
    void setDeNode(int);
    int getDeNode();
    void setLength(double);
    double getLength();
    void setNumSec(int);
    int getNumSec();
    void setAsBroken();
    void setAsWorking();
    bool isWorking();
    //
    void calcSignal(Signal&,double);
    //
    double getCost();
    static void setCostType(int);
    //
    void occupySlot(int);
    void releaseSlot(int);
    bool isSlotOccupied(int slot);
    bool isSlotFree(int slot);
private:
    int orNode;
    int deNode;
    bool isBroken;
    double length;
    int numSec;
    bool* Status;
    void setSlotStatus(int slot, bool status);
    static LinkCostType linkCostType;
};

#endif
