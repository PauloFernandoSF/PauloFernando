//Classe criada para adicionar o conceito de fibra ao simulador e trabalhar com fibras multi-núcleos

#ifndef FIBER_H_INCLUDED
#define FIBER_H_INCLUDED

using namespace std;
typedef int  FiberId;


#include "Core.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
class Fiber{
public:
     Fiber(FiberId fId,vector<Core*>vC,int oNode,int dNode);
     FiberId getFiberId();
     bool getCoreOccupation(int sPosition,Core* core);
     void setOrNode(int);
     int  getOrNode();
     void setDeNode(int);
     int  getDeNode();
     Core*getFiberCore(int);

private:
     int orNode;
     int deNode;
     FiberId fiberId;
     vector<Core*> vCore;


};

#endif // FIBER_H_INCLUDED
