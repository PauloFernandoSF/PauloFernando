//
//  Topology.cpp
//  EONsimulator
//
//  Created by Raul C. Almeida Jr. on 25/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#include "Topology.h"
#include "General.h"
#include <vector>
//#include "InpOut.h"

Link* *Topology::linkTopology = NULL;//Substitui Topology
vector<Node*> Topology::vNodes(0); //Vector with all the nodes in the network;
vector<Fiber*> Topology::vFibers(0); //Vector with all the fibers in the network;
vector<vector<Route*> >Topology::AllRoutes(0);
vector< vector<Route*> > Topology::AllInterRoutes(0);
vector<int>* *Topology::RouteInt = NULL;
int Topology::numNodes = 0; //Numero de nos da rede
int Topology::numFibers = 0; //Numero de fibras da rede
int Topology::numCores = 0; //Numero de núcleos por fibra da rede
int Topology::numLinks = 0; //Numero de enlaces da rede
int Topology::numSlots = 0; //Numero de slots por Enlace
int Topology::oneSlotFrag = 0;
int Topology::twoSlotFrag = 0;
int Topology::xtBloq = 0;
int Topology::resourceBloq = 0;
int Topology::totalSlots = 0;
float Topology::NU = 0;
bool Topology::coreSwitch = false;
vector<bool> Topology::NodeWorking(0);
vector<int> Topology::numReqPerCore(0);
vector<int> Topology::numReqPerRoute(0);
float Topology::xtMed = 0;


////////////////////////////////////////////////////////////
void Topology::initialise(){
xtBloq = 0,resourceBloq = 0,NU = 0, xtMed = 0;

    //Inicializa vetor com o número de requisições aceitas em cada núcleos
    //Para o AG, quando utilizado o AG, só precisa entrar uma vez neste,sem este "if" toda simulação fa um push_back em numReqPerRoute
        if(Topology::numReqPerRoute.size() == 0)
            for(int r = 0;r < Routing::KYEN;r++)
                Topology::numReqPerRoute.push_back(0);
        else
            for(int r = 0;r < Routing::KYEN;r++)
                Topology::numReqPerRoute.at(r) = 0;

        //for(int c = 0;c < numCores;c++)
        //    numReqPerCore.at(c) = 0;
        for(int orN = 0; orN < numNodes; orN++){
            vNodes.at(orN)->initialise();
            for(int deN = 0; deN < numNodes; deN++)
                if(linkTopology[orN*Topology::numNodes + deN] != NULL)
                    linkTopology[orN*Topology::numNodes + deN]->initialise();
        }
}

////////////////////////////////////////////////////////////
void Topology::setNumNodes(int nNodes){
    assert((numNodes == 0) && (nNodes>0));
    linkTopology = new Link*[nNodes*nNodes];
    for(int i = 0; i < nNodes; i++)
        vNodes.push_back(NULL);
    numNodes=nNodes;
    NodeWorking.clear(); //Retirar isso
    for(int i = 0; i < numNodes; i++){
        NodeWorking.push_back(true);
        for(int j = 0; j < numNodes; j++)
            linkTopology[i*numNodes + j] = NULL;
    }
    AllRoutes.clear();
    AllRoutes.resize(nNodes*nNodes);
    AllInterRoutes.resize(nNodes*nNodes);
}// --track-origins=yes

// ------------------------------------------------------ //
int Topology::getNumNodes(){
    return numNodes;
}

// ------------------------------------------------------ //
bool Topology::nodeValid(int node){
    return ((0 <= node) && (node < numNodes));
}

////////////////////////////////////////////////////////////
void Topology::setNumFibers(int nFibers){
    assert(nFibers>0);
    for(int i=0;i<nFibers*nFibers;i++)
        vFibers.push_back(NULL);
    numFibers=nFibers;

}
// ------------------------------------------------------ //
int Topology::getNumFibers(){
    return numFibers;
}
// ------------------------------------------------------ //
void Topology::setNumCores(int nCores ){
    assert(nCores>0);
    numCores=nCores;
    //for(int c = 0;c < numCores;c++)
    //    numReqPerCore.push_back(0);

}

// ------------------------------------------------------ //
int Topology::getNumCores(){
    return numCores;
}

int Topology::getFragNumber(unsigned int numSlots){
    if(numSlots == 1)
        return oneSlotFrag;
    if(numSlots == 2)
        return twoSlotFrag;
}
// ------------------------------------------------------ //
void Topology::setNumLinks(int nLinks){
    assert(nLinks>=0);
    numLinks=nLinks;
}

// ------------------------------------------------------ //
int Topology::getNumLinks(){
    return numLinks;
}

// ------------------------------------------------------ //
void Topology::setNumSlots(int nSlots){
    assert(nSlots>0);
    numSlots=nSlots;
}

