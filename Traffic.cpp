//
//  Traffic.cpp
//  EONsimulator
//
//  Created by Raul C. Almeida Jr. on 26/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#include "Traffic.h"

vector<double> Traffic::Vtraffic(0);
const double Traffic::BER = 0.0038;
const int Traffic::polarization = 2;

//////////////////////////////////////////////////////////////////////////////////////
void Traffic::addTraffic(double bps){
    Vtraffic.push_back(bps);
}

//////////////////////////////////////////////////////////////////////////////////////
/*
void Traffic::setBER(double ber){
    BER = ber;
}
 */

// -------------------------------------------------------------------------- //
double Traffic::getBER(){
    return BER;

}

// -------------------------------------------------------------------------- //
/*
void Traffic::setPolarization(int p){
    polarization = p;
}
 */

// -------------------------------------------------------------------------- //
int Traffic::getPolarization(){
    return polarization;
}

//////////////////////////////////////////////////////////////////////////////////////
double Traffic::bitRateTrafficRequest(){
    int aux = General::uniform(0,(int) Traffic::Vtraffic.size());
    return Traffic::Vtraffic.at(aux);
}

// -------------------------------------------------------------------------- //
void Traffic::sourceDestinationTrafficRequest(int& orN, int& deN){
    //srand(NULL);//Sempre mudar semente
    orN = rand()% Topology::getNumNodes();
    deN = rand()% (Topology::getNumNodes()-1);
    if(deN >= orN)
        deN += 1;
    if( (orN < 0) || (orN>= Topology::getNumNodes()) || (deN<0) || (deN>=Topology::getNumNodes()) ||(deN == orN)){
        cout<<"Error in SDPair";
        cin.get();
    }
}

//////////////////////////////////////////////////////////////////////////////////////
