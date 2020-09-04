//
//  Assignment.cpp
//  EONsimulator
//
//  Created by Raul Almeida on 27/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#include "Assignment.h"
#include "Topology.h"

////////////////////////////////////////////////////////////////////////
Assignment::Assignment()
:orN(-1),deN(-1),route(NULL), Si(-1), Sf(-1), numSlots(-1), osnrTh(0.0), bandwidth(0.0), deactivationTime(Def::MAX_DOUBLE){
    useOfRegenerator = false;
}

// ---------------------------------------------------------- //
Assignment::Assignment(int oN, int dN, Route *r, int si, int sf, int ns, double osnrTh)
:orN(oN), deN(dN), route(r), Si(si), Sf(sf), numSlots(ns), osnrTh(osnrTh), bandwidth(0.0), deactivationTime(Def::MAX_DOUBLE){
    useOfRegenerator = false;

    //tempSlot.resize(Routing::KYEN,vector<int>(0));
    //tempCore.resize(Routing::KYEN,vector<int>(0));
    //xtVec.resize(Routing::KYEN,vector<float>(0));
    //xtMed.resize(Routing::KYEN,vector<float>(0));
    for(int tam = 0;tam < Routing::KYEN;tam++){
            modulation.push_back(Def::MAX_INT);
            numSlotsVec.push_back(NULL);
            powerVec.push_back(0);
    }

}

// ---------------------------------------------------------- //
Assignment::~Assignment(){
        //Atributos para a configuração de CARTAXO
        //while(!xtVec.empty() || !tempCore.empty() || !tempSlot.empty() || !modulation.empty() || !xtMed.empty() || !numSlotsVec.empty() || !powerVec.empty()
        //|| !numSlotsMatrix.empty() || !modulationMatrix.empty()){
            //xtVec.erase(xtVec.begin());
            //xtMed.erase(xtMed.begin());
            //tempCore.erase(tempCore.begin());
            //tempSlot.erase(tempSlot.begin());
            //modulation.erase(modulation.begin());
            //numSlotsVec.erase(numSlotsVec.begin());
            //modulationMatrix.erase(modulationMatrix.begin());
            //numSlotsMatrix.erase(numSlotsMatrix.begin());
            //powerVec.erase(powerVec.begin());
        //}

        while(!modulation.empty() ||  !numSlotsVec.empty() || !powerVec.empty()
        ){
            //xtVec.erase(xtVec.begin());
            //xtMed.erase(xtMed.begin());
            //tempCore.erase(tempCore.begin());
            //tempSlot.erase(tempSlot.begin());
            modulation.erase(modulation.begin());
            numSlotsVec.erase(numSlotsVec.begin());
            //modulationMatrix.erase(modulationMatrix.begin());
            //numSlotsMatrix.erase(numSlotsMatrix.begin());
            powerVec.erase(powerVec.begin());
        }
        if(route != NULL)
            delete route;
        for(unsigned int i = 0; i < trialRoutes.size(); i++)
            delete trialRoutes.at(i);
        trialRoutes.clear();

}

////////////////////////////////////////////////////////////////////////
int Assignment::getOrN(){
    return orN;
}

