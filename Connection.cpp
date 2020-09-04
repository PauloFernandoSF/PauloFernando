#include "Connection.h"
#include "Topology.h"

////////////////////////////////////////////////////////////////
Connection::Connection(Assignment *assign)
:assignment(assign){
}

// ------------------------------------------------- //
Connection::~Connection(){
    delete assignment;
}

////////////////////////////////////////////////////////////////
Assignment*  Connection::getAssignment(){
    return assignment;
}

////////////////////////////////////////////////////////////////
int Connection::getFirstSlot(){
	return assignment->getFirstSlot();
}

// ------------------------------------------------- //
void Connection::incFirstSlot(int x){
	assert( (assignment->getFirstSlot() + x >= 0) && (assignment->getFirstSlot() + x < Topology::getNumSlots()) );
}

// ------------------------------------------------- //
int Connection::getLastSlot(){
	return assignment->getLastSlot();
}

// ------------------------------------------------- //
void Connection::incLastSlot(int x){
	assert( (assignment->getLastSlot() + x >= 0) && (assignment->getLastSlot() + x < Topology::getNumSlots()) );
}

//////////////////////////////////////////////////////////////////
void Connection::expandRight(){
    assert(getLastSlot() < Topology::getNumSlots()-1);
    Topology::occupySlot(this, getLastSlot()+1);
    this->incLastSlot(+1);
}

// ------------------------------------------------- //
void Connection::expandLeft(){
    assert(getFirstSlot() > 0);
    Topology::occupySlot(this, getFirstSlot()-1);
    this->incFirstSlot(-1);
}

// ------------------------------------------------- //
void Connection::compressRight(){
    Topology::releaseSlot(this, getLastSlot());
    this->incLastSlot(-1);
}

// ------------------------------------------------- //
void Connection::compressLeft(){
    Topology::releaseSlot(this, getFirstSlot());
    this->incFirstSlot(+1);
}


//////////////////////////////////////////////////////////////////
void Connection::print(){
    assignment->print();
}

/*
// ------------------------------------------------- //
void Connection::releaseSlot(int s){
    Topology::CompressSlot(this, s);
    this->incLastSlot(-1);
}
 */

