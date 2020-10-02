#include "Heuristics.h"



///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
long double Heuristics::calculateCostLink(bool* Disp, int L){ //L é a largura da requisição
	//return 1.0; //Dijkstra Normal
	/*
	long double alpha = 0.45;
	return exp( -alpha*Heuristics::calcNumFormAloc(L, Disp)/(Topology::getNumSlots()-L+1.0) );
	*/


	//Dijkstra que usa o número de formas para a requisição apenas:
	int numFormsL = Heuristics::calcNumFormAloc(L, Disp,Topology::getNumSlots());
	return (long double) 1.0/(numFormsL+1.0);

}

// ------------------------------------------------------------------------------ //
long double Heuristics::calculateCostRouting(int hops, bool* Disp, int L){ //L é a largura da requisicao
	//Dijkstra normal, que usa o número de hops apenas
	//return hops;
	//Dijkstra que usa o número de formas para a requisição apenas:
	int numFormsL = Heuristics::calcNumFormAloc(L, Disp,Topology::getNumSlots());
	return (long double) 1.0/(numFormsL+1.0);
	/*
	//Dijkstra que usa a ponderacao entre o numero de hops e o numero de formas:
	long double alpha = 0.9999;
	int numFormsL = Heuristics::calcNumFormAloc(L, Disp);
	return (long double) alpha*hops + (1.0-alpha)*((long double)1.0/(numFormsL+1.0));
	*/
	//
}

///////////////////////////////////////////////////////////////////////////////////
int Heuristics::calcNumFormAloc(int L, bool* Disp,int tam){ //L indica a largura da requisicao e Disp o vetor de disponibilidade
/*
	int sum = 0, si, se; //si eh o slot inicial da alocacao, que vai de 0 ate SE-L
	for(si = 0; si <= tam-L; si++){
		for(se = si; se < si+L; se++) //se checa se todos os slots de si ate si+L-1 estao disponiveis
			if(Disp[se] == false)
				break;
		if(se == si+L) // Os slots si,si+1,...,si+Lf-1 estao disponiveis
			sum++;
	}
	return sum;
}
*/
	int sum = 0, si; //si eh o slot inicial da alocacao, que vai de 0 ate SE-L
	int cont = 0;
	for(si = 0; si < tam; si++){
			if(Disp[si] == false)
            {
               if(cont >= L)
               {
                 sum += cont - L + 1;
                 //sum += floor (cont/L);
               }
                cont = 0;
            }
            else
            {
                cont++;
            }
	}
    if(cont >= L)
    {
        sum += cont - L + 1;
        //sum += floor (cont/L);
    }
	return sum;
}

// -------------------------------------------------------------------------- //
int Heuristics::calcNumAloc(int L, bool* Disp){
	int sum = 0, si = 0, se;
	while(si <= Topology::getNumSlots()-L){
		for(se = si; se < si+L; se++)
			if(Disp[se] == false){
				si = se+1;
				break;
			}
		if(se == si+L){ // Slots si,si+1,...,si+L-1 are all available
            sum++;
			si = se;
		}
	}
	return sum;
}

/////////////////////////////////////////////////////////////////////////////////
/*
//Atribui‹o de rota:
void Heuristics::RoutingMinHop(Assignment* assignment){
    vector<Route*> *routeSet = Topology::getRoutes(assignment->getOrN(), assignment->getDeN());
    //Missing:
    //Order routeSet by the number of hops
    //
    Heuristics::Routing(assignment);
}

// -------------------------------------------------------------------------- //
void Heuristics::RoutingMinLength(Assignment*  assignment){
    vector<Route*> *routeSet = Topology::getRoutes(assignment->getOrN(), assignment->getDeN());
    //Missing:
    //Order routeSet by the route length
    //
    Heuristics::Routing(assignment);
}
*/

// -------------------------------------------------------------------------- //
void Heuristics::Routing(Assignment* assignment){
    //assignment->clearTrialRoutes();
    //
    switch(Def::getRoutingType()) {
        case RoutingDJK :
            Routing::Dijkstra(assignment);
            break;
        case RoutingYEN :
            Routing::Yen(assignment);
            break;
        default: cout<<"Routing Not Defined"<<endl;
    }
}

/////////////////////////////////////////////////////////////////////////////////
void Heuristics::SpectrumAssignment(Assignment* assign,int ElementPosition,int vetor,int pos,int numSlots,int modulation){
    assign->setFirstSlot(-1);
    assign->setLastSlot(-1);
    switch(Def::getSpectrumAssignmentType()) {
        case SpectrumAssignmentFirstFitSlot :
            Heuristics::FirstFitSlot(assign);
            break;
        case SpectrumAssignmentFirstFitOrdenado :
            Heuristics::FirstFitOrdenado(assign,ElementPosition,vetor);
            break;
        case SpectrumAssignmentFirstFitCore:
            if(Topology::getCS())
                Heuristics::FirstFitCore(assign,Topology::getCS());
            else
                Heuristics::FirstFitCore(assign);
            break;
        case SpectrumAssignmentRandomCore:
            Heuristics::RandomCore(assign);
            break;
        case SpectrumAssignmentMSCL:
            Heuristics::MSCL(assign);
            break;
        case SpectrumAssignmentFirstFitAG:
            if(Topology::getCS())
                Heuristics::FirstFitAG_CS(assign,ElementPosition,vetor);
            else
                Heuristics::FirstFitAG(assign,ElementPosition,vetor);
            break;
        case SpectrumAssignmentPSO:
            Heuristics::PSO(assign,ElementPosition);
            break;
        case SpectrumAssignmentFirstFitAreas:
            Heuristics::FirstFitAreas(assign);
            break;
        case SpectrumAssignmentNucleoExaustivo:
            Heuristics::NucleoExaustivo(assign,ElementPosition);
            break;
        case SpectrumAssignmentPropostaFuji:
            if(Topology::getCS())
                Heuristics::PropostaFuji_CS(assign);
            else
                Heuristics::PropostaFuji(assign);
            break;
        case SpectrumAssignmentCPFFXT:
            //Heuristics::CorePrioritization(assign,pos);//FFCORE PRIORIZATION
            Heuristics::FirstFitCoreXT(assign,pos,numSlots,modulation);//FFXT
            break;
        case SpectrumAssignmentXTGreedy:
            Heuristics::FirstFitCoreXT(assign,pos,numSlots,modulation);
            break;
        case SpectrumAssignmentFirstFitXT:
            Heuristics::FirstFitXT(assign);
            break;
        default: cout<<"Routing Not Defined"<<endl;
    }
}

