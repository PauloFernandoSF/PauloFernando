#include "Core.h"
#include <vector>

using namespace std;

Core::Core(CoreId cId,NumSlots nSlots)
:coreId (cId){
//Set number of adjacent cores-6
if(cId == 5)
    numAdjCores = cId;
else
    numAdjCores = 3;

    vDisp = new bool[nSlots];
    for(int i=0;i < nSlots;i++){
        vDisp[i]= false;
        vecAssignment.push_back(NULL);
        slotXT.push_back(0);
    }
    for(int cores = 0;cores < numAdjCores;cores++){
        adjCore.push_back(NULL);
    }
    //For PSO Implementation
    num_req_3slots = 0;
    num_req_4slots = 0;
    num_req_5slots = 0;
    ////////////////////////
}
int Core::coreSize(){
//    return vDisp.size();
}
//Retorna 1 se estiver ocupado e 0 c.c
bool Core::getSlotOccupation(int sPosition){
    return vDisp[sPosition];
}
//Ocupa o slot atribuindo o valor 1
void Core::occupySlots(int sPosition,Assignment* assignment){
    for(int a = sPosition;a < sPosition + assignment->getNumSlots();a++){//CONSIDERANDO BANDA DE GUARDA
        assert(!this->getSlotOccupation(a));
        vDisp[a] = true;
        vecAssignment.at(a) = assignment;
    }
}

void Core::releaseSlots(int sPosition,Assignment* assignment){
    for(int a = sPosition;a < sPosition + assignment->getNumSlots();a++){//CONSIDERANDO BANDA DE GUARDA
        assert(this->getSlotOccupation(a));
        vDisp[a]= false;
        vecAssignment.at(a) = NULL;
        slotXT.at(a) = 0;
    }
}

Assignment* Core::getSlotAssignment(int slot){
            return vecAssignment.at(slot);
}

CoreId Core::getCoreId(){
    return coreId;
}

void Core::setAdjCores(int numCores){
//Pensando em 6 núcleos
    int x = coreId;
    //Núcleo do "meio" tem todos os núcleos como adjacentes
    if(coreId ==  numCores - 1){
        for(int core = 0;core < coreId;core++)
            adjCore.at(core) = core;
    }
    else{
        if(coreId == 0 || coreId == numCores - 2){
            if(coreId == 0){
                adjCore.at(0) = coreId + 1;
                adjCore.at(1) = numCores - 1;
                adjCore.at(2) = numCores - 2;
            }
            else{
                adjCore.at(0) = 0;
                adjCore.at(1) = numCores - 1;
                adjCore.at(2) = coreId - 1;
            }
        }
        else{
            adjCore.at(0) = coreId + 1;
            adjCore.at(1) = numCores - 1;
            adjCore.at(2) = coreId - 1;
        }
    }
        /*
        //Nucleo 0
        if(coreId == 0){
            adjCore.at(0) = coreId + 1;
            adjCore.at(1) = numCores - 1;
            adjCore.at(2) = numCores - 2;
        }
        //Nucleo 1
        if(coreId == 1){
            adjCore.at(0) = coreId - 1;
            adjCore.at(1) = numCores - 1;
            adjCore.at(2) = coreId + 1;
        }
        //Nucleo 2
        if(coreId == 2){
            adjCore.at(0) = coreId + 1;
            adjCore.at(1) = numCores - 1;
            adjCore.at(2) = coreId - 1;
        }
        if(coreId == 3){
            adjCore.at(0) = coreId + 1;
            adjCore.at(1) = numCores - 1;
            adjCore.at(2) = coreId - 1;
        }
        if(coreId == 4){
            adjCore.at(0) = 0;
            adjCore.at(1) = numCores - 1;
            adjCore.at(2) = coreId - 1;
        }

    }*/
}

int Core::getAdjCoreIndex(int pos){
    return adjCore.at(pos);
}

void Core::setNumAdjCores(int num){
    numAdjCores = num;
}

int Core::getNumAdjCores(){
    return numAdjCores;
}

void Core::setSlotXT(int slot,float xt){
    slotXT.at(slot) = xt;
}

float Core::getSlotXT(int slot){
    return slotXT.at(slot);
}
