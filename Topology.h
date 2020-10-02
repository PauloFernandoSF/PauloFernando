//
//  Topology.h
//  EONsimulator
//
//  Created by Raul C. Almeida Jr. on 25/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#ifndef EONsimulator_Topology_h
#define EONsimulator_Topology_h

#include "Link.h"
#include "Node.h"
#include "Route.h"
#include <vector>
#include "Assignment.h"
#include "Connection.h"
#include "General.h"
#include "Fiber.h"
#include "InpOut.h"
#include "Signal.h"
#include "AlgoritmoGenetico.h"
#include "PSO.h"

//////////////////////////////////////////////////////////////////////
class Topology{
public:
    static void initialise();
    //Structure
    static void setNumCores(int);
    static int getNumCores();
    static void setNumFibers(int);
    static int getNumFibers();
    static void setNumNodes(int);
    static int getNumNodes();
    static bool nodeValid(int node);
    static void setNumLinks(int);
    static int getNumLinks();
    static void setNumSlots(int);
    static int getNumSlots();
    static void printRouteState(Route*,int,int,int);
    static void setTotalSlots();
    static int getTotalSlots();
    //
    static bool isValidLink(Link*); // Testa se o link Ž v‡lido
    static bool isValidSlot(int);// Testa se o slot Ž v‡lido
    static bool isValidRoute(Route*);// Testa se a rota Ž v‡lida
    static bool isValidFiber(Fiber* fiber); //Testa se  fibra é válida
    //
    static void insertLink(Link*);
    static Link* getLink(int orN, int deN); //Input: origin and destination nodes of the link
    static void insertFiber(Fiber*);
    static Fiber* getFiber(int,int);
    static void insertNode(Node*);
    static Node* getNode(NodeId);
    //
    static bool isNodeWorking(int node);
    static void setNodeAsWorking(int node);
    //Set all nodes of the route as working
    static void setAllNodesAsWorking(Route*);
    static void setNodeAsBroken(int node);
    static void setAllLinksWorking();
    //Routing
    static void setRoute(int orN, int deN, Route* route);
    static void addRoute(int orN, int deN, Route* route);
    static void setRoutes(int orN, int deN, vector<Route*>* routes);
    static void addRoutes(int orN, int deN, vector<Route*>* routes);
    static void clearRoutes(int orN, int deN);
    //
    static vector<Route*>* getRoutes(int orN, int deN);
    //
    static bool areThereOccupiedSlots();
    static int  sumOccupation(int s);
    static bool checkSlotDisp(const Route *route, int s);
    static bool checkSlotsDisp(const Route *route, int si, int sf);
    static bool checkSlotsDispCore(const Route *route, int si, int sf,Assignment*);
    static bool checkSlotsCore(const Route *route, int si, int sf,int core_id);
    static bool checkSlotsCore_CS(const Route *route, int si, int sf,Assignment*,int core_id);
    static bool checkSlotsCore_CS(const Route *route, int si, int sf,Assignment*, vector<unsigned int>*);
    static int  checkSlotsCore_1(const Route *route, int si, int sf,Assignment*,int core_id);
    static bool checkSlotsCoreAG(Route *route, int si, int sf,int coreID);
    static bool checkPossibleSet(Route* route,int si, int sf,Assignment*assignment);
    static bool checkCoreSet(Route* route,int si,int sf,int core,int hop);
    static bool checkSlotNumberDisp(const Route *route, int numSlots); //Check whether there are numSlots contiguous slots available in the route
    static bool checkOSNR(const Route *route, double OSNRth,Assignment*); //Check whether the route OSNR is enough compared to the required OSNR
    static bool checkOSNRVec(const Route *route, double OSNRth,Assignment*,int);
    static bool checkXT(int ,int,int,Assignment*, float XT);
    static void updateNU(); //Update Average Network Utilization #ocupied slots/#total network slots/#number of times that this function is called(average)
    static void setNU(float);
    static float getNU();
    static void updateAdjConnXT(Connection*);
    //
    static void Connect(Connection*);
    static void occupySlot(Connection*, int);
    static void releaseConnection(Connection*);
    static void releaseSlot(Connection*, int);
    static void SetFragNumber();
    static int  getFragNumber(unsigned int);
    //
    static void print();
    static void printAllRoutes();
    static void printRoute(Route*);
    static void printAllNodes();
    //
    static vector< vector<Route*> > AllRoutes;
    static vector< vector<Route*> > AllInterRoutes;
    static void setInterRoutes();
    //
    static bool getCS(); //Return if CoreSwitch is available
    static void setCS(bool); //Set CoreSwitch availability
public:
    static bool* linkState; //Substitui topology_S por enlace;
    static Link* *linkTopology; //Substitui Topology
    static vector<Node*> vNodes; //Vector with all the nodes in the network;
    static vector<Fiber*> vFibers; //Vector with all the nodes in the network;
    static int xtBloq;
    static int resourceBloq;
    static float xtMed;
    static vector<int> numReqPerCore;
    static vector<int> numReqPerRoute;

private:
    static float NU;
    static int totalSlots;
    static int numNodes; //Numero de nos da rede
    static int numFibers;//Numeros de fibras da rede
    static int numCores; //Numero de núcleos da fibra
    static int numLinks; //Numero de enlaces da rede
    static int numSlots; //Numero de slots por Enlace
    static bool coreSwitch; //Variable to identify CoreSwitch funcionality
    static int oneSlotFrag,twoSlotFrag; //Variables to store number of one and two slots fragments
    static vector<int>* *RouteInt;
    static vector<bool> NodeWorking;
};

#endif
