//
//  Modulation.h
//  EONsimulator
//
//  Created by Raul Almeida on 27/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#ifndef __EONsimulator__Modulation__
#define __EONsimulator__Modulation__

#include <stdio.h>
#include <iostream>
#include <cmath>
using namespace std;
#include "Def.h"
#include "Assignment.h"
#include "Fiber.h"
#include "Topology.h"

class Modulation{
public:
    void setRollOff();
    double getRollOff();
    //
    static double bandwidthQAM(int M, double Rbps, double pol); //Input: QAM type:2(M=1),4(M=2),8(M=3),...; bps=Rb (traffic rate in bps); p=polarization; Return:Bandwidth in Hz
    static double getOSNRQAM(int M, double ber, double bitRate, int polarization);
    static double getSNRbQAM(int M, double ber);
    static double getsnrbQAM(int M, double ber);
    static double getOSNRLimit(int M, double ber,double bitRate);
    static bool   getXT_Index(Assignment* assignment,int,int,bool,int);
    static bool   getXT(Assignment* assignment,int,int,bool,int);
    static bool   getXT(Assignment* assignment,int,int,Route*,int,int);
    static bool   setInterConnectionsXT(Assignment* assignment,int,int);
    static float  getXTLimit(int M, double bitRate,double outage,double ber);
    static void   setOutagePb(double);
    static double getOutagePb();
private:
    static double rollOff;
    static double outagePb;
};

#endif /* defined(__EONsimulator__Modulation__) */
