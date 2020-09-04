//
//  Signal.h
//  EONsimulator
//
//  Created by Raul Almeida on 28/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#ifndef __EONsimulator__Signal__
#define __EONsimulator__Signal__

#include <stdio.h>
#include <stdio.h>
#include <math.h>

#include "General.h"


////////////////////////////////////////////////////////////////////////////////////////////////
class Signal{
public:
    void initialise();
    //
    void setSignalPower(double);
    double getSignalPower();
    void setASEPower(double);
    double getAsePower();
    void setNonLinearPower(double);
    double getNonLinearPower();
    //
    double getosnr();
    double getOSNR();
private:
    double signalPower;
    double asePower;
    double nlPower;
public:
    static double pASE(double fn, double gain); //fn-noise factor; gain-gain of the EDFA. pASE retorna a potência ASE
    static double nASE(double fn, double gain); //nASE retorna a densidade espectral de potência
    //
    static const double v; //central frequency
    static const double h; //Planck constant
    static const double Fn; //Amplifier Noise Figure;
    static const double fn; //Amplifier Noise Factor;
    static const double Bo; //Reference Bandwidth;
    static const double Alpha; //Fiber attenuation coefficient
    static const double alpha; //Fiber attenuation coefficient
    static const double pIn; //Signal Input Power in Watts
    static const double Pin; //Signal Input Power in dBm;
    static const double pRef; //Reference Power at the fiber input in W;
    static const double Pref; // Reference Power at the fiber input in dBm;
    static const double OSNRin; //OSNR of the input signal in dB
    static const double osnrIn; //OSNR of the input signal in linear
    static const double Lwss; //WSS loss in dB
    static const double lWSS; //WSS loss in linear
};

#endif /* defined(__EONsimulator__Signal__) */