// ------------------------------------------------------ //
int Topology::getNumSlots(){
    return numSlots;
}


////////////////////////////////////////////////////////////////////////////
bool Topology::isNodeWorking(int node){
    return NodeWorking.at(node);
}

// ------------------------------------------------------ //
void Topology::setNodeAsWorking(int node){
    assert(NodeWorking.at(node) == false);
    NodeWorking.at(node) = true;
}

// ------------------------------------------------------ //
void Topology::setAllNodesAsWorking(Route* route){
    unsigned int nodes = route->getNumNodes();
    vector<int>* path = route->getPath();
    for(unsigned int node = 0;node < nodes;node++)
        NodeWorking.at(path->at(node)) = true;
}

// ------------------------------------------------------ //
void Topology::setNodeAsBroken(int node){
    assert(NodeWorking.at(node) == true);
    NodeWorking.at(node) = false;
}
// ------------------------------------------------------ //
void Topology::setCS(bool cs){
    coreSwitch = cs;
}
// ------------------------------------------------------ //
bool Topology::getCS(){
    return(coreSwitch);
}

////////////////////////////////////////////////////////////////////////////
void Topology::setRoute(int orN, int deN, Route* route){
     Topology::clearRoutes(orN, deN);
     Topology::addRoute(orN, deN, route);
}

// ------------------------------------------------------ //
void Topology::setAllLinksWorking(){
    Link* link;
    for(int orN = 0;orN < numNodes;orN++)
        for(int deN = 0;deN < numNodes;deN++){
                if(orN == deN)
                    continue;
                link = getLink(orN,deN);
                if(link != NULL)
                    link->setAsWorking();
         }
}
// ------------------------------------------------------ //
void Topology::addRoute(int orN, int deN, Route* route){
    AllRoutes.at(orN*numNodes+deN).push_back(route);
}

// ****************************************************** //
void Topology::setRoutes(int orN, int deN, vector<Route*>* routes){ //Delete all routes between orN and deN and set with routes
     Topology::clearRoutes(orN, deN);
     Topology::addRoutes(orN, deN, routes);
}

// ------------------------------------------------------ //
void Topology::addRoutes(int orN, int deN, vector<Route*>* routes){
     General::addVectorPtr<Route>(Topology::getRoutes(orN, deN), routes);
}

// ------------------------------------------------------ //
void Topology::clearRoutes(int orN, int deN){
    General::clearVectorPtr<Route>(Topology::getRoutes(orN, deN));
}

// ------------------------------------------------------ //
vector<Route*>* Topology::getRoutes(int orN, int deN){
    return &AllRoutes.at(orN*numNodes+deN);
}

////////////////////////////////////////////////////////////
bool Topology::isValidLink(Link* link){
    return (Topology::nodeValid(link->getOrNode())&&(Topology::nodeValid(link->getDeNode())));
}

// ------------------------------------------------------ //
bool Topology::isValidFiber(Fiber* fiber){
    return (Topology::nodeValid(fiber->getOrNode())&&(Topology::nodeValid(fiber->getDeNode())));
}

// ------------------------------------------------------ //

bool Topology::isValidSlot(int s){ // Testa se o slot é válido
    if(s >= 0 && s < Topology::numSlots)
        return true;
    return false;
}

