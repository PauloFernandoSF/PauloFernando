#ifndef Heuristics_H
#define Heuristics_H

#include <assert.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
using namespace std;

#include "Topology.h"
#include "Assignment.h"
#include "Routing.h"

/////////////////////////////////////////////////////////////
class Heuristics{
	public:
		static void FFO_convencional(vector<int>**);
		static void FFO_extremos(vector<int>**);
		static void FFO_invertido(vector<int>**);
		static void FFO_metrica(vector<int>**);
		static void TestFFlists(vector<int>**);
		static void PrintFFlists(vector<int>**);
    //Atribuição de rota:
     /*
        static void RoutingMinHop(Assignment*);
        static void RoutingMinLength(Assignment*);
     */
        static void Routing(Assignment*);
    //Atribuição de slots:
        static void SpectrumAssignment(Assignment*,int,int,int,int,int);
        static void FirstFitOrdenado(Assignment*,int,bool);
        static void FirstFitCore(Assignment*);
        static void FirstFitCore(Assignment*,bool);
        static void FirstFitAreas(Assignment*);
        static void FirstFitAG(Assignment*,int,int);
        static void PSO(Assignment*,unsigned int);
        static void FirstFitAG_CS(Assignment*,int,int);
        static void FirstFitCoreXT(Assignment* assignment,int,int,int);
        static void CorePrioritization(Assignment* assignment,int);
        static void PropostaFuji(Assignment*);
        static void PropostaFuji_CS(Assignment*);
        static void RandomCore(Assignment*);
        static void FirstFitSlot(Assignment*);
        static void FirstFitXT(Assignment*);
        static void MostUsed(Assignment*);
        static void MSCL(Assignment*);
        static void NucleoExaustivo(Assignment*,int);
    //Connection Expansion and Compression:
        static void ExpandConnection(Connection *con);
        static void ExpandRandomly(Connection *con);
        static void CompressConnection(Connection *con);
        static void CompressRandomly(Connection *con);
    //
		static long double calculateCostLink(bool* Disp, int L);
		static long double calculateCostRouting(int hops, bool* Disp, int L);
		static double calculateCoreCost(unsigned int,unsigned int,Route*,unsigned int);
    //
        static void setSpectrumAssignmentType(int);
	private:
		static int calcNumFormAloc(int L, bool* Disp,int tam);
		static int calcNumAloc(int L, bool* Disp);
};

#endif
