#include "Routing.h"

int Routing::KYEN = 1;


//////////////////////////////////////////////////////////////////////////////////////////////
void Routing::Dijkstra(Assignment* assign){ //// Função Dijkstra que retorna o caminho mais curto dado como entrada os
    assert(assign->popTrialRoute() == NULL);
    assign->pushTrialRoute(Dijkstra(assign->getOrN(), assign->getDeN()));
}
bool RouteCompare::operator()( Route*  & routeA,
                               Route*  & routeB) {
    return (routeA->getCost() > routeB->getCost());
}
// ---------------------------------------------------------------- //
void Routing::Dijkstra(){ // Função Dijkstra para retornar os caminhos de todos os pares de nós da rede
    Route *route;
    for(int orN = 0; orN < Topology::getNumNodes(); orN++)
        for(int deN = 0; deN < Topology::getNumNodes(); deN++){
            if(orN != deN)
                route = Dijkstra(orN, deN);
            else
                route = NULL;
            Topology::setRoute(orN, deN, route);
        }
}
// -------------------------------------------------------------------------- //
Route* Routing::Dijkstra(int orN, int deN){ //Nós de origem e destino da requisição
    int VA, i, j, k=-1, h, hops; long double min;
    vector<int> r;
    Link *link;
    Route *routeDJK = NULL;
    double *CustoVertice = new double[Topology::getNumNodes()];
    int *Precedente = new int[Topology::getNumNodes()];
    int *PathRev = new int[Topology::getNumNodes()];
    bool *Status = new bool[Topology::getNumNodes()]; //Variável para armazenar os nós que ja foram usados no algoritmo
    assert(orN != deN);
    bool networkDisconnected = false;
    //Busca para todos os pares de nó a rota mais curta:
    //Inicializa os nós com custo infinito, exceto o nó de origem
    for(i = 0; i < Topology::getNumNodes(); i++){
        if(i != orN)
            CustoVertice[i] = Def::MAX_DOUBLE;
        else
            CustoVertice[i] = 0.0;
        Precedente[i] = -1;
        Status[i] = 0;
    }
    /////////////////////////////////////////////////////////////
    VA = Topology::getNumNodes();
    while(VA > 0 && !networkDisconnected){
        min = Def::MAX_DOUBLE;
        //Loop para encontrar vértice com o menor custo e que ainda não foi usado
        for(i = 0; i < Topology::getNumNodes(); i++)
            if((Status[i] == 0)&&(CustoVertice[i] < min)){
                min = CustoVertice[i];
                k = i;
            }
        /////////////////////////////////////////////////////////////////////////
        if(k == deN)
            break;
        Status[k] = 1;//Nó k é o próximo a ser usado no algoritmo
        VA = VA-1;
        bool outputLinkFound = false;
        for(j = 0; j < Topology::getNumNodes(); j++){
            link = Topology::getLink(k,j);
            if((link != NULL) && (link->isWorking()) && (Topology::isNodeWorking(link->getOrNode())) && (Topology::isNodeWorking(link->getDeNode())) ){
                outputLinkFound = true;
                if((Status[j] == 0) && (CustoVertice[k] + link->getCost() < CustoVertice[j])){
                    CustoVertice[j] = CustoVertice[k]+link->getCost();
                    Precedente[j] = k;
                }
            }
        }
        if(!outputLinkFound)
            networkDisconnected = true;
    }//Fim do while
    if(!networkDisconnected){
        PathRev[0] = deN;
        hops = 0;
        j = deN;
        while(j != orN){
            hops = hops+1;
            if(Precedente[j] != -1){
                PathRev[hops] = Precedente[j];
                j = Precedente[j];
            }
            else{
                networkDisconnected = true;
                break;
            }
        }
        if(!networkDisconnected){
            r.clear();
            for(h = 0; h <= hops; h++)
                r.push_back(PathRev[hops-h]);
            routeDJK = new Route(r);
        }
    }
    delete []CustoVertice; delete []Precedente; delete []Status; delete []PathRev;
    return routeDJK;

}
//************************************************************************** //
void Routing::setKYEN(int k){
    Routing::KYEN = k;
}
// -------------------------------------------------------------------------- //
void Routing::Yen(Assignment *assign){
    vector<Route*>* vRoutes = Topology::getRoutes(assign->getOrN(),assign->getDeN());
    assign->pushTrialRoutes(*vRoutes);
}
// -------------------------------------------------------------------------- //
void Routing::Yen(int KYEN){
    vector<Route*> routes;
    unsigned int numNodes = Topology::getNumNodes();
    for(unsigned int orN = 0; orN < numNodes; orN++)
        for (unsigned int deN = 0; deN < numNodes; deN++){
            if(orN != deN){
                routes = Routing::Yen(orN, deN, KYEN);
                for(unsigned int i = 0;i < routes.size();i++){
                        //Topology::printRoute(routes.at(i));
                }
                Topology::setRoutes(orN, deN, &routes);
            }
            else
                routes.resize(KYEN,nullptr);
        }

            routes.clear();
}
// -------------------------------------------------------------------------- //
void Routing::Yen(int orN, int deN, int KYEN){
    vector<Route*> vRoutes;
    Routing::Yen(orN, deN, KYEN, vRoutes);
    Topology::setRoutes(orN, deN, &vRoutes);
}
// -------------------------------------------------------------------------- //
void Routing::Yen(int orN, int deN, int KYEN, vector<Route*>&A){
    assert(orN != deN);
    int L_or, L_de; Route *routeYEN = NULL, *routeYENb, *routeAj, *routeSpur; // Mudei de nullptr para NULL
    int spurNode, bestRouteB;
    vector<Route*> B;
    vector<Link*> LinkBroken;
    vector<int> pathAcum, pathYENb;
    double cost, minCostB;
    Link *link;
    A.clear();
    B.clear();
    for(int k = 0; k < KYEN; k++){
        if(k == 0){ //Do Normal YEN
            routeYEN = Routing::Dijkstra(orN, deN);
            if(routeYEN == NULL)
                break;
            A.push_back(routeYEN);
        }
        else{ //Yen starting from the second route
            pathAcum.clear();
            LinkBroken.clear();
            for(unsigned int lk = 0; lk < routeYEN->getNumHops(); lk++){ //lDJK vai até o penúltimo nó
                spurNode = routeYEN->getNode(lk);
                pathAcum.push_back(spurNode);
                //Leva para infinito o custo de todos os enlaces a partir de spurNode das rotas em A que possuem a mesma rota de orN até spurNode
                for(unsigned int j = 0; j < A.size(); j++){
                    bool identicalAj = true;
                    routeAj = A.at(j);
                    if(lk < routeAj->getNumNodes()-1){ //routeAj has the lk-th node as well as the link lk-(lk+1)
                        //Check whether routeAj from node 0 to lk is equal to pathAcum
                        for(unsigned int n = 0; n <= lk; n++)
                            if(routeAj->getNode(n) != pathAcum.at(n)){
                                identicalAj = false;
                                break;
                            }
                    }
                    else
                        identicalAj = false;
                    if(identicalAj){ //Set link lk-(lk+1) to infinity
                        L_or = routeAj->getNode(lk);
                        L_de = routeAj->getNode(lk+1);
                        link = Topology::getLink(L_or, L_de);
                        //if (Topology::validLink(link))
                        link->setAsBroken();
                        LinkBroken.push_back(link);
                    }
                }
                //Set the cost of all nodes in PathAcum from 0 to lk-1:
                for(unsigned int i = 0; i < pathAcum.size()-1; i++)
                    Topology::setNodeAsBroken(pathAcum.at(i));
                //Calculates routeYEN from lk to destination node:
                routeSpur = Routing::Dijkstra(spurNode, deN); //routeSpur is the second half of the route
                //Restore the cost of all the links in linkBroken:
                for(unsigned int i = 0; i < LinkBroken.size(); i++)
                    LinkBroken.at(i)->setAsWorking();
                LinkBroken.clear();
                //restore the cost of all nodes in PathAcum from 0 to lk-1
                for(unsigned int i = 0; i < pathAcum.size()-1; i++)
                    Topology::setNodeAsWorking(pathAcum.at(i));
                //
                if(routeSpur != NULL){ //There is a shortest path between spurNode and deN;
                    pathYENb.clear(); //pathYEN will store the complete route form orN to deN;
                    //Insert the first half in pathYEN (without spurNode):
                    for(unsigned int n = 0; n < pathAcum.size()-1; n++)
                        pathYENb.push_back(pathAcum.at(n));
                    //Insert the second half in pathYEN (with spurNode):
                    for(unsigned int n = 0; n < routeSpur->getNumNodes(); n++)
                        pathYENb.push_back(routeSpur->getNode(n));
                    delete routeSpur;
                    routeYENb = new Route(pathYENb);
                    B.push_back(routeYENb);
                }
            }
            //Dentre todas as rotas inseridas em B, escolhe a de menor custo:
            bestRouteB = -1;
            minCostB = Def::MAX_DOUBLE;
            for(unsigned int i = 0; i < B.size(); i++){
                cost = B.at(i)->getCost();
                if(cost < minCostB){
                    minCostB = cost;
                    bestRouteB = i;
                }
            }
            if(bestRouteB == -1) //There is no route in B with acceptable cost
                break;
            //Remove From B the best route
            routeYEN = B.at(bestRouteB); //Update routeYEN
            B.erase (B.begin()+bestRouteB);
            A.push_back(routeYEN);
        }
    } //end of for(int k = 0; k < KYEN; k++){
    for(unsigned int i = 0; i < B.size(); i++)
        delete B.at(i);
}
// -------------------------------------------------------------------------- //
vector<Route*> Routing::Yen(unsigned int orN,unsigned int deN, int KYEN){
    assert(orN != deN);
    int L_or, L_de;// Mudei de nullptr para NULL
    int spurNode;
    vector<Route*> routesYen;
    Route* newRoute,* rootPath,* spurPath,*totalPath;
    vector<Link*> LinkBroken;
    vector<int>  pathYENb;
    double cost, minCostB;
    std::priority_queue<Route*,vector<Route*>,RouteCompare> candidateRoutes;
    Link *link;

    newRoute = Routing::Dijkstra(orN, deN);
    routesYen.push_back(newRoute);
          for(unsigned int k = 1; k < KYEN; k++){
            unsigned int auxSize = routesYen.at(k-1)->getNumNodes() - 2;

            for(unsigned int i = 0; i <= auxSize; i++){ //lDJK vai até o penúltimo nó
                //Topology::printRoute(routesYen.at(k-1));
                spurNode = routesYen.at(k-1)->getNode(i);
                rootPath = routesYen.at(k-1)->createPartialRoute(0,i);
                //Topology::printRoute(rootPath);
                //Leva para infinito o custo de todos os enlaces a partir de spurNode das rotas em routesYen que possuem links em comum com a rota em análise
                for(auto it: routesYen){
                    if(i < it->getNumNodes()){
                            newRoute = it->createPartialRoute(0,i);
                            //Topology::printRoute(newRoute);
                            if(rootPath->GetPath() == newRoute->GetPath()){
                                    Topology::getLink(spurNode,it->getNode(i + 1))->setAsBroken();
                            }
                    }
                }

                for(unsigned int a = 0; a < rootPath->getNumNodes(); a++){
                    if(rootPath->getNode(a) == spurNode)
                        continue;
                    Topology::setNodeAsBroken(rootPath->getNode(a));
                }
                spurPath = Dijkstra(spurNode,deN);
                //Topology::printRoute(spurPath);
                if(spurPath != nullptr){
                    totalPath = rootPath->addRoute(spurPath);
                    //Topology::printRoute(totalPath);
                    candidateRoutes.push(totalPath);

                }
                Topology::setAllLinksWorking();
                Topology::setAllNodesAsWorking(rootPath);
            }
            if(candidateRoutes.empty())
                break;
            routesYen.push_back(candidateRoutes.top());
            candidateRoutes.pop();
         }

    while(routesYen.size() < KYEN){
        routesYen.push_back(nullptr);
    }
    return routesYen;
}



