//
//  Signal.cpp
//  EONsimulator
//
//  Created by Raul Almeida on 28/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#include "Signal.h"

const double Signal::v = 194*pow(10,12);//193.4*pow(10,12);
const double Signal::h = 6.62606957*pow(10,-34); //Planck constant
const double Signal::Fn = 3.16; //Amplifier Noise Figure;
const double Signal::fn = General::dBtoLinear(Fn);
const double Signal::Bo = 12.5*pow(10,9); //Reference Bandwidth;
const double Signal::Alpha = 0.2*pow(10,-3); //Amplifier Gain distribution dB/m
const double Signal::pIn = 0.0003;
const double Signal::Pin = General::linearWTodBm(pIn);
const double Signal::pRef = 0.0003;
const double Signal::Pref = General::linearWTodBm(pRef);
const double Signal::OSNRin = 1000;//30
const double Signal::osnrIn = General::dBtoLinear(OSNRin);

//////////////////////////////////////////////////////////////////////////////////////////
void Signal::initialise(){
    signalPower = Signal::pIn;
    //asePower = Signal::pIn/Signal::osnrIn;
    asePower = 0;
    nlPower = 0.0;
}

//////////////////////////////////////////////////////////////////////////////////////////
void Signal::setSignalPower(double powerWatts){
    signalPower = powerWatts;
}

// --------------------------------------------------------- //
double Signal::getSignalPower(){
    return signalPower;

}

// --------------------------------------------------------- //
void Signal::setASEPower(double powerWatts){
    asePower = powerWatts;
}

// --------------------------------------------------------- //
double Signal::getAsePower(){
    return asePower;
}

// --------------------------------------------------------- //
void Signal::setNonLinearPower(double powerWatts){
    nlPower = powerWatts;
}

// --------------------------------------------------------- //
double Signal::getNonLinearPower(){
    return nlPower;
}

//////////////////////////////////////////////////////////////////////////////////////////
double Signal::getosnr(){
    return signalPower/((asePower+nlPower)*Bo);
}

// --------------------------------------------------------- //
double Signal::getOSNR(){
    return General::linearTodB(getosnr());
}

//////////////////////////////////////////////////////////////////////////////////////////
double Signal::pASE(double fn, double gain){
    return 2*nASE(fn, gain);
}

// --------------------------------------------------------- //
double Signal::nASE(double fn, double gain){ //Densidade Espectral de Ruído por polarização
    assert(gain >= 1.0); //
    return (h*v*(gain-1.0)*fn)/2;
}

