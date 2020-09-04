//
//  InpOut.h
//  EONsimulator
//
//  Created by Raul C. Almeida Jr. on 25/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//
#ifndef EONsimulator_InpOut_h
#define EONsimulator_InpOut_h
#include <fstream>
#include <iostream>
using namespace std;


class InpOut{
public:
    static ifstream topology;
    static ifstream traffic;
    static ifstream modulation;
    static ifstream link;
    static ifstream PSO_Individual;

    static ofstream Result;
    static ofstream GA_Results;
    static ofstream GA_Better_Worst;
    static ofstream GA_Initial_Pop;
    static ofstream Core_Freq;
    static ofstream GA_Log;
    static ofstream PSO_Results;
    static ofstream PSO_Log;

    InpOut();
};

#endif
