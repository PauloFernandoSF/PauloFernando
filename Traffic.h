//
//  Traffic.h
//  EONsimulator
//
//  Created by Raul C. Almeida Jr. on 26/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#ifndef EONsimulator_Traffic_h
#define EONsimulator_Traffic_h

#include <iostream>
#include <vector>
using namespace std;

#include "General.h"
#include "Topology.h"

//////////////////////////////////////////////////////////////////////
class Traffic{
public:
    static void addTraffic(double tbps);
    //
    //static void setBER(double);
    static double getBER();
    //static void setPolarization(int);
    static int getPolarization();
    //
    static double bitRateTrafficRequest();
    static void sourceDestinationTrafficRequest(int& orN, int& deN);
    //
private:
    static vector<double> Vtraffic;
    static const double BER;
    static const int polarization;
};

#endif
