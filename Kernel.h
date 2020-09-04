#ifndef KERNEL_H_INCLUDED
#define KERNEL_H_INCLUDED

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <limits>
#include <climits>
#include <vector>
#include <time.h>
using namespace std;

#include "Schedule.h"
#include "Def.h"
#include "InpOut.h"
#include "General.h"
#include "Route.h"
#include "Routing.h"
#include "Connection.h"
#include "Heuristics.h"
#include "Topology.h"
#include "Traffic.h"
#include "Modulation.h"
#include "Node.h"
#include "Core.h"
#include "Fiber.h"
#include "AlgoritmoGenetico.h"
#include "PSO.h"

//ifstream topologyFile ("C:/Users/paulo_000/Downloads/Mestrado/Redes_Opticas/Simula/simula/Topology.txt");

class Kernel{
public:
    static long double Simulation(int,int);
    static void Load();
    static void Simulate(int,int);
    static void InitialiseAll();
    static void FinaliseAll();
    static void ConnectionRequest(Event*,int,int);
    static bool RSA(Assignment* assignment,int,int);
    static bool RSA_XT_CP(Assignment* assignment,int,int);
    static bool RSA_XT_FF(Assignment* assignment,int,int);
    static bool RSA_XT_Random(Assignment* assignment,int,int);
    static bool RSA_XT_Greedy(Assignment* assignment,int,int);
    static void ConnectionSetup(Assignment* assign);
    static void ConnectionRelease(Event*);
    static void AccountForBlocking(int NslotsReq, int NslotsUsed);
    static long double mu, laNet, laE, muC, LaNetMin, LaNetMax, LaPasso;
    static void ImprimeFreqCores();
    static void coreRequestCountAdd(Assignment*,bool);
    static void addNumSim();
    static int  getNumSim();
private:
    static int numSim;
};

#endif // KERNEL_H_INCLUDED
