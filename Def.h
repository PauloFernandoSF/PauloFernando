#ifndef Def_H
#define Def_H

#include <assert.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#include "General.h"

//Definitions of Types:
typedef double TIME;

enum RoutingType {RoutingInvalid, RoutingDJK, RoutingYEN}; // Informa o algortimo de Roteamento;
enum SpectrumAssignmentType {SpectrumAssignmentInvalid, SpectrumAssignmentFirstFitSlot, SpectrumAssignmentFirstFitOrdenado, SpectrumAssignmentFirstFitCore, SpectrumAssignmentRandomCore,
SpectrumAssignmentMSCL,SpectrumAssignmentFirstFitAG, SpectrumAssignmentFirstFitAreas, SpectrumAssignmentNucleoExaustivo, SpectrumAssignmentPropostaFuji,
SpectrumAssignmentPSO,SpectrumAssignmentCPFFXT,SpectrumAssignmentXTGreedy,SpectrumAssignmentFirstFitXT}; // Informa o algortimo de Alocação de espectro;

////////////////////////////////////////////////////////////////
class Def{
	public:
        static void initialise();
    //
		static void setNumReqMax(double);
		static double getNumReqMax();
		static void setLaUniform(double);
		static void setLaRandom(double);
		static void setLaManual(double);
		static void setLaCheck(double);
		static double getLaNet(int);
    //
        static void setRoutingType(RoutingType r);
        static int getRoutingType();
        static void setSpectrumAssignmentType(SpectrumAssignmentType sa);
        static int getSpectrumAssignmentType();
private:
		static vector<double> LaNet;
		static double numReqMax;
	public:
		static double numReq;
		static vector<long double> numReq_Acc_2slots;
		static vector<long double> numReq_Acc_4slots;
		static vector<long double> numReq_Acc_8slots;
		static double numReq_Bloq;
        static double numReq_Acc;
		static double numReq_Bloq_2;
        static double numReq_Bloq_3;
        static double numReq_Bloq_4;
        static double numReq_Bloq_5;
        static double numReq_Bloq_8;
		static double numSlots_Bloq;
		static double numSlots_Req;
		static double numHopsPerRoute;
		static double netOccupancy;
		static double MAX_DOUBLE;
		static int MAX_INT;
        static double slotBW;
        static double Bref;
        static bool log;
    private:
        static int SpectrumAssigmentType;
        static int RoutingType;
};

#endif
