#ifndef PSO_INDIVIDUAL_H
#define PSO_INDIVIDUAL_H


#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <random>
#include "Def.h"
#include "General.h"
#include "InpOut.h"
using namespace std;

class PSO_Individual{
public:
     PSO_Individual(unsigned int,unsigned int,unsigned int,double,double,unsigned int,unsigned int);
     PSO_Individual(unsigned int,unsigned int,unsigned int);
     virtual ~PSO_Individual();
     float readIndividual();
     static void Initialize();
     void setCurrentFitness(double);
     double getCurrentFitness();
     void setCurrentPosition(unsigned int,unsigned int,unsigned int,float);
     float getCurrentPosition(unsigned int,unsigned int,unsigned int);
     void setLocalBestFitness(double);
     double getLocalBestFitness();
     void setLocalBestPosition(unsigned int,unsigned int,unsigned int,float);
     float getLocalBestPosition(unsigned int,unsigned int,unsigned int);
     void setVelocity(unsigned int,unsigned int,unsigned int,float);
     float getVelocity(unsigned int,unsigned int,unsigned int);
     void initializeAttributes(unsigned int,unsigned int,unsigned int);
     void initializeNeighbors(unsigned int,unsigned int);
     void setBestNeighborIndex(unsigned int);
     void printVelocity(unsigned int,unsigned int,unsigned int);
     unsigned int getBestNeighborIndex();
     vector< unsigned int > getNeighborVec();
     void addContSim();
     int getContSim();

private:
    double CurrentFitness;
    double LocalBestFitness;
    unsigned int bestNeighborIndex;
    vector< unsigned int > neighborsIndex;
    vector< vector < vector<float> > > LocalBestPosition;
    vector< vector < vector<float> > > CurrentPosition;
    vector< vector < vector<float> > > Velocity;
    int cont_sim;
};
#endif // PSO_INDIVIDUAL_H
