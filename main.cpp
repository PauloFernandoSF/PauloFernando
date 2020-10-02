#include "Kernel.h"
//Variaveis para mensurar o tempo de simulação
clock_t tInicio, tFim, tDecorrido;
//VERIFICAR MSCL QUANDO ALTERAR REQUISIÇÕES!!!!!!!!!!!!
int main(){
	Kernel::Load();
    cout<<"Network Topology"<<endl;
    Topology::print();
    tInicio = clock();
    //PSO::BestIndividual();
    //PSO::Initialize();
    AlgoritmoGenetico::InicializaAG();
    //AlgoritmoGenetico::TorneioAG();
    //AlgoritmoGenetico::GeraTodos();
	//Inicio da simulacao:
	//cout<<"Start Of Simulation:"<<endl<<endl;
	/*for(Kernel::laNet = Kernel::LaNetMin; Kernel::laNet <= Kernel::LaNetMax; Kernel::laNet += Kernel::LaPasso) {
        cout<<"New Simulation"<<endl;
        Kernel::InitialiseAll();
        cout<<"Simulating..."<<endl;
		Kernel::Simulate(0,0);
        Kernel::FinaliseAll();
        cout<<endl<<endl;
	}*/
    tFim = clock();
    tDecorrido = ((tFim - tInicio) / (CLOCKS_PER_SEC));
    InpOut::Result << tDecorrido << " segundos" <<endl;
    cout<<"Tempo de simulacao: "<<tDecorrido<<" segundos"<<endl;
    cout<<"Fim do programa";
	//Kernel::Simulation();
    cout<<"Fim do programa";
    cin.get(); cin.get();
    return 0;
}

