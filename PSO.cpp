#include "PSO.h"
#include <iomanip>
//PSO Parameters
unsigned int PSO::Generations = 100;
unsigned int PSO::Population_Size  = 40;
unsigned int PSO::D1  = 5;
unsigned int PSO::D2  = 5;
unsigned int PSO::D3  = 5;
unsigned int PSO::NeighborsNum = 2;
double PSO::C1 = 2.05;//Daniel
double PSO::C2 = 2.05;//Daniel
double PSO::Xsi = 0.72984; //Daniel
double PSO::Maximum = 1;//Boundries to initialize position and velocity
double PSO::Minimum = -1;
int    PSO::Ressimulation = 1;
double PSO::GlobalBestFitness = Def::MAX_DOUBLE;
unsigned int PSO::BestIndividualIndex = Def::MAX_INT;
vector<PSO_Individual*> PSO::Population(0);
vector< vector < vector<float> > > PSO::GlobalBestPosition;
////////////////

void PSO::Initialize(){
printLogPSO();
double fitness;
initializeGlobalBest(D1,D2,D3);
    for(unsigned int a = 0;a < Population_Size;a++){
        Population.push_back(NULL);
        Population.at(a) = new PSO_Individual(D1,D2,D3,Maximum,Minimum,Population_Size,a);
        fitness = Kernel::Simulation(a,0);
        Population.at(a)->setCurrentFitness(fitness);
        Population.at(a)->setLocalBestFitness(fitness);
        Ressim(a,Ressimulation);
    }
    BestIndividualIndex = Def::MAX_INT;
    Operation();
}

void PSO::Operation(){
double fitness;
    for(unsigned int a = 0;a < Generations;a++){
      for(unsigned int b = 0;b < Population_Size;b++){
            //Best Neighbour
            updateLocalBestIndex(b);
            //Update velocity, position and set fitness
            updateParticle(Population.at(b),b);
            //update local best position
            if(Population.at(b)->getCurrentFitness() < Population.at(b)->getLocalBestFitness()){
                Population.at(b)->setLocalBestFitness(Population.at(b)->getCurrentFitness());
                updateLocalBestPosition(Population.at(b));
            }
            //save global best
            if(Population.at(b)->getCurrentFitness() < GlobalBestFitness){
                GlobalBestFitness = Population.at(b)->getCurrentFitness();
                BestIndividualIndex = b;
                setGlobalBestPosition(BestIndividualIndex);
            }
      }
      //Print best fitness of the generation
      printBestFitness(a);
      //Last Generation,keep variables values!
      if(a == Generations - 1){
            printBestIndividual();
            //printBestIndividualVelocity();
            break;
      }
      //Reset variables for next generation,each individual can change from generation to generation!
      //BestIndividualIndex = Def::MAX_INT;
      //GlobalBestFitness   = Def::MAX_DOUBLE;
      //}
    }
    //printBestIndividual();
}

void PSO::BestIndividual(){
Population.push_back(NULL);
Population.at(0) = new PSO_Individual(D1,D2,D3);
printIndividual(0);
}

void PSO::updateLocalBestIndex(unsigned int index){
double bestFit = Def::MAX_DOUBLE;
unsigned int bestIndex = Def::MAX_INT;
vector< unsigned int > vec = Population.at(index)->getNeighborVec();
    for(unsigned int a = 0;a < NeighborsNum;a++){
        if(Population.at(vec.at(a))->getCurrentFitness() < bestFit){
            bestFit = Population.at(vec.at(a))->getCurrentFitness();
            bestIndex = vec.at(a);
        }
    }
    Population.at(index)->setBestNeighborIndex(bestIndex);
}

void PSO::updateParticle(PSO_Individual* individual,unsigned int index){
double E1 = General::uniform(0.0,1.0), E2 = General::uniform(0.0,1.0),fitness;
float old_velocity, new_velocity,new_pos;
unsigned int neighborIndex = individual->getBestNeighborIndex();
    for(unsigned int a = 0;a < D1;a++)
        for(unsigned int b = 0;b < D2;b++)
            for(unsigned int c = 0;c < D3;c++){
               old_velocity = individual->getVelocity(a,b,c);

               new_velocity = Xsi*(old_velocity + C1*E1*(individual->getLocalBestPosition(a,b,c) - individual->getCurrentPosition(a,b,c))
               + C2*E2*(Population.at(neighborIndex)->getCurrentPosition(a,b,c) - individual->getCurrentPosition(a,b,c)));
               new_pos      = individual->getCurrentPosition(a,b,c) + new_velocity;
               //Limitation of search space for position[-1 to 1], reflect method Xt+1 = Xmax- (Xt+1 - Xmax)
               //Xt+1 = Xmin+(Xmin - Xt+1)
               //individual->setVelocity(a,b,c, new_velocity);
               if(new_pos > 1.0 || new_pos < -1.0){
                    if(new_pos > 1.0)
                        //Limite superior
                        //individual->setCurrentPosition(a,b,c,(Maximum));
                        //Reflexão
                        individual->setCurrentPosition(a,b,c,(Maximum - (new_pos - Maximum)));
                    else
                        //Limite inferior
                        //individual->setCurrentPosition(a,b,c,(Minimum));
                        //Reflexão
                        individual->setCurrentPosition(a,b,c,(Minimum + (Minimum - new_pos)));
                    //Limit velocity? Zero method -> whenever a partcile position is out of the search space, velocity is set to ZERO!
                    individual->setVelocity(a,b,c, 0.0);
               }
               else{
                    individual->setCurrentPosition(a,b,c,new_pos);
                    individual->setVelocity(a,b,c, new_velocity);

               }
           }

    fitness = Kernel::Simulation(index,0);
    Population.at(index)->setCurrentFitness(fitness);
    Ressim(index,Ressimulation);
}

