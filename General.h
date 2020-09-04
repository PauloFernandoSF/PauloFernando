#ifndef General_H
#define General_H

#include <assert.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <iostream>
using namespace std;
#include <math.h>
#include "Route.h"


////////////////////////////////////////////////////////////////
class General{
	public:
		template <class T>
		static void setVector(T* V, int size, T value);
		template <class T>
		static void printAnsiVector(vector<T>&);
		static double exponential(double L);
		static double uniform(double xmin, double xmax);
        static int uniform(int xmin, int xmax);
        //
        template <class T>
        static void clearVectorPtr(vector<T*>* v);
        template <class T>
        static void removeFromVectorPtr(T*);
        static void appendVector(vector<int>*, const vector<int>*);
        template <class T>
        static void addVectorPtr(vector<T*>* v1, vector<T*>* v2); //Copy v2 to v1

        //
		template <class T>
		static const T& ABS(const T& x);
		template <class T>
		static const T& MAX (const T& a, const T& b);
		template <class T>
		static const T& MIN (const T& a, const T& b);




		//template <class T>
		//static ofstream&(const vector<T>&);
        static double linearTodB(double x);
        static double linearWTodBm(double x);
        static double dBtoLinear(double x);
	private:

};

////////////////////////////////////////////////////////////////
/*
template <class T>
void General::setVector(T* V, int size, T value){
	for(int i = 0; i < size; i++)
		V[i] = value;
}*/

// -------------------------------------------------------------------------- //
template <class T>
void General::printAnsiVector(vector<T>& v){
	for(unsigned i = 0; i < v.size(); i++)
		cout<<v.at(i)<<" ";
}

////////////////////////////////////////////////////////////////////////////////
template <class T>
const T& General::ABS(const T& x){
	return (x>0)?x:-x;
}

// -------------------------------------------------------------------------- //
template <class T>
const T& General::MAX (const T& a, const T& b){
	return (a>b)?a:b;
}

// -------------------------------------------------------------------------- //
template <class T>
const T& General::MIN (const T& a, const T& b){
	return (a<b)?a:b;
}


////////////////////////////////////////////////////////////////////////////////
template <class T>
void General::clearVectorPtr(vector<T*>* v){
    for(unsigned int i = 0; i < v->size(); i++)
        delete v->at(i);
    v->clear();
}




// --------------------------------------------------------- //
template <class T>
void General::addVectorPtr(vector<T*>* v1, vector<T*>* v2){ //Copy v2 to v1
    for(int i = 0; i < v2->size(); i++)
        v1->push_back(v2->at(i));
}



#endif
