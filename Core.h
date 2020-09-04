#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED


#include <vector>
#include <cstddef>
#include <assert.h>
#include "Assignment.h"


using namespace std;
typedef int CoreId;
typedef int NumSlots;

class Core{
public:
     Core(CoreId cId,NumSlots nSlots);
     CoreId getCoreId();
     bool getSlotOccupation(int sPosition);
     void occupySlots(int,Assignment*);
     void releaseSlots(int,Assignment*);
     int  coreSize();
     Assignment* getSlotAssignment(int slot);
     void setAdjCores(int);
     int getAdjCoreIndex(int core);
     void setNumAdjCores(int num);
     int getNumAdjCores();
     void setSlotXT(int,float);
     float getSlotXT(int);
     //FOR PSO IMPLEMENTATION
     unsigned int num_req_3slots;
     unsigned int num_req_4slots;
     unsigned int num_req_5slots;
     ////////////////////////
private:
     CoreId coreId;
     bool* vDisp;
     int numAdjCores;
     vector<Assignment*> vecAssignment;
     vector<int> adjCore;
     vector<float> slotXT;

};

#endif // CORE_H_INCLUDED
