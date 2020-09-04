#ifndef PSO_H
#define PSO_H

#include "Kernel.h"
#include "PSO_Individual.h"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>


using namespace std;

class PSO{
    public:
        static void Initialize();
        static void Operation();
        static void BestIndividual();
        static void updateParticle(PSO_Individual*,unsigned int);
        static void setGlobalBestPosition(unsigned int);
        static void  updateLocalBestPosition(PSO_Individual*);
        static double getGlobalBestPosition(unsigned int,unsigned int,unsigned int);
        static PSO_Individual* getPopulationElement(unsigned int);
        static void  printBestIndividual();
        static void  printIndividual(int);
        static void  printBestFitness(unsigned int);
        static void  printBestIndividualVelocity();
        static unsigned int getD1();
        static unsigned int getD2();
        static unsigned int getD3();
        static void initializeGlobalBest(unsigned int,unsigned int,unsigned int);
        static void updateLocalBestIndex(unsigned int);
        static void printLogPSO();
        static void Ressim(int,int);
    private:
        static unsigned int NeighborsNum;
        static unsigned int Generations;
        static unsigned int Population_Size;
        static unsigned int D1;
        static unsigned int D2;
        static unsigned int D3;
        static int Ressimulation;
        //Velocity parameters
        static double C1;
        static double C2;
        static double Xsi;
        /////////////////////
        //Solution space
        static double Minimum;
        static double Maximum;
        ////////////////
        //Global Best
        static double GlobalBestFitness;
        static vector< vector < vector<float> > > GlobalBestPosition;
        static unsigned int BestIndividualIndex;
        /////////////
        static vector< double > FitVec;
        static vector< PSO_Individual* > Population;
};

#endif // PSO_H
