//
//  Node.cpp
//  EONsimulator
//
//  Created by Raul Almeida on 14/04/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#include "Node.h"

////////////////////////////////////////////////////////////////////////
Node::Node(NodeId nid, int nReg)
:nodeId(nid), numReg(nReg){
    numUsedReg = 0;
}

////////////////////////////////////////////////////////////////////////
void Node::initialise(){
    assert(numUsedReg == 0);
    numUsedReg = 0;
}

////////////////////////////////////////////////////////////////////////
NodeId Node::getNodeId(){
    return nodeId;
}


////////////////////////////////////////////////////////////////////////
bool Node::assignAregenerator(){
    if(numUsedReg < numReg){
        numUsedReg++;
        return true;
    }
    return false;
}

// -------------------------------------------------------- //
bool Node::releaseAregenerator(){
    if(numUsedReg > 0){
        numUsedReg--;
        return true;
    }
    return false;
}

// -------------------------------------------------------- //
bool Node::isTrereFreeRegenerator(){
    if(numUsedReg < numReg)
        return true;
    return false;
}

////////////////////////////////////////////////////////////////////////
void Node::print(){
    cout<<"Node Id="<<nodeId<<" numReg="<<numReg<<" numUsedReg="<<numUsedReg;
}
