#ifndef Route_H
#define Route_H

#include <assert.h>
#include <vector>
#include <iostream>
using namespace std;
#include "Link.h"
//
class Topology;

//////////////////////////////////////////////
class Route{
public:
	Route(vector<int>&);
	int getOrN() const;
	int getDeN() const;
	unsigned int getNumHops() const;
    unsigned int getNumNodes() const;
	int getNode(unsigned int) const;
	void addNodeAt(unsigned int pos, int node);
	void print() const;
	vector<int>* getPath();
	vector<int>  GetPath();
	double getCost();
    //
    Route* createPartialRoute(int n1Ind, int n2Ind);
    Route* addRoute(Route*);
private:
	vector<int> Path;
};

#endif