void Assignment::initializePosAloc(int route,int mod){
    tempSlot.resize(route,vector<vector<int> >(mod,vector<int>(0)));
    tempCore.resize(route,vector<vector<int> >(mod,vector<int>(0)));
    xtVecGreedy.resize(route,vector<vector<float> >(mod,vector<float>(0)));
    xtMed.resize(route,vector<vector<float> >(mod,vector<float>(0)));
}
// ---------------------------------------------------------- //
void Assignment::setTempCore(int r,int m,int aloc,int core){
     tempCore.at(r).at(m).at(aloc) = core;
}
// ---------------------------------------------------------- //
int Assignment::getTempCore(int r,int m,int aloc){
    return tempCore.at(r).at(m).at(aloc);
}
// ---------------------------------------------------------- //
void Assignment::addTempCore(int r,int m,int core){
    return tempCore.at(r).at(m).push_back(core);
}
// ---------------------------------------------------------- //
void Assignment::delTempCore(int route,int mod,int pos){
    tempCore.at(route).at(mod).erase(tempCore.at(route).at(mod).begin() + pos);
}
// ---------------------------------------------------------- //
int Assignment::getTempCoreSize(int r,int m){
    return tempCore.at(r).at(m).size();
}
// ---------------------------------------------------------- //
void Assignment::setPowerVec(int pos,double power){
     powerVec.at(pos) = power;
}
// ---------------------------------------------------------- //
double Assignment::getPowerVec(int pos){
    return powerVec.at(pos);
}
// ---------------------------------------------------------- //
void Assignment::setTempSlot(int r,int m,int aloc,int slot){
    tempSlot.at(r).at(m).at(aloc) = slot;
}
// ---------------------------------------------------------- //
int Assignment::getTempSlot(int r,int m,int aloc){
    return tempSlot.at(r).at(m).at(aloc);
}
// ---------------------------------------------------------- //
void Assignment::addTempSlot(int r,int m,int slot){
    tempSlot.at(r).at(m).push_back(slot);
}
// ---------------------------------------------------------- //
void Assignment::delTempSlot(int r,int m,int pos){
    tempSlot.at(r).at(m).erase(tempSlot.at(r).at(m).begin() + pos);
}
// ---------------------------------------------------------- //
int Assignment::getTempSlotSize(int r,int m){
    return tempSlot.at(r).at(m).size();
}
// ---------------------------------------------------------- //
void Assignment::setNumSlotsVec(int pos,int num){
    numSlotsVec.at(pos) = num;
}
// ---------------------------------------------------------- //
int Assignment::getNumSlotsVec(int pos){
    return numSlotsVec.at(pos);
}
// ---------------------------------------------------------- //
int Assignment::getDeN(){
    return deN;
}
// ---------------------------------------------------------- //
void Assignment::setFirstSlot(int si){
    Si = si;
}
// ---------------------------------------------------------- //
int Assignment::getFirstSlot(){
    return Si;
}
//Functions to Core Switching
void Assignment::setCoreId(int core,int hop){
assert(core >= 0 && core < Topology::getNumCores());
    coreVec.push_back(NULL);
    coreVec.at(hop) = core;
}
// ---------------------------------------------------------- //
int Assignment::getHopCoreId(int hop){
    return coreVec.at(hop);
}

int Assignment::getCoreIdSize(){
    return coreVec.size();
}
// ---------------------------------------------------------- //
void Assignment::setCoreId(int core){
    coreId = core;
}
// ---------------------------------------------------------- //
int Assignment::getCoreId(){
    return coreId;
}
// ---------------------------------------------------------- //
void Assignment::setLastSlot(int sf){
    Sf = sf;
}
// ---------------------------------------------------------- //
int Assignment::getLastSlot(){
    return Sf;
}
// ---------------------------------------------------------- //
void Assignment::setNumSlots(int ns){
    numSlots = ns;
}
// ---------------------------------------------------------- //
int Assignment::getNumSlots(){
    return numSlots;
}
// ---------------------------------------------------------- //
void Assignment::setOSNRth(double osnr){
    osnrTh = osnr;
}
// ---------------------------------------------------------- //
double Assignment::getOSNRth(){
    return osnrTh;
}
// ---------------------------------------------------------- //
void Assignment::setBandwidth(double bw){
    bandwidth = bw;
}
// ---------------------------------------------------------- //
double Assignment::getBandwidth(){
    return bandwidth;
}
// ---------------------------------------------------------- //
void Assignment::setXTVec(int r,int m,int aloc,float xt){
    xtVecGreedy.at(r).at(m).at(aloc) = xt;
}
// ---------------------------------------------------------- //
float Assignment::getXTVec(int r,int m,int aloc){
    return xtVecGreedy.at(r).at(m).at(aloc);
}
// ---------------------------------------------------------- //
void Assignment::addXTVec(int route,int mod){
     xtVecGreedy.at(route).at(mod).push_back(Def::MAX_DOUBLE);
}
// ---------------------------------------------------------- //
void Assignment::setXTReal(float xt){
    xtReal = xt;
}
// ---------------------------------------------------------- //
float Assignment::getXTReal(){
    return xtReal;
}
// ---------------------------------------------------------- //
void Assignment::setXTLimit(float xt){
    xtLimit = xt;
}
// ---------------------------------------------------------- //
float Assignment::getXTLimit(){
    return xtLimit;
}
// ---------------------------------------------------------- //
void Assignment::setXTMed(int route,int mod,int aloc,float xt){
    xtMed.at(route).at(mod).at(aloc) = xt;
}
// ---------------------------------------------------------- //
float Assignment::getXTMed(int route,int mod,int aloc){
    return xtMed.at(route).at(mod).at(aloc);
}
// ---------------------------------------------------------- //
void Assignment::addXTMed(int route,int mod){
     xtMed.at(route).at(mod).push_back(Def::MAX_DOUBLE);
}
// ---------------------------------------------------------- //

