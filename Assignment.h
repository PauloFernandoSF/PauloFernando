//
//  Assignment.h
//  EONsimulator
//
//  Created by Raul Almeida on 27/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#ifndef __EONsimulator__Assignment__
#define __EONsimulator__Assignment__

#include <stdio.h>
#include <deque>
#include "Route.h"
class Topology;
//#include "Topology.h"
#include "Def.h"

class Assignment{
public:
    Assignment();
    Assignment(int oN, int dN, Route* = NULL, int = -1, int = -1, int = -1, double = 0.0);
    ~Assignment();
    int getOrN();
    int getDeN();
    void setCoreId(int,int);
    int getHopCoreId(int);
    int getCoreIdSize();
    void setCoreId(int);
    int getCoreId();
    void setFirstSlot(int);
    int getFirstSlot();
    void setLastSlot(int);
    int getLastSlot();
    void setNumSlots(int);
    int getNumSlots();
    void setOSNRth(double);
    double getOSNRth();
    void setXTVec(int,int,int,float);
    float getXTVec(int ,int,int);
    void addXTVec(int,int);
    void setXTReal(float);
    float getXTReal();
    void setXTLimit(float);
    float getXTLimit();
    void setNumSlotsVec(int,int);
    int getNumSlotsVec(int);
    void setXTMed(int,int,int,float);
    float getXTMed(int,int,int);
    void addXTMed(int,int);
    void setBandwidth(double);
    double getBandwidth();
    void setBitRate(double bps);
    double getBitRate();
    void setDeactivationTime(TIME t);
    double getDeactivationTime();

    void setRoute(Route*);
    Route* getRoute();
    void setUseOfRegenerator(bool);
    bool getUseOfRegenerator();
    //
    void pushTrialRoute(Route*);
    void pushTrialRoutes(vector<Route*>&);
    Route* getTrialRoute(int pos);
    Route* popTrialRoute();
    Route* popTrialRoute(int);
    bool isThereTrialRoute();
    void clearTrialRoutes();
    //
    bool isLightPathValid();
    //
    void print();
    void   setPower(double);
    double getPower();
    void   setTempCore(int,int,int,int);
    void   addTempCore(int,int,int);
    void   delTempCore(int,int,int);
    int    getTempCore(int,int,int);
    int    getTempCoreSize(int,int);
    void   resetTempCore(int);
    void   setTempSlot(int,int,int,int);
    int    getTempSlotSize(int,int);
    void   addTempSlot(int,int,int);
    void   delTempSlot(int,int,int);
    int    getTempSlot(int,int,int);
    void   resetTempSlot(int);
    void   resetXTVec(int);
    int    getModulation(int);
    void   setModulation(int,int);
    int    getModulationMatrix(int,int);
    void   setModulationMatrix(int,int,int);
    void   initializeModMatrix(int);
    void   initializePosAloc(int,int);
    int    getNumSlotsMatrix(int,int);
    void   setNumSlotsMatrix(int,int,int);
    void   initializeNumSlotsMatrix(int);
    void   setPowerVec(int,double);
    double getPowerVec(int);


private:
    int orN;
    int deN;
    vector<int> coreVec;
    vector< vector<vector<int>>> tempSlot;
    vector< vector<vector<int>>> tempCore;
    vector<int> modulation;
    vector<int> numSlotsVec;
    vector<double> powerVec;
    vector< vector<vector<float>>> xtMed;
    int coreId;
    Route *route;
    deque<Route*> trialRoutes;
    int Si;
    int Sf;
    int numSlots;
    double osnrTh;
    float xtLimit;
    float xtReal;
    vector< vector<float>> xtVec;
    vector< vector<vector<float>>> xtVecGreedy;
    vector< vector<int>>  modulationMatrix;
    vector< vector<int>>  numSlotsMatrix;
    double bandwidth;
    double bitRate;
    double signalPower;
    bool useOfRegenerator;
    TIME deactivationTime;
};

#endif /* defined(__EONsimulator__Assignment__) */
