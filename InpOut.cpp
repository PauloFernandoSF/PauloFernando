#include "InpOut.h"
#include <stdio.h>

ifstream InpOut::topology("Topology.txt");
ifstream InpOut::traffic("Traffic.txt");
ifstream InpOut::modulation("Modulation.txt");
ifstream InpOut::link("Link.txt");
ifstream InpOut::PSO_Individual("PSO_Individual.txt");

ofstream InpOut::Result("Result.txt");
ofstream InpOut::GA_Results("AG_Results.txt");
ofstream InpOut::GA_Better_Worst("AG_Better_Worst.txt");
ofstream InpOut::GA_Initial_Pop("AG_Initial_Pop.txt");
ofstream InpOut::Core_Freq("Core_Freq.txt");
ofstream InpOut::GA_Log("GA_Log.txt");
ofstream InpOut::PSO_Results("PSO.txt");
ofstream InpOut::PSO_Log("PSO_Log.txt");


InpOut::InpOut()
{

}
