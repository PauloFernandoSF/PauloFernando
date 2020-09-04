#ifndef ROUTING_H_INCLUDED
#define ROUTING_H_INCLUDED

#include "Topology.h"
#include "Assignment.h"
#include <queue>


struct RouteCompare{
        /**
     * @brief Compare the cost of two routes.
     * @param routeA Route to be compared with.
     * @param routeB Route to compare.
     * @return true if the cost of routeA is larger than routeB.
     * False otherwise.
     */
        bool operator()( Route* & routeA,
                         Route* & routeB);
    };
///////////////////////////////////////////////////////////////
class Routing{
public:
    static void Dijkstra();
    static void Dijkstra(Assignment*);
    static Route* Dijkstra(int orN, int deN);
    //
    static void setKYEN(int K);
    static void Yen(Assignment*);
    static void Yen(int K);
    static void Yen(int orN, int deN, int KYEN, vector<Route*>&A);
    static void Yen(int orN, int deN, int K);
    static vector<Route*> Yen(unsigned int orN,unsigned int deN, int K);
public:
    static int KYEN;
};

#endif // ROUTING_H_INCLUDED