// ------------------------------------------------------ //
bool Topology::isValidRoute(Route* r){ // Testa se a rota é válida
    if(r != NULL){
        int L_or, L_de; Link *link;
        for(unsigned int c = 0; c < r->getNumHops(); c++){
            L_or = r->getNode(c);
            L_de = r->getNode(c+1);
            link = Topology::getLink(L_or, L_de);
            if(link == NULL)
                return false;
        }
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////
void Topology::insertLink(Link* link){
    assert(linkTopology[link->getOrNode()*Topology::getNumNodes() + link->getDeNode()] == NULL);
    linkTopology[link->getOrNode()*Topology::numNodes + link->getDeNode()]=link;
}

// ------------------------------------------------------ //
Link* Topology::getLink(int orN, int deN){
    if(!( (Topology::nodeValid(orN)) && (Topology::nodeValid(deN)) ))
        cout<<"Error in Topology::getLink()"<<orN<<" "<<deN<<endl;
    return linkTopology[orN*numNodes + deN];
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //
void Topology::insertNode(Node* node){
    assert(node->getNodeId() < vNodes.size()); //Nodes have to be inserted in order of their ID
    vNodes.at(node->getNodeId()) = node;
}

// ------------------------------------------------------ //
Node* Topology::getNode(NodeId nodeId){
    return vNodes.at(nodeId);
}
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //

void Topology::insertFiber(Fiber* fiber){
    assert(fiber->getFiberId() < vFibers.size()); //Fibers have to be inserted in order of their ID
    vFibers.at(fiber->getOrNode()*numNodes + fiber->getDeNode()) = fiber;
}

// ------------------------------------------------------ //
Fiber* Topology::getFiber(int orN, int deN){
    return vFibers.at(orN*numNodes + deN);
}


////////////////////////////////////////////////////////////
void Topology::Connect(Connection* connection){
    //Insert connection into the network
    int L_or, L_de; Link *link;Fiber* fiber; const Route* route = connection->getAssignment()->getRoute();
    int core;
    for(int c = 0; c < route->getNumHops(); c++){
            if(nodeValid(route->getNode(c))){ // Acrescentei
                L_or = route->getNode(c);
                L_de = route->getNode(c+1);
                fiber = Topology::getFiber(L_or, L_de);
                assert(Topology::isValidFiber(fiber)); // Acrescentei
                //Core Siwtch
                if(coreSwitch){
                    core = connection->getAssignment()->getHopCoreId(c);
                    //for(int s = connection->getFirstSlot(); s <= connection->getLastSlot(); s++){
                    fiber->getFiberCore(core)->occupySlots(connection->getFirstSlot(),connection->getAssignment());
                    //}
                }
                //No Core Switch
                else{
                    core = connection->getAssignment()->getCoreId();
                    fiber->getFiberCore(core)->occupySlots(connection->getFirstSlot(),connection->getAssignment());
                }
          }
    }

    Def::numHopsPerRoute += route->getNumHops();
    Def::netOccupancy += (connection->getLastSlot()-connection->getFirstSlot()+1)*route->getNumHops();
}

void Topology::SetFragNumber(){
    for(unsigned int a = 0;a < Topology::getNumFibers();a++){
        if(vFibers.at(a) == NULL)
            continue;
        else{
             for(unsigned int c = 0;c < Topology::getNumCores();c++)
                for(unsigned int s = 0;s < Topology::getNumSlots();s++){
                   //Logic for one slot fragment
                   if(!vFibers.at(a)->getFiberCore(c)->getSlotOccupation(s)){
                        if(s == 0 ){
                            if(vFibers.at(a)->getFiberCore(c)->getSlotOccupation(s + 1)){
                                oneSlotFrag++;
                                continue;
                            }
                        }
                        if(s == Topology::getNumSlots() - 1){
                            if(vFibers.at(a)->getFiberCore(c)->getSlotOccupation(s - 1)){
                                oneSlotFrag++;
                                continue;
                            }
                        }
                        if(vFibers.at(a)->getFiberCore(c)->getSlotOccupation(s - 1)
                        && vFibers.at(a)->getFiberCore(c)->getSlotOccupation(s + 1)){
                            oneSlotFrag++;
                        }
                   }
                   ///////////////////////////////////////////////////////////
                    //Logic for two slot fragment
                   if(!vFibers.at(a)->getFiberCore(c)->getSlotOccupation(s) && !vFibers.at(a)->getFiberCore(c)->getSlotOccupation(s + 1)){
                        if(s == 0 ){
                            if(vFibers.at(a)->getFiberCore(c)->getSlotOccupation(s + 2)){
                                twoSlotFrag++;
                                continue;
                            }
                        }
                        if(s + 1 == Topology::getNumSlots() - 1){
                            if(vFibers.at(a)->getFiberCore(c)->getSlotOccupation(s - 1))
                                twoSlotFrag++;
                            break;
                        }
                        if(vFibers.at(a)->getFiberCore(c)->getSlotOccupation(s - 1)
                        && vFibers.at(a)->getFiberCore(c)->getSlotOccupation(s + 2)){
                            twoSlotFrag++;
                        }
                   }
                   ///////////////////////////////////////////////////////////

              }
        }
    }
}
// ------------------------------------------------------ //
void Topology::occupySlot(Connection* connection, int s){ //occupy slot s in all links of connection
    //Insert connection into the network
    int L_or, L_de; Link *link; const Route* route = connection->getAssignment()->getRoute();
    for(unsigned c = 0; c < route->getNumHops(); c++){
        L_or = route->getNode(c);
        L_de = route->getNode(c+1);
        link = Topology::getLink(L_or, L_de);
        link->occupySlot(s);
    }
}
// ------------------------------------------------------ //
void Topology::releaseConnection(Connection* connection){
    int L_or, L_de;Link *link;
    Route *route = connection->getAssignment()->getRoute();
    int core;Fiber* fiber;
    //release all slots used for the Connection
    Topology::printRouteState(route,connection->getAssignment()->getNumSlots(),connection->getAssignment()->getCoreId(),connection->getFirstSlot());

    updateAdjConnXT(connection);
    for(int c = 0; c < route->getNumHops(); c++){
       L_or = route->getNode(c);
       L_de = route->getNode(c+1);
       fiber = Topology::getFiber(L_or, L_de);
       assert(Topology::isValidFiber(fiber));// Acrescentei
       //Release for Core Switch
       if(coreSwitch){
            core = connection->getAssignment()->getHopCoreId(c);
            //for(int s = connection->getFirstSlot(); s <= connection->getLastSlot(); s++){
            fiber->getFiberCore(core)->releaseSlots(connection->getFirstSlot(),connection->getAssignment());
            //}
       }
       //Release without Core Switch
       else{
            //UPDATE XT OF INTERFERING CONNECTIONS
            core = connection->getAssignment()->getCoreId();
            fiber->getFiberCore(core)->releaseSlots(connection->getFirstSlot(),connection->getAssignment());
       }
    }
    //Topology::printRouteState(route,connection->getAssignment()->getNumSlots(),connection->getAssignment()->getCoreId(),connection->getFirstSlot());

}

void Topology::updateAdjConnXT(Connection* connection){
Assignment* assignment = connection->getAssignment();
Route* route = assignment->getRoute();
int numHops = route->getNumHops(),ori,de,adjCoreIndex,iniSlot = assignment->getFirstSlot(),numSlots = assignment->getNumSlots(),nucleo = assignment->getCoreId();
int L;
Fiber* fiber;
Core* adjCore;
Link* link;
double R,A,h;
float k,beta;
//coeficiente de acoplamento
k =  float(0.012); //0.00584;
//constante de propagação
beta = pow(10,7);
//raio de curvatura
R = double(0.01);
//core pitch
A = double(4.5*pow(10,-5));
//
h = double((2*pow(k,2)*R)/(beta*A));
//Topology::printRouteState(route,numSlots,nucleo,iniSlot);
    for(int c = 0;c < numHops;c++){
        ori = route->getNode(c);de = route->getNode(c + 1);
        fiber = Topology::getFiber(ori,de);
        link = Topology::getLink(ori,de);
        L = link->getLength();
        for(int adj = 0;adj < fiber->getFiberCore(nucleo)->getNumAdjCores();adj++){
            //Topology::printRouteState(route,numSlots,nucleo,iniSlot);
            adjCoreIndex = fiber->getFiberCore(nucleo)->getAdjCoreIndex(adj);
            adjCore = fiber->getFiberCore(adjCoreIndex);
            //índice de interferencia-Verificar quantas conexões se sobrepoem
            for(int slot = iniSlot;slot < (iniSlot + numSlots - 1);slot++){
                if(fiber->getCoreOccupation(slot,adjCore) && fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot)->getLastSlot() != slot){
                    //Slot interferente,reduzir XT
                    float xt = adjCore->getSlotXT(slot);
                    xt = fiber->getFiberCore(nucleo)->getSlotXT(slot);
                    if(xt > 0){
                        xt = fiber->getFiberCore(adjCoreIndex)->getSlotXT(slot) - (1 - exp(-2*h*L))/(1 + exp(-2*h*L));
                        fiber->getFiberCore(adjCoreIndex)->setSlotXT(slot,xt);
                    }
                }
            }
        }
    }
}
// ------------------------------------------------------ //
void Topology::releaseSlot(Connection* connection, int s){ //Release slot s in all links of connection
    int L_or, L_de; Link *link; const Route *route = connection->getAssignment()->getRoute();
    //release all slots used for the Connection
    for(unsigned c = 0; c < route->getNumHops(); c++){
        L_or = route->getNode(c);
        L_de = route->getNode(c+1);
        link = Topology::getLink(L_or, L_de);
        link->releaseSlot(s);
    }
}
////////////////////////////////////////////////////////////
bool Topology::areThereOccupiedSlots(){
    Link *link;
//    if (Topology::validLink(link))
    for(int oN = 0; oN < numNodes; oN++)
        for(int dN = 0; dN < numNodes; dN++){
            link = linkTopology[oN*numNodes + dN];
            if(link != NULL) //There is a link between nodes oN and dN;
                for(int s = 0; s < getNumSlots(); s++) //Check whether all slots are free;
                    if(link->isSlotOccupied(s)) //If a single slot is occupied, return true;
                        return true;
        }
    return false;
}
// ------------------------------------------------------ //
int Topology::sumOccupation(int s){  //Sum the occupation of slot s in the network
    int soma=0; Link *link;
    for(int oN = 0; oN < Topology::getNumNodes(); oN++)
        for(int dN = 0; dN < Topology::getNumNodes(); dN++){
            link = Topology::getLink(oN, dN);
            if( (link != NULL) && (link->isSlotOccupied(s)) ) //Se houver enlace entre origem e destino e o slot 's' estiver ocupado neste enlace
                soma++; // acrescentei o validLink na linha de cima
        }
    return soma;
}
// ------------------------------------------------------ //
bool Topology::checkSlotDisp(const Route *route, int s){ //Checa se o slot está disponível na rota
    int L_or, L_de; Link *link;
    for(unsigned c = 0; c < route->getNumHops(); c++){
        L_or = route->getNode(c);
        L_de = route->getNode(c+1);
        link = Topology::getLink(L_or,L_de);
        if(link->isSlotOccupied(s)) // o enlace c->c+1 esta ocupado;
            return false;
    }
    return true;
}
// ------------------------------------------------------ //
bool Topology::checkSlotsDisp(const Route *route, int si, int sf){ //Checa se um intervalo de slots está disponível na rota
    int L_or, L_de; Link *link;
    for(unsigned c = 0; c < route->getNumHops(); c++){
        L_or = route->getNode(c);
        L_de = route->getNode(c+1);
        link = Topology::getLink(L_or,L_de);
        for(int s = si; s <= sf; s++)
            if(link->isSlotOccupied(s)) // o enlace c->c+1 esta ocupado no slot s;
                return false;
    }
    return true;
}
// ------------------------------------------------------ //
bool Topology::checkSlotsDispCore(const Route *route, int si, int sf,Assignment*assignment){ //Checa se um intervalo de slots está disponível na rota
    int L_or, L_de, coreID, x = route->getNumHops();
    bool flag;
    Fiber *fiber;
    L_or = route->getNode(0);L_de = route->getNode(1);
    fiber = Topology::getFiber(L_or,L_de);
        /////////////procura o núcleo disponível
        for(int i=0;i<Topology::getNumCores() ;i++){
            for(int s = si; s <= sf; s++){
                if(fiber->getFiberCore(i)->getSlotOccupation(s)){ // o enlace c->c+1 esta ocupado no slot s?;
                    flag = false;
                    break;
                }
                if(s == sf){                //encontrou o núcleo no primeiro enlace
                    coreID = i;
                    flag = true;
                    break;
                }
            }
            if(flag)
                break;
            if(!flag  && i == Topology::getNumCores() - 1)
                return false;
            if(i == Topology::getNumCores() - 1)
                return false;
        }
        ////////////////////////////////////////////////////
        //procura no mesmo core e slots nos outros hops da rota
        for(unsigned c = 1; c < route->getNumHops(); c++){
            L_or = route->getNode(c);L_de = route->getNode(c+1);
            fiber = Topology::getFiber(L_or,L_de);
               for(int s = si;s <= sf; s++){
                    if(fiber->getFiberCore(coreID)->getSlotOccupation(s))
                       return false;
               }
        }
        //////////////////////////////////////////////////////////
    assignment->setCoreId(coreID);
    return true;
}
// ------------------------------------------------------ //
void Topology::setInterRoutes(){

    Route *routeAux, *routeAux2;
    int nodeRoute[2], nodeRouteInt[2];
    bool flag = true;

    for(unsigned int a = 1; a < AllRoutes.size() - 1; a++){
        if(a%(numNodes + 1) == 0)
            a += 1;
        routeAux = AllRoutes.at(a).at(0);
        for(unsigned int b = 0; b < (routeAux->getPath()->size() - 1); b++){
            nodeRoute[0] = routeAux->getPath()->at(b);
            nodeRoute[1] = routeAux->getPath()->at(b+1);
            for(unsigned int c = 1; c < AllRoutes.size() - 1; c++){
                if(c%(numNodes + 1) == 0)
                    c += 1;
                routeAux2 = AllRoutes.at(c).at(0);
                if(routeAux == routeAux2)
                    continue;
                for(unsigned int d = 0; d < (routeAux2->getPath()->size()-1); d++){
                    nodeRouteInt[0] = routeAux2->getPath()->at(d);
                    nodeRouteInt[1] = routeAux2->getPath()->at(d+1);
                    if(nodeRoute[0]==nodeRouteInt[0] && nodeRoute[1]==nodeRouteInt[1]){
                        for(unsigned int e = 0; e < AllInterRoutes.at(a).size(); e++){
                            if(AllInterRoutes.at(a).at(e) == routeAux2){
                                flag = false;
                                d = routeAux2->getPath()->size()-1;
                                break;
                            }
                        }
                        if(flag){
                            AllInterRoutes.at(a).push_back(routeAux2);
                            break;
                        }
                        flag = true;
                    }
                }
            }
        }
    }
}
// ------------------------------------------------------ //
bool Topology::checkSlotsCore(const Route *route, int si, int sf,int core_id){ //Checa se um intervalo de slots está disponível na rota
int L_or, L_de,hops = route->getNumHops();
Fiber *fiber;
    //Verfica a disponibilidade do conjunto de slots no core nos hops restantes da rota
            for(unsigned c = 0; c < hops; c++){
                L_or = route->getNode(c);L_de = route->getNode(c+1);
                fiber = Topology::getFiber(L_or,L_de);
                   for(int s = si;s <= sf; s++){
                        if(fiber->getFiberCore(core_id)->getSlotOccupation(s)){
                           return false;
                        }
                   }
            }
    //////////////////////////////////////////////////////////////////////////////////////
    return true;
}
// ------------------------------------------------------ //
bool Topology::checkSlotsCoreAG(const Route *route, int si, int sf,int coreID){ //Checa se um intervalo de slots está disponível na rota
    int L_or, L_de, x = route->getNumHops();
    bool flag = false;
    Fiber *fiber;
    //procura no mesmo core e slots nos outros hops da rota
    for(unsigned int c = 0; c < route->getNumHops(); c++){
           L_or = route->getNode(c);L_de = route->getNode(c+1);
           fiber = Topology::getFiber(L_or,L_de);
           for(int s = si;s <= sf; s++){
                 if(fiber->getFiberCore(coreID)->getSlotOccupation(s))
                      return false;
                 if(s == sf){
                      break;
                 }
           }
    }
    //////////////////////////////////////////////////////////
    return true;
}
// ------------------------------------------------------ //
bool Topology::checkSlotNumberDisp(const Route *route, int numSlots){ //Check whether there are enough number of contiguous slots available in the route
    int numContiguousSlots = 0;
    for(int s = 0; s < Topology::numSlots; s++){
        if(checkSlotDisp(route, s))
            numContiguousSlots++;
        else
            numContiguousSlots = 0;
        if(numContiguousSlots == numSlots)
            return true;
    }
    return false;
}
// ------------------------------------------------------ //
int Topology::checkSlotsCore_1(const Route *route, int si, int sf,Assignment*assignment,int core_id){ //Checa se um intervalo de slots está disponível na rota
    int L_or, L_de,x = route->getNumHops(), stop_slot = 500;
    Fiber *fiber;
    //bool flag = false;
    //Verfica a disponibilidade do conjunto de slots no core nos hops restantes da rota
            for(unsigned c = 0; c < route->getNumHops(); c++){
                L_or = route->getNode(c);L_de = route->getNode(c+1);
                fiber = Topology::getFiber(L_or,L_de);
                   for(int s = si;s <= sf; s++){
                        if(fiber->getFiberCore(core_id)->getSlotOccupation(s)){
                           stop_slot = s;
                           return stop_slot;
                        }
                   }
            }
    //////////////////////////////////////////////////////////////////////////////////////
    //Found available set of slots
    return stop_slot;
}

bool Topology::checkSlotsCore_CS(const Route *route, int si, int sf,Assignment*assignment,int core_id){ //Checa se um intervalo de slots está disponível na rota
    int L_or, L_de,hops = route->getNumHops();
    int nucleos[hops];
    Fiber *fiber;
    L_or = route->getNode(0);L_de = route->getNode(1);
    bool flag_1 = false;
    fiber = Topology::getFiber(L_or,L_de);
    //Verfica a disponibilidade do conjunto de slots no core no primeiro hop da rota
            /*for(int s = si; s <= sf; s++){
                if(fiber->getFiberCore(core_id)->getSlotOccupation(s)){ // o enlace c->c+1 esta ocupado no slot s?;
                    return false;
                }
                if(s == sf){                //encontrou o núcleo no primeiro enlace
                    nucleos[0] = core_id;
                    break;
                }
            }*/
    /////////////////////////////////////////////////////////////////////////////////////
    //Verfica a disponibilidade do conjunto de slots no core nos hops restantes da rota
            for(int c = 0; c < hops; c++){
                    L_or = route->getNode(c);L_de = route->getNode(c+1);
                    fiber = Topology::getFiber(L_or,L_de);
                    int core = 0;
                    //laço para percorrer todos nucleos
                    for(int cont = 0;cont < Topology::getNumCores();cont++){
                           //laço para percorrer todo conunto de slots
                           for(int s = si;s <= sf; s++){
                                if(fiber->getFiberCore(core)->getSlotOccupation(s))
                                    break;
                                if(s == sf){
                                    nucleos[c] = core;
                                    flag_1 = true;
                                    break;
                                }
                           }
                           //nucleo com slots disponiveis encontrados - parte para proximo enlace
                           if(flag_1){
                                flag_1 = false;
                                break;
                           }
                           //nao encontrou nenhum nucleo
                           if(cont == Topology::getNumCores()- 1)
                                return false;
                           //chegou no nucleo 6 e nao encontrou os slots disponiveis, mas nao percorreu todos nucleos
                           if(core == Topology::getNumCores()- 1)
                                core = 0;
                           //tenta outro nucleo
                           else
                                core++;
                   }
            }
            //preenche nucleo com os nucleos de cada enlace
            for(int c = 0; c < hops; c++){
                 assignment->setCoreId(nucleos[c],c);
            }

    //////////////////////////////////////////////////////////////////////////////////////

    return true;
}

bool Topology::checkSlotsCore_CS(const Route *route, int si, int sf,Assignment*assignment, vector<unsigned int>* ordem){ //Checa se um intervalo de slots está disponível na rota
    int L_or, L_de,hops = route->getNumHops();
    int nucleos[hops];
    Fiber *fiber;
    bool flag = false,flag_1 = false;

    //Verfica a disponibilidade do conjunto de slots no core nos hops restantes da rota
            for(int c = 0; c < hops; c++){
                    L_or = route->getNode(c);L_de = route->getNode(c+1);
                    fiber = Topology::getFiber(L_or,L_de);
                    int core;
                    //laço para percorrer todos nucleos
                    for(int cont = 0;cont < Topology::getNumCores();cont++){
                           core = ordem->at(cont);
                           //laço para percorrer todo conunto de slots
                           for(int s = si;s <= sf; s++){
                                if(fiber->getFiberCore(core)->getSlotOccupation(s))
                                    break;
                                if(s == sf){
                                    nucleos[c] = core;
                                    flag_1 = true;
                                    break;
                                }
                           }
                           //nucleo com slots disponiveis encontrados - parte para proximo enlace
                           if(flag_1){
                                flag_1 = false;
                                break;
                           }
                            //nao encontrou nenhum nucleo
                           if(cont == Topology::getNumCores()- 1)
                                return false;

                   }
            }
            //preenche nucleo com os nucleos de cada enlace
            for(int c = 0; c < route->getNumHops(); c++){
                 assignment->setCoreId(nucleos[c],c);
            }

    //////////////////////////////////////////////////////////////////////////////////////

    return true;
}

//Check if a set of slots is available for an allocation, with core switch
bool Topology::checkPossibleSet(Route* route,int si, int sf,Assignment*assignment){
int L_or, L_de,hops = route->getNumHops();
Fiber *fiber;
bool flag = false,flag_1 = false;
    //hops
    for(int h = 0; h < hops; h++){
        L_or = route->getNode(h);
        L_de = route->getNode(h + 1);
        fiber = Topology::getFiber(L_or,L_de);
        //verify cores
        for(unsigned int core = 0;core < Topology::getNumCores();core++){
            for(int s = si; s <= sf; s++){
                 if(fiber->getFiberCore(core)->getSlotOccupation(s)){
                    //There isn't possible allocation
                    if(core == Topology::getNumCores() - 1)
                        return false;
                    //Go to another core
                    break;
                 }
                 //Found a possible core in the hop
                 if(s == sf){
                    flag = true;
                    break;
                 }
            }
            //Go to another hop
            if(flag){
                flag = false;
                break;
            }
        }
    }
    return true;
}
//Check if a set of slots is available in a specific core in a fiber
bool Topology::checkCoreSet(Route* route,int si,int sf,int core,int hop){
    int L_or = route->getNode(hop), L_de = route->getNode(hop + 1);
    Fiber *fiber = Topology::getFiber(L_or,L_de);

    for(int s = si; s <= sf; s++){
        if(fiber->getFiberCore(core)->getSlotOccupation(s))
                 return false;
    }
    return true;
}
//Check whether the route OSNR is enough compared to the required OSNR
bool Topology::checkOSNR(const Route *route, double OSNRth,Assignment*assignment){
    int L_or, L_de; Link *link;
    Signal signal;
    signal.initialise();
    for(unsigned c = 0; c < route->getNumHops(); c++){
        L_or = route->getNode(c);
        L_de = route->getNode(c+1);
        link = Topology::getLink(L_or,L_de);
        link->calcSignal(signal,assignment->getBitRate());
    }
    double a = signal.getOSNR();
    if(signal.getOSNR() > OSNRth){
        assignment->setPower(signal.getSignalPower());
        return true;
    }
    return false;
}

bool Topology::checkOSNRVec(const Route *route, double OSNRth,Assignment*assignment,int pos){
    int L_or, L_de; Link *link;
    Signal signal;
    signal.initialise();
    double x = 0,a;
    for(unsigned c = 0; c < route->getNumHops(); c++){
        L_or = route->getNode(c);
        L_de = route->getNode(c+1);
        link = Topology::getLink(L_or,L_de);
        link->calcSignal(signal,assignment->getBitRate());
        x  = signal.getAsePower();
    }
    x  = signal.getAsePower();
    if(assignment->getBitRate() == 42.8*pow(10,9) || assignment->getBitRate() == 112*pow(10,9))
            signal.setSignalPower(signal.getSignalPower());
    else{
        if(assignment->getBitRate() == 224*pow(10,9))
            //Modificar para outra fórmula
            signal.setSignalPower(signal.getSignalPower()*2);
        if(assignment->getBitRate() == 448*pow(10,9))
            //Modificar para outra fórmula
            signal.setSignalPower(signal.getSignalPower()*4);
        if(assignment->getBitRate() == 112*pow(10,10))
            //Modificar para outra fórmula
            signal.setSignalPower(signal.getSignalPower()*10);
    }
    a = signal.getOSNR();
    if(signal.getOSNR() > OSNRth){
        assignment->setPowerVec(pos,signal.getSignalPower());
        return true;
    }
    return false;
}


bool Topology::checkXT(int r,int m,int p,Assignment* assignment, float xt){ //Check whether the route xt is enough compared to the required XT
double possibleAlocXT = assignment->getXTVec(r,m,p);
    //Alguma conexão adjacente passou do threshold
    if(possibleAlocXT == Def::MAX_DOUBLE){
        return false;
    }
    if(possibleAlocXT == 0){
        return true;
    }
    else{
        possibleAlocXT = General::linearTodB(possibleAlocXT);
        if(possibleAlocXT < xt)
            return true;
    }
    return false;
}

////////////////////////////////////////////////////////////
void Topology::print(){

}

// ------------------------------------------------- //
void Topology::printAllRoutes(){
    Route* route;
    //Imprimir Rotas:
	for(int orN = 0; orN < Topology::getNumNodes(); orN++)
		for(int deN = 0; deN < Topology::getNumNodes(); deN++){
			if(orN != deN){
				cout<<endl<<"[orN="<<orN<<"  deN="<<deN<<"]" <<endl;
				for(unsigned int p = 0; p < Topology::getRoutes(orN, deN)->size(); p++){
                    cout<<"Route("<<p<<"): ";
                    route = Topology::getRoutes(orN, deN)->at(p);
                    printRoute(route);
                }
            }
        }
}

// ------------------------------------------------- //
void Topology::printRoute(Route* route){
    int hops = route->getNumHops();
    cout<<"hops="<<hops<<":";
    if(hops >= 0)
        for(int h = 0; h <= hops; h++)
            cout<<route->getNode(h)<<"-";
    cout<<endl;
}

// ------------------------------------------------- //
void Topology::printAllNodes(){
    for(unsigned int i = 0; i < vNodes.size(); i++){
        vNodes.at(i)->print();
        cout<<endl;
    }
}

void Topology::printRouteState(Route* route,int numSlots,int nucleo,int iniSlot){
int ori,dest;
    for(int hops = 0;hops < route->getNumHops();hops++){
        cout<< "Núcleo " << nucleo;cout << endl;
        cout<< "Slot inicial " << iniSlot;cout << endl;
        cout<< "Numero de Slots " << numSlots;cout << endl;
        cout<< "Hop " << hops;cout << endl;
        ori = route->getNode(hops);
        dest = route->getNode(hops + 1);
        Fiber* fiber = Topology::getFiber(ori,dest);
        Core* core;
        for(int c = 0;c < Topology::getNumCores();c++){
            cout<< "Core " << c;cout << endl;
            core = fiber->getFiberCore(c);
            for(int slot = 0;slot < Topology::getNumSlots();slot++){
                    bool a = fiber->getCoreOccupation(slot,core);
                    if(slot < 100){
                        if(fiber->getCoreOccupation(slot,core))
                            if(core->getSlotAssignment(slot)->getLastSlot() == slot){
                                cout<<"["<<slot<<"]  ";cout<< "-";
                            }
                        cout<<"["<<slot<<"]  ";
                        cout<< fiber->getCoreOccupation(slot,core);

                        if(slot % 10 == 0 && slot != 0)
                               cout << endl;
                    }
                    else{
                        cout<<"["<<slot<<"]  ";cout<< fiber->getCoreOccupation(slot,core);
                        if(slot % 10 == 0)
                            cout << endl;
                    }
            }
            cout<<endl;
        }
    }
}

void Topology::setTotalSlots(){
        totalSlots = numCores*numFibers*numSlots;
}

int Topology::getTotalSlots(){
        return totalSlots;
}

void Topology::setNU(float nu){
    NU += nu;
}

float Topology::getNU(){
    return NU;
}

void Topology::updateNU(){
//Verificar número total de slots disponiveis
Link* link;
Fiber* fiber;
Core* core;
int numoccuSlots = 0,f = float(totalSlots);
    for(int ori = 0;ori < Topology::numNodes;ori++)
        for(int des = 0;des < Topology::numNodes;des++)
                if(Topology::getLink(ori,des) != NULL){
                    fiber = Topology::getFiber(ori,des);
                    for(int c = 0;c < Topology::getNumCores();c++){
                        core = fiber->getFiberCore(c);
                        for(int slot = 0;slot < Topology::getNumSlots();slot++)
                            if(fiber->getCoreOccupation(slot,core))
                                    numoccuSlots++;
                    }
                }
    Topology::setNU(float(numoccuSlots)/float(totalSlots));
}
