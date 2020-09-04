#include "Def.h"
#include <limits>


vector<double> Def::LaNet(0);
vector<long double> Def::numReq_Acc_2slots(7,0);
vector<long double> Def::numReq_Acc_4slots(7,0);
vector<long double> Def::numReq_Acc_8slots(7,0);
double Def::numReqMax = 0.0;
double Def::numReq = 0.0;
double Def::numReq_Bloq = 0.0;
double Def::numReq_Bloq_2 = 0.0;
double Def::numReq_Bloq_3 = 0.0;
double Def::numReq_Bloq_4 = 0.0;
double Def::numReq_Bloq_5 = 0.0;
double Def::numReq_Bloq_8 = 0.0;
double Def::numReq_Acc = 0.0;
double Def::numSlots_Bloq = 0.0;
double Def::numSlots_Req = 0.0;
double Def::numHopsPerRoute = 0.0;
double Def::netOccupancy = 0.0;
double Def::MAX_DOUBLE = std::numeric_limits<double>::max();
int Def::MAX_INT = std::numeric_limits<int>::max();
double Def::slotBW = 12500000000;
double Def::Bref = 12500000000;
bool Def::log = false;


int Def::RoutingType = RoutingInvalid;
int Def::SpectrumAssigmentType = SpectrumAssignmentInvalid;

///////////////////////////////////////////////////////////////
void Def::initialise(){
    Def::numReq = 0.0; Def::numReq_Bloq = 0.0; Def::numReq_Acc = 0.0; Def::numSlots_Req = 0.0; Def::numSlots_Bloq = 0.0;
    Def::numHopsPerRoute = 0.0; Def::netOccupancy = 0.0;

}

////////////////////////////////////////////////////////////////
void Def::setNumReqMax(double x){
	assert(x > 0);
	numReqMax = x;
}

// ------------------------------------------------- //
double Def::getNumReqMax(){
	return numReqMax;
}


//////////////////////////////////////////////////////////////////////////////////////////////
void Def::setRoutingType(enum RoutingType r){
    Def::RoutingType = r;
}

// ------------------------------------------------- //
int Def::getRoutingType(){
    return Def::RoutingType;
}

// ------------------------------------------------- //
void Def::setSpectrumAssignmentType(enum SpectrumAssignmentType sa){
    Def::SpectrumAssigmentType = sa;
}

// ------------------------------------------------- //
int Def::getSpectrumAssignmentType(){
    return Def::SpectrumAssigmentType;
}


