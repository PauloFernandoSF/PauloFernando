//
//  Node.h
//  EONsimulator
//
//  Created by Raul Almeida on 14/04/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#ifndef __EONsimulator__Node__
#define __EONsimulator__Node__

#include <stdio.h>
#include <iostream>
#include <assert.h>
//#include "InpOut.h"
using namespace std;


typedef unsigned int NodeId;

////////////////////////////////////////////////////////////////////////
class Node{
public:
    Node(NodeId nid, int nReg);
    //
    void initialise();
    //
    NodeId getNodeId();
    //
    bool assignAregenerator();
    bool releaseAregenerator();
    bool isTrereFreeRegenerator();
    //
    void print();
private:
    NodeId nodeId;
    int numReg;
    int numUsedReg;
};

#endif /* defined(__EONsimulator__Node__) */
