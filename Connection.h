#ifndef Connection_H
#define Connection_H


#include <assert.h>
#include "Def.h"
//#include "Route.h"
class Topology;
//#include "Topology.h"
//class Assignment;
#include "Assignment.h"
#include "Schedule.h"


//////////////////////////////////////////////
class Connection{
public:
    Connection(Assignment *assign);
    ~Connection();
    Assignment* getAssignment();
	int getFirstSlot();
	void incFirstSlot(int);
	int getLastSlot();
	void incLastSlot(int);
    // Expansion and Compression:
    void expandRight();
    void expandLeft();
    void compressRight();
    void compressLeft();
    //
    void print();
    //void releaseSlot(int s);
private:
    Assignment *assignment;
};

#endif