void PSO::setGlobalBestPosition(unsigned int index){
    for(unsigned int a = 0;a < D1;a++)
        for(unsigned int b = 0;b < D2;b++)
            for(unsigned int c = 0;c < D3;c++)
                GlobalBestPosition.at(a).at(b).at(c) = Population.at(index)->getCurrentPosition(a,b,c);

}

double PSO::getGlobalBestPosition(unsigned int d1,unsigned int d2,unsigned int d3){
    return GlobalBestPosition.at(d1).at(d2).at(d3);
}

void PSO::updateLocalBestPosition(PSO_Individual* ind){
    for(unsigned int a = 0;a < D1;a++)
        for(unsigned int b = 0;b < D2;b++)
            for(unsigned int c = 0;c < D3;c++)
                ind->setLocalBestPosition(a,b,c, ind->getCurrentPosition(a,b,c));
}

PSO_Individual* PSO::getPopulationElement(unsigned int position){
    return Population.at(position);
}

void PSO::printBestFitness(unsigned int generation){
    InpOut::PSO_Results << generation << "   " << GlobalBestFitness<< endl;
}

unsigned int PSO::getD1(){
    return D1;
}

unsigned int PSO::getD2(){
    return D2;
}

unsigned int PSO::getD3(){
    return D3;
}

void PSO::printBestIndividual(){
    for(unsigned int a = 0;a < D1;a++)
        for(unsigned int b = 0;b < D2;b++)
            for(unsigned int c = 0;c < D3;c++){
                if(c == D3 - 1){
                    InpOut::PSO_Results<<fixed << setprecision(7);
                    InpOut::PSO_Results<< GlobalBestPosition.at(a).at(b).at(c)  << "        " << endl;
                }
                else{
                    InpOut::PSO_Results<<fixed << setprecision(7);
                    InpOut::PSO_Results << GlobalBestPosition.at(a).at(b).at(c) << "        ";
                }
            }
}

void PSO::printBestIndividualVelocity(){
for(unsigned int a = 0;a < D1;a++)
        for(unsigned int b = 0;b < D2;b++)
            for(unsigned int c = 0;c < D3;c++){
                if(c == D3 - 1){
                    InpOut::PSO_Results<<fixed << setprecision(7);
                    InpOut::PSO_Results<< Population.at(BestIndividualIndex)->getVelocity(a,b,c) << "        " << endl;
                }
                else{
                    InpOut::PSO_Results<<fixed << setprecision(7);
                    InpOut::PSO_Results << Population.at(BestIndividualIndex)->getVelocity(a,b,c) << "        ";
                }
            }
}

void PSO::initializeGlobalBest(unsigned int d1,unsigned int d2,unsigned int d3){
GlobalBestPosition.resize(d1);
    for(unsigned int a = 0;a < d1;a++){
       GlobalBestPosition.at(a).resize(d2);
       for(unsigned int b = 0;b < d2;b++)
           GlobalBestPosition.at(a).at(b).resize(d3);
    }
}

void PSO::printIndividual(int position){
    for(unsigned int a = 0;a < D1;a++)
        for(unsigned int b = 0;b < D2;b++)
            for(unsigned int c = 0;c < D3;c++){
                if(c == D3 - 1){
                    InpOut::PSO_Results<<fixed << setprecision(7);
                    InpOut::PSO_Results<< Population.at(position)->getCurrentPosition(a,b,c)  << "        " << endl;
                }
                else{
                    InpOut::PSO_Results<<fixed << setprecision(7);
                    InpOut::PSO_Results << Population.at(position)->getCurrentPosition(a,b,c) << "        ";
                }
            }
}

void PSO::printLogPSO(){

    InpOut::PSO_Log<< "Generations"  <<":    "<< Generations                <<endl;
    InpOut::PSO_Log<< "Population"   <<":     "<< Population_Size                 <<endl;
    InpOut::PSO_Log<< "D1" <<":   "<< D1          <<endl;
    InpOut::PSO_Log<< "D2" <<":   "<< D2          <<endl;
    InpOut::PSO_Log<< "D3" <<":   "<< D3          <<endl;
    InpOut::PSO_Log<< "c1" <<":   "<< C1          <<endl;
    InpOut::PSO_Log<< "c2" <<":   "<< C2          <<endl;
    InpOut::PSO_Log<< "Xsi"<<":   "<< Xsi         <<endl;
    InpOut::PSO_Log<< "Quantidades de ressimulações"<<":   "<< Ressimulation         <<endl;
}

//Function to ressimulate individual in the vector -vec- in the position -pos- for number of times -num_times-
void PSO::Ressim(int pos,int num_times){
long double pb_old,pb_new,pb_med;

    for(int a = 0;a < num_times;a++){
            pb_old = Population.at(pos)->getCurrentFitness();
            pb_new = Kernel::Simulation(pos,0);
            pb_med = (pb_old + pb_new)/2;
            Population.at(pos)->addContSim();
            Population.at(pos)->setCurrentFitness(pb_med);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