// -------------------------------------------------------------------------- //
//Atribui‹o de slots:
// -------------------------------------------------------------------------- //
void Heuristics::FirstFitOrdenado(Assignment*  assignment,int ElementPosition,bool vetor){
Route *route = assignment->getRoute();bool flag = false;
int numSlotsReq = assignment->getNumSlots();
int slot_range = Topology::getNumSlots() - numSlotsReq + 1,coreID, modulacao;
vector<unsigned int> ordem;


    if(numSlotsReq == 12){
        modulacao = 4;
        for(int a = 0;a < Topology::getNumCores();a++){
            if(a == 0){
                ordem.push_back(NULL);
                ordem[a] = 5;
            }
            if(a == 1){
                ordem.push_back(NULL);
                ordem[a] = 4;
            }
            if(a == 2){
                ordem.push_back(NULL);
                ordem[a] = 3;
            }
            if(a == 3){
                ordem.push_back(NULL);
                ordem[a] = 2;
            }
            if(a == 4){
                ordem.push_back(NULL);
                ordem[a] = 1;
            }
            if(a == 5){
                ordem.push_back(NULL);
                ordem[a] = 0;
            }
        }
    }
    if(numSlotsReq == 15){
        modulacao = 3;
        for(int a = 0;a < Topology::getNumCores();a++){
            if(a == 0){
                ordem.push_back(NULL);
                ordem[a] = 5;
            }
            if(a == 1){
                ordem.push_back(NULL);
                ordem[a] = 4;
            }
            if(a == 2){
                ordem.push_back(NULL);
                ordem[a] = 3;
            }
            if(a == 3){
                ordem.push_back(NULL);
                ordem[a] = 2;
            }
            if(a == 4){
                ordem.push_back(NULL);
                ordem[a] = 1;
            }
            if(a == 5){
                ordem.push_back(NULL);
                ordem[a] = 0;
            }
        }
    }
    if(numSlotsReq == 23){
        modulacao = 2;
        for(int a = 0;a < Topology::getNumCores();a++){
            if(a == 0){
                ordem.push_back(NULL);
                ordem[a] = 5;
            }
            if(a == 1){
                ordem.push_back(NULL);
                ordem[a] = 4;
            }
            if(a == 2){
                ordem.push_back(NULL);
                ordem[a] = 3;
            }
            if(a == 3){
                ordem.push_back(NULL);
                ordem[a] = 2;
            }
            if(a == 4){
                ordem.push_back(NULL);
                ordem[a] = 1;
            }
            if(a == 5){
                ordem.push_back(NULL);
                ordem[a] = 0;
            }
        }
    }

   ///////////////////////////////////////////////
   //NO CoreSwitch
   bool cap = false;
   if(!Topology::getCS())
        for(int c = 0;c < Topology::getNumCores();c++){
            coreID = ordem[c];
            for(int s = 0; s < slot_range;s++){
                if(Topology::checkSlotsCore(route, s,s + numSlotsReq - 1,coreID)){
                    if(Topology::checkSlotsCoreAG(route, s,s + numSlotsReq - 1,coreID)){
                        if(!cap)
                            cap = true;
                        //Verificar XT da possível alocação, caso interfere no limite de alguma alocação já feita,rejeita!
                        if(Modulation::getXT(assignment,s,coreID,route,numSlotsReq,modulacao)){
                            //Topology::numReqPerCore.at(coreID)++;
                            assignment->setCoreId(coreID);
                            assignment->setFirstSlot(s);
                            assignment->setNumSlots(numSlotsReq);
                            assignment->setLastSlot(s + numSlotsReq - 1);
                            flag = true;
                            break;
                        }
                    }
                }
            }
            if(flag){
                break;
            }
        }
    //CoreSwitch
    else
        for(int c = 0;c < Topology::getNumCores();c++){
            //coreID = ordem[c];
            for(int s = 0; s < slot_range;s++){
                if(Topology::checkSlotsCore_CS(route, s,s + numSlotsReq - 1,assignment,&ordem)){
                        assignment->setFirstSlot(s);
                        assignment->setLastSlot(s + numSlotsReq - 1);
                        //assignment->setCoreId(coreID);
                        flag = true;
                        break;
                }
            }
            if(flag)
                break;
        }
}
// -------------------------------------------------------------------------- //
void Heuristics::FirstFitAreas(Assignment*  assignment){
    Route *route = assignment->getRoute();bool flag = false;
    int numSlotsReq = assignment->getNumSlots();
    int area_comum = 130,area_1 = 45, area_2 = 100,area_3 = 45,area_inf,area_sup;
    int slot_range = Topology::getNumSlots() - numSlotsReq + 1;
    //Loop para percorrer as 4 áreas
    for(int i = 0;i < 4;i++){
        if(numSlotsReq == 3){
            //configuração da area priorizavel das requisições de tamanho 3
            if(i == 0){
                area_inf = 0;
                area_sup = area_1;
            }
            //configuração da area comum
            if(i == 1){
                area_inf = Topology::getNumSlots() - area_comum;
                area_sup = Topology::getNumSlots();
            }
            //tentativa de alocar na area nao priorizavel
            if(i == 2){
                area_inf = area_1;
                area_sup = area_1 + area_2;
            }
            //tentativa de alocar na area nao priorizavel
            if(i == 3){
                area_inf = area_1 + area_2;
                area_sup = area_1 + area_2 + area_3;
            }
        }
        else if(numSlotsReq == 4){
            //configuração da area priorizavel das requisições de tamanho 4
            if(i == 0){
                area_inf = area_1;
                area_sup = area_1 + area_2;
            }
            //configuração da area comum
            if(i == 1){
                area_inf = Topology::getNumSlots() - area_comum;
                area_sup = Topology::getNumSlots();
            }
            //tentativa de alocar na area nao priorizavel
            if(i == 2){
                area_inf = area_1 + area_2;
                area_sup = area_1 + area_2 + area_3;
            }
            //tentativa de alocar na area nao priorizavel
            if(i == 3){
                area_inf = 0;
                area_sup = area_1;
            }
        }
        else if(numSlotsReq == 5){
            //configuração da area priorizavel das requisições de tamanho 5
            if(i == 0){
                area_inf = area_1 + area_2;
                area_sup = area_1 + area_2 + area_3;
            }
            //configuração da area comum
            if(i == 1){
                area_inf = Topology::getNumSlots() - area_comum;
                area_sup = Topology::getNumSlots();
            }
            //tentativa de alocar na area nao priorizavel
            if(i == 2){
                area_inf = 0;
                area_sup = area_1;
            }
            //tentativa de alocar na area nao priorizavel
            if(i == 3){
                area_inf = area_1;
                area_sup = area_1 + area_2;
            }
        }
        for(int core = 0;core < Topology::getNumCores();core++){
                //LastFit para área comum com número par de slots requeridos
                if(i == 1 && numSlotsReq == 4){
                      for(int s = area_sup - 1; s >= area_inf + numSlotsReq - 1; s--){
                           if(Topology::checkSlotsCore(route, s,s - assignment->getNumSlots() + 1,core)){
                                    assignment->setFirstSlot(s);
                                    assignment->setLastSlot(s - assignment->getNumSlots() + 1);
                                    assignment->setCoreId(core);
                                    flag = true;
                                    break;
                           }
                      }
                }
                /////////////////////////////////////////////////////////////
                //FirsFit para todos os outros casos
                else{
                      for(int s = area_inf; s < area_sup - numSlotsReq + 1; s++){
                           if(Topology::checkSlotsCore(route, s,s + assignment->getNumSlots() - 1,core)){
                                    assignment->setFirstSlot(s);
                                    assignment->setLastSlot(s + assignment->getNumSlots() - 1);
                                    assignment->setCoreId(core);
                                    flag = true;
                                    break;
                           }
                      }
                }
                ///////////////////////////////////////////////////////////////
                if(flag)
                   break;
        }
        if(flag)
            break;
    }
}
// -------------------------------------------------------------------------- //
void Heuristics::PSO(Assignment* assignment, unsigned int position){
Route *route = assignment->getRoute();
unsigned int num_slots = assignment->getNumSlots(), num_cores = Topology::getNumCores();
unsigned int slot_range = Topology::getNumSlots() - num_slots + 1, hops = route->getNumHops(),coreIndex;
double costMin,costAux,minimumCost;
bool flag = false;
vector<unsigned int> possible_slots(0),finalCost(0);
vector<vector<unsigned int>> corePerHop(0);
    //Search for the possible allocations and fill the vector with start slot
    for(unsigned int s = 0; s < slot_range;s++){
         if(Topology::checkPossibleSet(route, s,s + num_slots - 1,assignment)){
                //save initial position of available set of slots
                possible_slots.push_back(s);
                flag = true;
         }
    }
    //There isn't any possible allocation - BLOCK
    if(!flag)
        return;
    //Calculate the minimum cost for each possible allocation and fill finalCost vector
    corePerHop.resize(possible_slots.size(),vector<unsigned int> (hops));
    for(unsigned p = 0;p < possible_slots.size();p++){
        minimumCost = Def::MAX_DOUBLE;
        for(unsigned h = 0;h < hops;h++){
            costMin   = Def::MAX_DOUBLE;
            coreIndex = Def::MAX_INT;
            for(unsigned int core = 0;core < num_cores;core++){
                //Check if a set of slots can be allocated in a specific core and hop
                if(Topology::checkCoreSet(route,possible_slots.at(p),possible_slots.at(p) + num_slots - 1,core,h)){
                        //Calculate the cost for a possible allocation for specific pso individual(position),core and hop
                        costAux = calculateCoreCost(core,position,route,h);
                        //Update the minimum cost and the specific index
                        if(costAux < costMin){
                            costMin   = costAux;
                            coreIndex = core;
                        }
                }
            }
            //Fill corePerHop vector with coreIndex with minimum cost
            //if(coreIndex == Def::MAX_INT)
            //    unsigned int t = 0;
            corePerHop.at(p).at(h) = coreIndex;
            if(costMin < minimumCost){
                minimumCost = costMin;
            }
        }
        //Fill finalCost vector with the minimum cost for the pssobile allocation among all hops
        finalCost.push_back(minimumCost);
    }
    /////////////////////////////////////////////////////
    //Find least cost possible allocation
    minimumCost               = Def::MAX_DOUBLE;
    unsigned int allocIndex   = Def::MAX_INT;
    for(unsigned int a = 0;a < possible_slots.size();a++){
        if(finalCost.at(a) < minimumCost){
            minimumCost = finalCost.at(a);
            allocIndex  = a;
        }
    }
    //////////////////////////////////////
    //Assign cores
    for(unsigned int h = 0;h < hops;h++)
            assignment->setCoreId(corePerHop.at(allocIndex).at(h),h);
    /////////////
    //Assign slots
    assignment->setFirstSlot(possible_slots.at(allocIndex));
    assignment->setLastSlot(possible_slots.at(allocIndex) + num_slots - 1);
    //////////////

}
// -------------------------------------------------------------------------- //
void Heuristics::RandomCore(Assignment*  assignment){

    Route *route = assignment->getRoute();
    int numSlotsReq = assignment->getNumSlots();
    int slot_range = Topology::getNumSlots() - numSlotsReq + 1,core_size = Topology::getNumCores();
    bool cs_matrix[core_size][slot_range];
    vector<int> cores;
    //Inicializa matriz de ocupação de cores e slots
    for(int i = 0;i < core_size;i++)
        for(int s = 0; s < slot_range ; s++){
                cs_matrix[i][s] = false;

        }
    ////////////////////////////////////////////////
    //Verificar todos slots de todos os nucleos
    for(int i = 0;i < core_size;i++)
        for(int s = 0; s < slot_range; s++)
            if(Topology::checkSlotsCore(route, s,s + assignment->getNumSlots() - 1,i))//caso conjunto de slots esteja disponivel o primeiro slot do conjunto recebe o valor 1, c.c recebe 0
             {
                 cs_matrix[i][s] = true;
             }
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //Preenche vetor com os nucleos disponiveis
    for(int i = 0;i < core_size;i++)
        for(int s = 0; s < slot_range; s++)
            if(cs_matrix[i][s]){
                cores.push_back(i);
                break;
            }
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //caso o vetor de cores possua algum elemento,a escolha randomica é feita
    if(cores.size() > 0){
            int c, s;
            do{
                c = rand() % core_size;s = rand() % slot_range ;
                bool b = cs_matrix[c][s];
                if(cs_matrix[c][s]){
                        assignment->setFirstSlot(s);
                        assignment->setLastSlot(s + assignment->getNumSlots() - 1);
                        assignment->setCoreId(c);
                        break;
                }

            }
            while(cs_matrix[c][s] == false);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
}
// -------------------------------------------------------------------------- //
void Heuristics::FirstFitAG(Assignment*  assignment,int ElementPosition,int vetor){
    Route *route = assignment->getRoute();bool flag = false;
    int numSlotsReq = assignment->getNumSlots(),modulacao;
    int slot_range = Topology::getNumSlots() - numSlotsReq + 1,coreID;
    vector<unsigned int> ordem;
    Individuo* individuo;
    ///////////////////////////////////////////////////////////
    if(vetor == 1)
            individuo = AlgoritmoGenetico::GetPopulationAuxElement(ElementPosition);
    if(vetor == 0)
            individuo = AlgoritmoGenetico::GetPopulationElement(ElementPosition);
    //
    if(numSlotsReq == 12){
        for(int a = 0;a < Topology::getNumCores();a++){
            ordem.push_back(NULL);
            ordem[a] = individuo->combinacoes.at(0).at(a);
        }
        modulacao = 4;
    }
    //
    if(numSlotsReq == 15){
        for(int a = 0;a < Topology::getNumCores();a++){
            ordem.push_back(NULL);
            ordem[a] = individuo->combinacoes.at(1).at(a);
        }
        modulacao = 3;
    }
    //
    if(numSlotsReq == 23){
        for(int a = 0;a < Topology::getNumCores();a++){
            ordem.push_back(NULL);
            ordem[a] = individuo->combinacoes.at(2).at(a);
        }
        modulacao = 2;
    }
    //////////////////////////////////////////////////////////////
    bool cap = false;
    for(int c = 0;c < Topology::getNumCores();c++){
        coreID = ordem[c];
        for(int s = 0; s < slot_range;s++){
            if(Topology::checkSlotsCoreAG(route, s,s + numSlotsReq - 1,coreID)){
                    if(!cap)
                        cap = true;
                    //Verificar XT da possível alocação, caso interfere no limite de alguma alocação já feita,rejeita!
                    if(Modulation::getXT(assignment,s,coreID,route,numSlotsReq,modulacao)){
                        //Topology::numReqPerCore.at(coreID)++;
                        assignment->setCoreId(coreID);
                        assignment->setFirstSlot(s);
                        assignment->setNumSlots(numSlotsReq);
                        assignment->setLastSlot(s + numSlotsReq - 1);
                        flag = true;
                        break;
                    }
                    else
                        int a =0;
            }
        }
        if(flag)
            break;
    }
    //Nao tem capacidade e no último formato de modulação
    if(!cap){
        //Requisição bloqueada
        Topology::resourceBloq++;
    }
    else{
        if(cap && !flag && modulacao == 2)
            Topology::xtBloq++;
        else{
            int y = 0;
        }
    }
}

void Heuristics::FirstFitXT(Assignment*  assignment){
    Route *route = assignment->getRoute();bool flag = false;
    int numSlotsReq = assignment->getNumSlots(),modulacao;
    int slot_range = Topology::getNumSlots() - numSlotsReq + 1,coreID;
    vector<unsigned int> ordem;
    Individuo* individuo;

    if(numSlotsReq == 2){
        for(int a = 0;a < Topology::getNumCores();a++){
            ordem.push_back(NULL);
            ordem[a] = a;
        }
        modulacao = 6;
    }
    if(numSlotsReq == 3){
        for(int a = 0;a < Topology::getNumCores();a++){
            ordem.push_back(NULL);
            ordem[a] = a;
        }
        modulacao = 4;
    }
    if(numSlotsReq == 4){
        for(int a = 0;a < Topology::getNumCores();a++){
            ordem.push_back(NULL);
            ordem[a] = a;
        }
        modulacao = 2;
    }
    //////////////////////////////////////////////////////////////
    bool cap = false;
    for(int c = 0;c < Topology::getNumCores();c++){
        coreID = ordem[c];
        for(int s = 0; s < slot_range;s++){
            if(Topology::checkSlotsCoreAG(route, s,s + numSlotsReq - 1,coreID)){
                    if(!cap)
                        cap = true;
                    //Verificar XT da possível alocação, caso interfere no limite de alguma alocação já feita,rejeita!
                    if(Modulation::getXT(assignment,s,coreID,route,numSlotsReq,modulacao)){
                        Topology::numReqPerCore.at(coreID)++;
                        assignment->setCoreId(coreID);
                        assignment->setFirstSlot(s);
                        assignment->setNumSlots(numSlotsReq);
                        assignment->setLastSlot(s + numSlotsReq - 1);
                        flag = true;
                        break;
                    }
            }
        }
        if(flag)
            break;
    }
    if(!flag){
        //Requisição bloqueada
        //Bloqueio por XT
        if(cap)
            Topology::xtBloq++;
        //Bloqueio por Recurso
        else
            Topology::resourceBloq++;
    }
}
// -------------------------------------------------------------------------- //
void Heuristics::FirstFitAG_CS(Assignment*  assignment,int ElementPosition,int vetor){
    Route *route = assignment->getRoute();
    int numSlotsReq = assignment->getNumSlots();
    int slot_range = Topology::getNumSlots() - numSlotsReq + 1,coreID;
    vector<unsigned int> ordem;
    Individuo* individuo;
    ///////////////////////////////////////////////////////////
    if(vetor == 1)
            individuo = AlgoritmoGenetico::GetPopulationAuxElement(ElementPosition);
    if(vetor == 0)
            individuo = AlgoritmoGenetico::GetPopulationElement(ElementPosition);

    if(numSlotsReq == 3)
        for(int a = 0;a < Topology::getNumCores();a++){
            ordem.push_back(NULL);
            ordem[a] = individuo->combinacoes.at(0).at(a);
        }
    if(numSlotsReq == 4)
        for(int a = 0;a < Topology::getNumCores();a++){
            ordem.push_back(NULL);
            ordem[a] = individuo->combinacoes.at(1).at(a);
        }
    if(numSlotsReq == 5)
        for(int a = 0;a < Topology::getNumCores();a++){
            ordem.push_back(NULL);
            ordem[a] = individuo->combinacoes.at(2).at(a);
        }
    //////////////////////////////////////////////////////////////
    for(int s = 0; s < slot_range;s++){
       if(Topology::checkSlotsCore_CS(route, s,s + numSlotsReq - 1,assignment,&ordem)){
             assignment->setFirstSlot(s);
             assignment->setLastSlot(s + numSlotsReq - 1);
             break;
       }
    }
}
// -------------------------------------------------------------------------- //
void Heuristics::NucleoExaustivo(Assignment*  assignment,int pos){
Route *route = assignment->getRoute();bool flag = false;
int numSlotsReq = assignment->getNumSlots();
int slot_range = Topology::getNumSlots() - numSlotsReq + 1,coreID;
int ordem[Topology::getNumCores()];
Individuo* individuo = AlgoritmoGenetico::GetIndividuo(pos);
    ///////////////////////////////////////////////////////////
    if(numSlotsReq == 2)
        for(int a = 0;a < Topology::getNumCores();a++){
            ordem[a] = individuo->combinacoes.at(0).at(a);
        }
    if(numSlotsReq == 3)
        for(int a = 0;a < Topology::getNumCores();a++){
            ordem[a] = individuo->combinacoes.at(1).at(a);
        }
    if(numSlotsReq == 4)
        for(int a = 0;a < Topology::getNumCores();a++){
            ordem[a] = individuo->combinacoes.at(2).at(a);
        }
    if(numSlotsReq == 5)
        for(int a = 0;a < Topology::getNumCores();a++){
            ordem[a] = individuo->combinacoes.at(3).at(a);
        }
    if(numSlotsReq == 6)
        for(int a = 0;a < Topology::getNumCores();a++){
            ordem[a] = individuo->combinacoes.at(4).at(a);
        }
    if(numSlotsReq == 9)
        for(int a = 0;a < Topology::getNumCores();a++){
            ordem[a] = individuo->combinacoes.at(5).at(a);
        }
    if(numSlotsReq == 10)
        for(int a = 0;a < Topology::getNumCores();a++){
            ordem[a] = individuo->combinacoes.at(6).at(a);
        }

    //////////////////////////////////////////////////////////////
    for(int c = 0;c < Topology::getNumCores();c++){
        coreID = ordem[c];
        for(int s = 0; s < slot_range;s++){
            if(Topology::checkSlotsCoreAG(route, s,s + numSlotsReq - 1,coreID)){
                    assignment->setFirstSlot(s);
                    assignment->setLastSlot(s + numSlotsReq - 1);
                    assignment->setCoreId(coreID);
                    flag = true;
                    break;
            }
        }
        if(flag)
            break;
    }


}

void Heuristics::MSCL(Assignment* assignment){
    int numSlotsReq = assignment->getNumSlots();
    Route *route = assignment->getRoute(),*route_aux;
	int s,x,NslotsUsed,core,numNodes = Topology::getNumNodes();
	int NslotsReq = assignment->getNumSlots(),slot_range = Topology::getNumSlots() - NslotsReq + 1;
    int core_size = Topology::getNumCores(),orNode = route->getOrN(),desNode = route->getDeN();
    //Ponteiro que receberá todas as rotas que interferem com route
    vector<Route*> RouteInt = Topology::AllInterRoutes.at(orNode*numNodes + desNode);
    int a = RouteInt.size();
    int vetCapInic;
	int vetCapFin;
	double perda, perdaMin = Def::MAX_DOUBLE;
    int si;
    //Vetor de disponibilidade das rotas que interferem
    bool vetDispInt[Topology::getNumSlots()],vetDispFin[Topology::getNumSlots()];
    //Route* q = RouteInt.at(0);
    //q->print();
    //Route* p = RouteInt.at(1);
    //p->print();
    //route->print();

    //Obtem quais slots podem começar a requisição:
    bool DispFitSi = false;
		//Percorre os núcleos que possuem disponibilidade para alocar a requisição
        for(int e = 0;e < core_size;e++){
            //Verifica quais conjuntos de slots podem ser alocados
            for(s = 0; s < slot_range; s++){
                perda = 0.0;
                DispFitSi = Topology::checkSlotsCore(route, s,s + NslotsReq - 1,e);
                if(DispFitSi == true){
                        //Percorre rotas interferentes
						for(unsigned int r = 0; r <= RouteInt.size(); r++){
                            if(r == RouteInt.size())
                                route_aux = route;
                            else
                                route_aux = RouteInt.at(r);
                            for(int se = 0;se < Topology::getNumSlots();se++){
								if(!(Topology::checkSlotsCore(route_aux,se,se,e))){
									vetDispInt[se] = false;
									vetDispFin[se] = false;
								}
								else{
									vetDispInt[se] = true;
									if(se >= s && se < s + NslotsReq)
                                        vetDispFin[se] = false;
                                    else
                                        vetDispFin[se] = true;
								}
							}
							vetCapInic = 0;
							vetCapFin  = 0;
							//Calcula a capacidade inicial baseada no número de formas de alocação,requisições->2,3,4,9,10
							for(int b=2; b <= 10; b++){
                                  if(b == 2 || b == 3 || b == 4 || b == 9 || b == 10)
                                    vetCapInic += Heuristics::calcNumFormAloc(b,vetDispInt,Topology::getNumSlots());
							}
							//Requisições->2,3,4,9,10
							for(int d=2; d <= 10; d++){
								  if(d == 2 || d == 3 || d == 4 || d == 9 || d == 10)
                                    vetCapFin += Heuristics::calcNumFormAloc(d,vetDispFin,Topology::getNumSlots());
							}
							//Requisições->2,3,4,5 e 6
                            /*
							for(int b=2; b <= 5; b = b + 1){
								  vetCapInic += Heuristics::calcNumFormAloc(b,vetDispInt,Topology::getNumSlots());
							}
							for(int d=2; d <= 5; d = d + 1){
								 vetCapFin += Heuristics::calcNumFormAloc(d,vetDispFin,Topology::getNumSlots());
							}*/
							perda += vetCapInic - vetCapFin;
						}//Fim da verificação das rotas interferentes
						if(perda < perdaMin){
								 perdaMin = perda;
								 core = e;
								 si = s;
						}
				}
            } //Fim da verificação dos slots
        }//Fim da verificação dos núcleos
        if(perdaMin < Def::MAX_DOUBLE){
            assignment->setFirstSlot(si);
            assignment->setLastSlot(si+NslotsReq-1);
            assignment->setCoreId(core);
            if(numSlotsReq == 2){
                Def::numReq_Acc_2slots.at(core)++;
            }
            if(numSlotsReq == 4){
                Def::numReq_Acc_4slots.at(core)++;
            }
            if(numSlotsReq == 8){
                Def::numReq_Acc_8slots.at(core)++;
            }
        }
}

void Heuristics::PropostaFuji(Assignment* assignment){
    int ordem[3], coreID, num_slots = assignment->getNumSlots();
    int slot_range = Topology::getNumSlots() - num_slots + 1;
    Route *route = assignment->getRoute();
    bool flag = false;
    //Configuração dos núcleos para requisições-2 para cada tipo e um núcleo comum
    if(num_slots == 3)
        for(int a = 0;a < 2;a++)
            ordem[a] = a;
    if(num_slots == 4)
        for(int a = 0;a < 2;a++)
            ordem[a] = a + 2;
    if(num_slots == 5)
        for(int a = 0;a < 2;a++)
            ordem[a] = a + 4;
    ordem[2] = 6;//núcleo comum
    //////////////////////////////////////////////////////////////////////////////
    for(int c = 0;c < 3;c++){
        coreID = ordem[c];
        for(int s = 0; s < slot_range;s++){
            if(Topology::checkSlotsCore(route, s,s + assignment->getNumSlots() - 1,coreID)){
                 assignment->setFirstSlot(s);
                 assignment->setLastSlot(s + num_slots - 1);
                 assignment->setCoreId(coreID);
                 flag = true;
                 break;
            }
        }
        if(flag)
            break;
    }
}

void Heuristics::PropostaFuji_CS(Assignment* assignment){
Route *route = assignment->getRoute();
int ordem[Topology::getNumCores()], coreID, num_slots = assignment->getNumSlots(), num_cores = Topology::getNumCores(),core;
int slot_range = Topology::getNumSlots() - num_slots + 1, hops = route->getNumHops(),minimum = Def::MAX_INT,index;
unsigned int greater_score = 0;
bool flag = false;
vector<unsigned int> possible_slots(0),final_score(0);
vector<vector<unsigned int>> scores(0);
    //Configuração dos núcleos para requisições-2 para cada tipo e um núcleo comum
    if(num_slots == 2){
        ordem[0] = 0;
        ordem[1] = 1;
        ordem[2] = 4;
        ordem[3] = 3;
        ordem[4] = 6;
        ordem[5] = 2;
        ordem[6] = 5;
    }
    if(num_slots == 4){//One more core for 4 slots
        ordem[0] = 2;
        ordem[1] = 3;
        ordem[2] = 4;
        ordem[3] = 1;
        ordem[4] = 6;
        ordem[5] = 0;
        ordem[6] = 5;
    }
    if(num_slots == 8){
        ordem[0] = 5;
        ordem[1] = 6;
        ordem[2] = 4;
        ordem[3] = 3;
        ordem[4] = 1;
        ordem[5] = 2;
        ordem[6] = 0;
    }
    /////////////////////////////////////////////////////////////////////////////
    //Search for the possible allocations and fill the vector with start slot
    for(unsigned int s = 0; s < slot_range;s++){
         if(Topology::checkPossibleSet(route, s,s + num_slots - 1,assignment)){
                //save initial position of available set of slots
                possible_slots.push_back(s);
                flag = true;
         }
    }
    //There isn't any possible allocation - BLOCK
    if(!flag)
        return;
    /////////////////////////////////////////////////////////////////////////
    //Calculate scores
    scores.resize(possible_slots.size(),vector<unsigned int> (hops));
    for(unsigned p = 0;p < possible_slots.size();p++){
        for(unsigned h = 0;h < hops;h++){
            unsigned int c = 0;
            for(unsigned int core = 0;core < num_cores;core++){
                c = ordem[core];
                if(Topology::checkCoreSet(route,possible_slots.at(p),possible_slots.at(p) + num_slots - 1,c,h)){
                    //One more core
                    if(num_slots == 4){
                        if(c == ordem[0] || c == ordem[1] || c == ordem[2])
                            scores.at(p).at(h) = c;
                        else
                            scores.at(p).at(h) = 2*num_cores - c;
                        if(greater_score < scores.at(p).at(h))
                            greater_score = scores.at(p).at(h);
                    }
                    //Just 2 prioritized cores for 3 and 5 slots requests
                    else{
                        if(c == ordem[0] || c == ordem[1])
                            scores.at(p).at(h) = c;
                        else
                            scores.at(p).at(h) = 2*num_cores - c;
                        if(greater_score < scores.at(p).at(h))
                            greater_score = scores.at(p).at(h);
                    }
                    break;
                }
           }
        }
        //Choose the maximum score among all links for the possible allocation
        final_score.push_back(greater_score);
        greater_score = 0;
    }
    //////////////////
    //Search in final_score the minimum value to choose the set of slots,if there is a draw the minimum index is choosen
    for(unsigned int f = 0;f < final_score.size();f++)
        if(minimum > final_score.at(f)){
            minimum = final_score.at(f);
            index = f;
        }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Choose core
    for(unsigned int h = 0;h < hops;h++)
        for(unsigned int c = 0;c < num_cores;c++){
            if(Topology::checkCoreSet(route,possible_slots.at(index),possible_slots.at(index) + num_slots - 1,ordem[c],h)){
                     assignment->setCoreId(ordem[c],h);
                     break;
            }
        }
    /////////////
    //Assign slots
    assignment->setFirstSlot(possible_slots.at(index));
    assignment->setLastSlot(possible_slots.at(index) + num_slots - 1);
    //////////////
}

void Heuristics::FirstFitCore(Assignment*  assignment){
    Route *route = assignment->getRoute();bool flag = false;
    int numSlotsReq = assignment->getNumSlots();
    int slot_range = Topology::getNumSlots() - numSlotsReq + 1;

    for(int core = 0;core < Topology::getNumCores();core++){
        for(int s = 0; s < slot_range;s++){
            if(Topology::checkSlotsCore(route, s,s + assignment->getNumSlots() - 1,core)){
                    assignment->setFirstSlot(s);
                    assignment->setLastSlot(s + assignment->getNumSlots() - 1);
                    assignment->setCoreId(core);
                    flag = true;
                    break;
            }
        }
        if(flag)
            break;
    }
}
//Função que preenche todas as possibilidades de alocação,caso não haja capacidade retorna FALSE
void Heuristics::FirstFitCoreXT(Assignment*  assignment,int pos,int mod,int numSlots){
    Route *route = assignment->getTrialRoute(pos);bool flag = false;
    int numSlotsReq = numSlots;//assignment->getNumSlotsVec(pos);
    int slot_range = Topology::getNumSlots() - numSlotsReq;//Banda de guarda
    //Primeiro slots
    for(int core = 0;core < Topology::getNumCores();core++){
        for(int s = 0; s <= slot_range;s++){
            //CONSIDERANDO BANDA DE GUARDA
            if(Topology::checkSlotsCore(route, s,s + numSlotsReq - 1,core)){
               //Pelo menos uma alocação é possível
               assignment->addTempSlot(pos,mod,s);
               assignment->addTempCore(pos,mod,core);
               //Cria espaço em xtVec para o XT da possível alocação
               assignment->addXTVec(pos,mod);
               assignment->addXTMed(pos,mod);
            }
        }
    }
}
//---------------------------------------------------------------------------//
void Heuristics::CorePrioritization(Assignment*  assignment,int pos){
/*Route *route = assignment->getTrialRoute(pos);bool flag = false;
int numSlotsReq = assignment->getNumSlotsVec(pos);
int slot_range = Topology::getNumSlots() - numSlotsReq;//Banda de guarda
//Ordem adotada para evitar XT
vector<unsigned int> ordem;
    for(unsigned int o = 0;o < Topology::getNumCores();o++){
        if(o == 0){
            ordem.push_back(0);
        }
        if(o == 1){
            ordem.push_back(2);
        }
        if(o == 0){
            ordem.push_back(4);
        }
        if(o == 0){
            ordem.push_back(5);
        }
        if(o == 0){
            ordem.push_back(1);
        }
        if(o == 0){
            ordem.push_back(3);
        }
        if(o == 0){
            ordem.push_back(6);
        }
    }
    for(int core = 0;core < Topology::getNumCores();core++){
        for(int s = 0; s < slot_range;s++){
            if(Topology::checkSlotsCore(route, s,s + assignment->getNumSlotsVec(pos),ordem[core])){//Considerando banda de guarda
               //Pelo menos uma alocação é possível
               assignment->addTempSlot(pos,s);
               assignment->addTempCore(pos,ordem[core]);
               //Cria espaço em xtVec para o XT da possível alocação
               assignment->addXTVec(pos);
               assignment->addXTMed(pos);
            }
            else{
                //Pelo menos uma alocação é possível
               assignment->addTempSlot(pos,Def::MAX_INT);
               assignment->addTempCore(pos,Def::MAX_INT);
               //Cria espaço em xtVec para o XT da possível alocação
               assignment->addXTVec(pos);
               assignment->addXTMed(pos);
            }
        }
    }*/
}
//---------------------------------------------------------------------------//
void Heuristics::FirstFitCore(Assignment*  assignment, bool cs){
    Route *route = assignment->getRoute();
    bool flag = false;
    int numSlotsReq = assignment->getNumSlots(), slot_range = Topology::getNumSlots() - numSlotsReq + 1;

    for(int core = 0;core < Topology::getNumCores();core++){
        for(int s = 0; s < slot_range;s++){
            if(Topology::checkSlotsCore_CS(route, s,s + numSlotsReq - 1,assignment,core)){
                    assignment->setFirstSlot(s);
                    assignment->setLastSlot(s + numSlotsReq - 1);
                    flag = true;
                    break;
            }
        }
        if(flag)
           break;
    }
}
//---------------------------------------------------------------------------//
double Heuristics::calculateCoreCost(unsigned int core,unsigned int position,Route* route,unsigned int hop){
PSO_Individual* ind = PSO::getPopulationElement(position);
float cost = 0;
unsigned int num_slots = Topology::getNumSlots(),orNode,deNode;
orNode                 = route->getNode(hop),deNode = route->getNode(hop + 1);
Fiber* fiber           = Topology::getFiber(orNode,deNode);
Core* coreObj                = fiber->getFiberCore(core);
float num_req_3_slots = (coreObj->num_req_3slots*3/num_slots + 1);//Get num req 3 slots in h hop * 3 / total slots
float num_req_4_slots = (coreObj->num_req_4slots*4/num_slots + 1);//Get num req 4 slots in h hop * 4 / total slots
float num_req_5_slots = (coreObj->num_req_5slots*5/num_slots + 1);//Get num req 5 slots in h hop * 5 / total slots
    //Power series in -2 to +2 range!
    for(int a = 0;a < PSO::getD1();a++)
        for(int b = 0;b < PSO::getD2();b++)
            for(int c = 0;c < PSO::getD3();c++)
                    cost += ind->getCurrentPosition(a,b,c)*pow(num_req_3_slots,(a - 2))*pow(num_req_4_slots,(b - 2))*pow(num_req_5_slots,(c - 2));

return cost;
}
//---------------------------------------------------------------------------//
/*
void Heuristics::MSCL(Assignment* assignment){

    Route *route = assignment->getRoute();
    Route *route_aux;
	int s,x,NslotsUsed,core,numNodes = Topology::getNumNodes();
	int NslotsReq = assignment->getNumSlots(),slot_range = Topology::getNumSlots() - NslotsReq + 1;
    int core_size = Topology::getNumCores(),orNode = route->getOrN(),desNode = route->getDeN();

    //Ponteiro que receberá todas as rotas que interferem com route
    vector<Route*> RouteInt = Topology::AllInterRoutes.at(orNode*numNodes + desNode);
    //vector<int> cores;
    int a = RouteInt.size();
    // Calculando a perda de capacidade para cada possibilidade de alocação
	//int vetCapInic[RouteInt.size()];
	//int vetCapFin[RouteInt.size()];
    int vetCapInic;
	int vetCapFin;
    int LowBound,HighBound;
    bool flag;

	double perda, perdaMin = Def::MAX_DOUBLE;
    int si;
    //Vetor de disponibilidade das rotas que interferem
    bool vetDispInt[Topology::getNumSlots()],vetDispFin[Topology::getNumSlots()];
    //Obtem quais slots podem começar a requisição:
    bool DispFitSi = false;
		//Percorre os núcleos que possuem disponibilidade para alocar a requisição
        for(int e = 0;e < core_size;e++){
            //Verifica quais conjuntos de slots podem ser alocados
            for(s = 0; s < slot_range; s++){
                perda = 0.0;
                DispFitSi = Topology::checkSlotsCore(route, s,s + NslotsReq - 1,assignment,e);
                if(DispFitSi == true){
                        //Percorre rotas interferentes -
						for(unsigned int r = 0; r <= RouteInt.size(); r++){
                            flag = false;LowBound = 0;HighBound = 0;
                            if(r == RouteInt.size())
                                route_aux = route;
                            else
                                route_aux = RouteInt.at(r);
                            //Inicia varredura do vetor disponibilidade em busca do limitante superior(primeiro zero)
                            for(int slot = s ;slot < Topology::getNumSlots();slot++){
                                //Caso "slot" esteja entre o slot inicial e final da requisição
                                if(slot >= s && slot <= s + NslotsReq - 1){
                                    //Caso "slot" esteja no slot incial da requisição e não esteja disponível,limitante inferior encontrado!
                                    if(slot == s && !(Topology::checkSlotsCore(route_aux,slot,slot,assignment,e))){
                                        vetDispInt[slot] = false;
                                        vetDispFin[slot] = false;
                                        LowBound = slot;
                                        flag = true;
                                    }
                                    //Caso "slot" esteja no slot final da requisição e não esteja disponível,limitante superior encontrado!
                                    if(slot == s + NslotsReq - 1 && !(Topology::checkSlotsCore(route_aux,slot,slot,assignment,e))){
                                        HighBound = slot;
                                        vetDispInt[slot] = false;
                                        vetDispFin[slot] = false;
                                        break;
                                    }
                                    //Caso "slot" esteja no slot final da requisição,esteja disponível e no slot final do núcleo,limite superior encontrado!
                                    if(slot == s + NslotsReq - 1 && (Topology::checkSlotsCore(route_aux,slot,slot,assignment,e)) && slot == Topology::getNumSlots() - 1){
                                        HighBound = slot;
                                        vetDispInt[slot] = true;
                                        vetDispFin[slot] = false;
                                        break;
                                    }
                                    //Caso "slot" esteja entre o slot final e o inicial da requisição e esteja disponível!
                                    if((Topology::checkSlotsCore(route_aux,slot,slot,assignment,e))){
                                        vetDispInt[slot] = true;
                                        vetDispFin[slot] = false;
                                        //Limite inferior encontrado
                                        if(slot == 0){
                                            LowBound = slot;
                                            flag = true;
                                        }
                                    }
                                    //Caso "slot" esteja entre o slot final e o inicial da requisição e não esteja disponível!
                                    else{
                                        vetDispFin[slot] = false;
                                        vetDispInt[slot] = false;
                                    }
                                }
                                //Caso "slot" esteja após o slot final da requisição
                                else{
                                    //Caso "slot" esteja no slot final e disponível,limite superior encontrado!
                                    if(slot == Topology::getNumSlots() - 1 && Topology::checkSlotsCore(route_aux,slot,slot,assignment,e)){
                                        HighBound = slot;
                                        vetDispInt[slot] = true;
                                        vetDispFin[slot] = true;
                                        break;
                                    }
                                    //Caso "slot" esteja no slot final e não disponível,limite superior encontrado!
                                    if(slot == Topology::getNumSlots() - 1 && !Topology::checkSlotsCore(route_aux,slot,slot,assignment,e)){
                                        HighBound = slot - 1;
                                        break;
                                    }
                                    //Caso "slot" não esteja no final e esteja disponível
                                    if((Topology::checkSlotsCore(route_aux,slot,slot,assignment,e))){
                                        vetDispInt[slot] = true;
                                        vetDispFin[slot] = true;

                                    }
                                    //Caso "slot" não esteja no final e não esteja disponível,limite superior encontrado!
                                    else{
                                        HighBound = slot - 1;
                                        break;
                                    }
                                }
                            }
                            //Varredura nos slots anteriores ao slot inical, para encontrar o limite inferior(zero)
                            for(int se = s - 1;se >= -1;se--){
                                //Caso o limite inferior tenha sido encontrado anteriormente
                                if(flag == true)
                                    break;
                                //Caso "se" esteja disponível
                                if((Topology::checkSlotsCore(route_aux,se,se,assignment,e))){
                                    vetDispInt[se] = true;
                                    vetDispFin[se] = true;
                                    //limite inferior encontrado
                                    if(se == 0){
                                        LowBound = 0;
                                        break;
                                    }
                                }
                                //Caso "se" não esteja disponível, limite inferior encontrado
                                else{
                                    LowBound = se + 1;
                                    break;
                                }
                            }
                            bool vetDispIntRed[HighBound - LowBound + 1],vetDispFinRed[HighBound - LowBound + 1];
                            for (int l = 0;l <= HighBound - LowBound;l++){
                                vetDispIntRed[l] = vetDispInt[LowBound + l];
                                vetDispFinRed[l] = vetDispFin[LowBound + l];
                            }
							vetCapInic = 0;
							vetCapFin  = 0;
							//if(route->getOrN()==6 && route->getDeN()==1)
                            //    int y = 1;
							//Calcula a capacidade inicial baseada no número de formas de alocação,requisições->2,4,8
							for(int b=2; b <= 8; b+=b){
								  vetCapInic += Heuristics::calcNumFormAloc(b,vetDispIntRed,sizeof(vetDispIntRed));
							}
							//Requisições->2,4,8
							for(int d=2; d <= 8; d+=d)
								 vetCapFin += Heuristics::calcNumFormAloc(d,vetDispFinRed,sizeof(vetDispFinRed));
							perda += vetCapInic - vetCapFin;
							bool z,q;
							if(perda<0){
                                 for(int v = s;v<=s+NslotsReq-1;v++){
                                    z = vetDispInt[v];
                                    q = vetDispFin[v];
                                 }
                            }
						}//Fim da verificação das rotas interferentes
						if(perda < perdaMin){
								 perdaMin = perda;
								 core = e;
								 si = s;
						}
				}
            } //Fim da verificação dos slots
        }//Fim da verificação dos núcleos
        if(perdaMin < Def::MAX_DOUBLE){
            assignment->setFirstSlot(si);
            assignment->setLastSlot(si+NslotsReq-1);
            assignment->setCoreId(core);
		}
}
*/
//---------------------------------------------------------------------------//

void Heuristics::FirstFitSlot(Assignment*  assignment){
    Route *route = assignment->getRoute();
    int numSlotsReq = assignment->getNumSlots();
    int slot_range = Topology::getNumSlots() - numSlotsReq + 1;
    bool flag = false;
    //Percorre os slots
    for(int s = 0; s < slot_range ; s++){
        //Percorre os nucleos
        for(int c = 0;c < Topology::getNumCores();c++){
            if(Topology::checkSlotsCore(route, s,s + assignment->getNumSlots() - 1,c)){
                assignment->setFirstSlot(s);
                assignment->setLastSlot(s + numSlotsReq - 1);
                assignment->setCoreId(c);
                flag = true;
                break;
            }
        }
        if(flag)
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void Heuristics::ExpandConnection(Connection *con){
    //Expand an edge slot according to the following policy:
    ExpandRandomly(con); ////Remove o slot da direita ou da esquerda com igual probabilidade.
}

// -------------------------------------------------------------------------- //
void Heuristics::ExpandRandomly(Connection *con){ //Remove aleatoriamente o slot da direita ou da esquerda.
    if(rand()%2 == 0)
        con->expandLeft(); //Expand to the left
    else
        con->expandRight(); //Expand to the rigth;
}

// *************************************************************************** //
void Heuristics::CompressConnection(Connection *con){
    //Compress an edge slot according to the following policy:
    CompressRandomly(con); //Remove o slot da direita ou da esquerda com igual probabilidade.
}


// -------------------------------------------------------------------------- //
void Heuristics::CompressRandomly(Connection *con){ //Remove aleatoriamente o slot da direita ou da esquerda.
    if(rand()%2 == 0) //Compress to the left
        con->compressLeft();
    else
        con->compressRight(); //Compress to the rigth;
}



