//
//  Link.cpp
//  EONsimulator
//
//  Created by Raul C. Almeida Jr. on 25/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#include "Link.h"
#include "Topology.h"
#include "Signal.h"

LinkCostType Link::linkCostType = minLength;//minHops;

//////////////////////////////////////////////////////////
Link::Link(int orNode, int deNode, double length, int numSec)
:orNode(orNode), deNode(deNode), isBroken(false), length(length), numSec(numSec), Status(NULL){
    assert(Topology::getNumSlots()>0);
    Status = new bool[Topology::getNumSlots()];
}

// --------------------------------------------------- //
Link::Link()
:orNode(-1), deNode(-1), isBroken(false), length(0.0), numSec(0), Status(NULL){
    assert(Topology::getNumSlots()>0);
    Status = new bool[Topology::getNumSlots()];
}

// --------------------------------------------------- //
Link::~Link(){
    delete []Status;
}

//////////////////////////////////////////////////////////
void Link::initialise(){
    for(int nSl = 0; nSl < Topology::getNumSlots(); nSl++)
        Status[nSl] = slotFree;
}

//////////////////////////////////////////////////////////
void Link::setOrNode(int oNode){
    orNode = oNode;
}

// --------------------------------------------------- //
int Link::getOrNode(){
    return orNode;
}

// --------------------------------------------------- //
void Link::setDeNode(int dNode){
    deNode = dNode;
}

// --------------------------------------------------- //
int Link::getDeNode(){
    return deNode;
}

// --------------------------------------------------- //
void Link::setLength(double x){
    assert(x>0.0);
    length = x;
}

// --------------------------------------------------- //
double Link::getLength(){
    return length;

}

// --------------------------------------------------- //
void Link::setNumSec(int x){
    assert(x>0);
    numSec = x;
}

// --------------------------------------------------- //
int Link::getNumSec(){
    return numSec;

}

//////////////////////////////////////////////////////////
void Link::setAsBroken(){
    isBroken = true;
}

// --------------------------------------------------- //
void Link::setAsWorking(){
    isBroken = false;
}

// --------------------------------------------------- //
bool Link::isWorking(){
    return !isBroken;
}

//////////////////////////////////////////////////////////
double Link::getCost(){
    if(isBroken)
        return Def::MAX_DOUBLE;
    if(linkCostType == minHops) //Assuming MinHops
        return 1.0;
    else if(linkCostType == minLength) //Assuming MinDist
        return length;
    assert(false);
}

//////////////////////////////////////////////////////////
void Link::calcSignal(Signal& signal,double Rbps){
    //Link structure: fiber - Amp - fiber - Amp ... fiber - Amp
    //Assuming a link with equal gain distribution
    int p = length;
    int a = numSec;
    double signalPower = signal.getSignalPower();
    double asePower = signal.getAsePower();
    double nonLinearPower = signal.getNonLinearPower();
    //
    double Lsec = (double) length/numSec;
    //double gLsec = 1.0/(General::dBtoLinear(Lsec*Signal::Alpha)); //Fiber Gain
    double gLsec = 1/General::dBtoLinear(Lsec*Signal::Alpha); //Fiber Section Loss + optical node loss
    //gLsec = General::linearTodB(gLsec);
    double gAmp; //= 1.0/(gLsec); //Amplifier Gain
    double gLsecLnode = 1/General::dBtoLinear(16);
    //ASE para do primeiro amplificador
    //asePower += Signal::pASE(Signal::fn, gAmp);
    //Consideration of the link Sections:
    for(int sec = 0; sec < numSec; sec++){
        if(sec == 0 || sec == numSec - 1){
            gAmp = 1/gLsecLnode + 1/gLsec;
        }
        else{
            gAmp = 1/gLsec;
        }
        signalPower *= 1/gAmp;
        //Fibre:
        //signalPower *= gLsec;
        //asePower *= gLsec;
        nonLinearPower *= gLsec;
        nonLinearPower += 0.0; //Alterar esse 0.0 para a potência fornecida pela seção
        //Amplifier:
        signalPower *= gAmp;
        double p = Signal::fn;
        asePower += Signal::pASE(Signal::fn, gAmp);
        nonLinearPower *= gAmp;
    }
    //asePower = General::linearTodB(asePower);
    signal.setASEPower(asePower);
    signal.setNonLinearPower(nonLinearPower);
}

//////////////////////////////////////////////////////////
void Link::occupySlot(int slot){
    assert(isSlotFree(slot));
    Status[slot] = slotUsed;
}

// --------------------------------------------------- //
void Link::releaseSlot(int slot){
    if(!isSlotOccupied(slot))
        cout<<"Trial to release empty slot"<<endl;
    assert(isSlotOccupied(slot));
    Status[slot] = slotFree;
}

// --------------------------------------------------- //
void Link::setSlotStatus(int slot, bool status){
    Status[slot] = status;
}

// --------------------------------------------------- //
bool Link::isSlotOccupied(int slot){
    if(Status[slot] == slotUsed)
        return true;
    return false;
}

// --------------------------------------------------- //
bool Link::isSlotFree(int slot){
    return !isSlotOccupied(slot);
}

//////////////////////////////////////////////////////////
