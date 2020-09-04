#include "General.h"

////////////////////////////////////////////////////////////////////////////////
double General::exponential(double L){
	return (double) -log(1.0-uniform(0.0,1.0))/L;
}

// -------------------------------------------------------------------------- //
double General::uniform(double xmin, double xmax){
	double va = powl(RAND_MAX,-3);
	do {
		va = (double) rand()/(RAND_MAX);
		va += ((double) rand()/(RAND_MAX))/(RAND_MAX);
		va += (((double) rand()/(RAND_MAX))/(RAND_MAX))/(RAND_MAX);
	}
	while( (va<=0) || (va>=1.0-powl(RAND_MAX,-3)) );
	return xmin + va*(xmax - xmin);
}

// -------------------------------------------------------------------------- //
int General::uniform(int xmin, int xmax){
    double va;
    do {
        va = (double) rand()/(RAND_MAX);
        va += ((double) rand()/(RAND_MAX))/(RAND_MAX);
        va += (((double) rand()/(RAND_MAX))/(RAND_MAX))/(RAND_MAX);
    }
    while( (va<=0) || (va>=1.0-powl(RAND_MAX,-3)) );
    return floor(xmin + va*(xmax - xmin));
}

////////////////////////////////////////////////////////////////////////////////
void General::appendVector(vector<int>* v1, const vector<int>* v2){
    for(unsigned int i = 0; i < v2->size(); i++)
        v1->push_back(v2->at(i));
}

////////////////////////////////////////////////////////////////////////////////
double General::linearTodB(double x){
    return 10*log10(x);
}

// --------------------------------------------------------- //
double General::linearWTodBm(double powerWatts){
    return 10*log10(powerWatts*1000);
}

// --------------------------------------------------------- //
double General::dBtoLinear(double x){
    return pow(10.0,x/10.0);
}