void Assignment::setBitRate(double bps){
    bitRate = bps;
}
// ---------------------------------------------------------- //
double Assignment::getBitRate(){
    return bitRate;
}
// ---------------------------------------------------------- //
void Assignment::setDeactivationTime(TIME t){
    assert(t > Schedule::getSimTime());
    deactivationTime = t;
}
// ---------------------------------------------------------- //
double Assignment::getDeactivationTime(){
    return deactivationTime;
}
// ---------------------------------------------------------- //
void Assignment::setRoute(Route* r){
    route = r;
}
// ---------------------------------------------------------- //
Route* Assignment::getRoute(){
    return route;
}
////////////////////////////////////////////////////////////////////////
void Assignment::setUseOfRegenerator(bool r){
    assert(useOfRegenerator == !r);
    useOfRegenerator = r;
}
// ---------------------------------------------------------- //
bool Assignment::getUseOfRegenerator(){
    return useOfRegenerator;
}
////////////////////////////////////////////////////////////////////////
void Assignment::pushTrialRoute(Route* r){
    trialRoutes.push_back(r);
}
// ---------------------------------------------------------- //
void Assignment::pushTrialRoutes(vector<Route*>& vRoute){
    for(unsigned int i = 0; i < vRoute.size(); i++)
        pushTrialRoute(vRoute.at(i));
    //vRoute.clear();
}
// ---------------------------------------------------------- //
Route* Assignment::popTrialRoute(){
    Route *r = NULL;
    if(!trialRoutes.empty()){
        r = trialRoutes.front();
        trialRoutes.pop_front();
    }
    return r;
}
// ---------------------------------------------------------- //
Route* Assignment::popTrialRoute(int pos){
    Route *r = NULL;
    if(!trialRoutes.empty()){
        r = trialRoutes.at(pos);
        trialRoutes.erase(trialRoutes.begin() + pos);
    }
    return r;
}
// ---------------------------------------------------------- //
bool Assignment::isThereTrialRoute(){
    return !trialRoutes.empty();
}
// ---------------------------------------------------------- //
void Assignment::clearTrialRoutes(){
    Route *r;
    while(!trialRoutes.empty()){
        //r = trialRoutes.front();
        //delete r;
        //trialRoutes.pop_front();
        trialRoutes.erase(trialRoutes.begin());
    }

}
////////////////////////////////////////////////////////////////////////
bool Assignment::isLightPathValid(){
    if( (Topology::isValidRoute(route)) && (Si <= Sf) && (Topology::isValidSlot(Si)) && (Topology::isValidSlot(Sf)) )
        return true;
    return false;
}
////////////////////////////////////////////////////////////////////////
void Assignment::print(){
    cout<<"orN="<<orN<<" deN="<<deN<<" bps="<<bitRate<<" deacTime="<<deactivationTime<<"  Si="<<getFirstSlot()<<"  Sf="<<getLastSlot();
    if(route != NULL){
        cout<<"  Route=";
        route->print();
    }
    if(getUseOfRegenerator())
        cout<<"+R";

}

Route* Assignment::getTrialRoute(int pos){
    return trialRoutes.at(pos);
}

double Assignment::getPower(){
    return signalPower;
}

void Assignment::setPower(double power){
     signalPower = power;
}

void Assignment::setModulation(int pos,int mod){
    modulation.at(pos) = mod;
}

int Assignment::getModulation(int pos){
    return modulation.at(pos);
}

void Assignment::initializeModMatrix(int num){
        modulationMatrix.resize(Routing::KYEN,vector<int>(0));
        for(int r = 0;r < Routing::KYEN;r++){
           for(int i = 0;i < num;i++){
                modulationMatrix.at(r).push_back(0);
           }
        }
}

void Assignment::setModulationMatrix(int r,int m,int modulation){
    modulationMatrix.at(r).at(m) = modulation;
}

int Assignment::getModulationMatrix(int r,int m){
    return modulationMatrix.at(r).at(m);
}

void Assignment::initializeNumSlotsMatrix(int num){
        numSlotsMatrix.resize(Routing::KYEN,vector<int>(0));
        for(int r = 0;r < Routing::KYEN;r++){
           for(int i = 0;i < num;i++){
                numSlotsMatrix.at(r).push_back(0);
           }
        }
}

void Assignment::setNumSlotsMatrix(int r,int m,int slots){
    numSlotsMatrix.at(r).at(m) = slots;
}

int Assignment::getNumSlotsMatrix(int r,int m){
    return numSlotsMatrix.at(r).at(m);
}

void Assignment::resetTempCore(int route){
    tempCore.at(route).erase(tempCore.at(route).begin(),tempCore.at(route).end());
}

void Assignment::resetTempSlot(int route){
    tempSlot.at(route).erase(tempSlot.at(route).begin(),tempSlot.at(route).end());
}

void Assignment::resetXTVec(int route){
    xtVec.at(route).erase(xtVec.at(route).begin(),xtVec.at(route).end());
}
