#include "Fiber.h"
using namespace std;

////////////////////////////////////////////////
Fiber::Fiber(FiberId fId,vector<Core*> vC,int oNode,int dNode )
:fiberId (fId),vCore(vC),orNode(oNode),deNode(dNode){

}

bool Fiber::getCoreOccupation(int sPosition,Core* core){
    return core->getSlotOccupation(sPosition);

}

Core* Fiber::getFiberCore(int cPosition){
    return vCore.at(cPosition);
}

void Fiber::setOrNode(int oNode){
    orNode = oNode;
}

// --------------------------------------------------- //
int Fiber::getOrNode(){
    return orNode;
}

// --------------------------------------------------- //
void Fiber::setDeNode(int dNode){
    deNode = dNode;
}

// --------------------------------------------------- //
int Fiber::getDeNode(){
    return deNode;
}


////////////////////////////////////////////////
FiberId Fiber::getFiberId(){
    return fiberId;
}
////////////////////////////////////////////////
