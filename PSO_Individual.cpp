#include "PSO_Individual.h"
#include <iomanip>


PSO_Individual::PSO_Individual(unsigned int d1,unsigned int d2,unsigned int d3,double maximum,double minimum,unsigned int pop,unsigned int index){
initializeAttributes(d1,d2,d3);
initializeNeighbors(pop,index);
this->cont_sim = 0;
    for(unsigned int a = 0;a < d1;a++)
        for(unsigned int b = 0;b < d2;b++)
            for(unsigned int c = 0;c < d3;c++){
                float coefficient_vel = General::uniform(minimum,maximum),coefficient_pos = General::uniform(minimum,maximum);
                Velocity.at(a).at(b).at(c) = coefficient_vel;
                CurrentPosition.at(a).at(b).at(c) = coefficient_pos;
                LocalBestPosition.at(a).at(b).at(c) = coefficient_pos;
            }
}

PSO_Individual::PSO_Individual(unsigned int d1,unsigned int d2,unsigned int d3){
initializeAttributes(d1,d2,d3);
this->cont_sim = 0;
    for(unsigned int a = 0;a < d1;a++)
        for(unsigned int b = 0;b < d2;b++)
            for(unsigned int c = 0;c < d3;c++){
                float coefficient = readIndividual();
                CurrentPosition.at(a).at(b).at(c) = coefficient;
            }
}

float PSO_Individual::readIndividual(){
float coefficient;
    InpOut::PSO_Individual >> coefficient;
return coefficient;
}
//2 NEIGHBORS
void PSO_Individual::initializeNeighbors(unsigned int pop,unsigned int index){
unsigned int bestNeighborIndex = Def::MAX_INT;
        //Conditions for ring topology - edges - 2 neighbors
        if(index == 0 || index == pop - 1){
                if(index == 0){
                    neighborsIndex.push_back(index + 1);
                    neighborsIndex.push_back(pop - 1);
                }
                else{
                    neighborsIndex.push_back(0);
                    neighborsIndex.push_back(index - 1);
                }
        }
        ////////////////////////////////////////////////////
        //Conditions outside edges
        else{
            neighborsIndex.push_back(index + 1);
            neighborsIndex.push_back(index - 1);
        }
        ///////////////////////////
}
/////////////
void PSO_Individual::setBestNeighborIndex(unsigned int best){
        bestNeighborIndex = best;
}

unsigned int PSO_Individual::getBestNeighborIndex(){
        return bestNeighborIndex;
}

vector< unsigned int > PSO_Individual::getNeighborVec(){
    return neighborsIndex;
}

void PSO_Individual::initializeAttributes(unsigned int d1,unsigned int d2,unsigned int d3){
Velocity.resize(d1);
CurrentPosition.resize(d1);
LocalBestPosition.resize(d1);
    for(unsigned int a = 0;a < d1;a++){
       Velocity.at(a).resize(d2);
       CurrentPosition.at(a).resize(d2);
       LocalBestPosition.at(a).resize(d2);
       for(unsigned int b = 0;b < d2;b++){
           Velocity.at(a).at(b).resize(d3);
           CurrentPosition.at(a).at(b).resize(d3);
           LocalBestPosition.at(a).at(b).resize(d3);
       }
    }
}

void PSO_Individual::printVelocity(unsigned int D1,unsigned int D2,unsigned int D3){
for(unsigned int a = 0;a < D1;a++)
        for(unsigned int b = 0;b < D2;b++)
            for(unsigned int c = 0;c < D3;c++){
                if(c == D3 - 1){
                    InpOut::PSO_Results<<fixed << setprecision(7);
                    InpOut::PSO_Results<< this->getVelocity(a,b,c) << "        " << endl;
                }
                else{
                    InpOut::PSO_Results<<fixed << setprecision(7);
                    InpOut::PSO_Results << this->getVelocity(a,b,c) << "        ";
                }
            }
}

void PSO_Individual::Initialize(){
}

PSO_Individual::~PSO_Individual()
{
}

void PSO_Individual::setCurrentFitness(double fit){
    CurrentFitness = fit;
}

double PSO_Individual::getCurrentFitness(){
    return CurrentFitness;
}

void PSO_Individual::setCurrentPosition(unsigned int d1,unsigned int d2,unsigned int d3,float pos){
    CurrentPosition.at(d1).at(d2).at(d3) = pos;
}

float PSO_Individual::getCurrentPosition(unsigned int d1,unsigned int d2,unsigned int d3){
    return CurrentPosition.at(d1).at(d2).at(d3);
}

void PSO_Individual::setVelocity(unsigned int d1,unsigned int d2,unsigned int d3,float vel){
    Velocity.at(d1).at(d2).at(d3) =  vel;
}

float PSO_Individual::getVelocity(unsigned int d1,unsigned int d2, unsigned int d3){
    return Velocity.at(d1).at(d2).at(d3);
}

void PSO_Individual::setLocalBestPosition(unsigned int d1,unsigned int d2,unsigned int d3,float pos){
    LocalBestPosition.at(d1).at(d2).at(d3) = pos;
}

float PSO_Individual::getLocalBestPosition(unsigned int d1,unsigned int d2,unsigned int d3){
    return LocalBestPosition.at(d1).at(d2).at(d3);
}

void PSO_Individual::setLocalBestFitness(double fit){
    LocalBestFitness = fit;
}

double PSO_Individual::getLocalBestFitness(){
    return LocalBestFitness;
}

void PSO_Individual::addContSim(){
    this->cont_sim++;
}

int PSO_Individual::getContSim(){
    return this->cont_sim;
}
