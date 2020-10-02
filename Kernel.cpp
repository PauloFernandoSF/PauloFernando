#include "Kernel.h"



bool ExpComp;
int Alg_Routing;
int Alg_Aloc; // Informa o algortimo de Roteamento e de Alocação de espectro;
long double Kernel::mu, Kernel::laNet, Kernel::laE, Kernel::muC; //laE = Taxa de expansão; muC = Taxa de compressão
long double Kernel::LaNetMin, Kernel::LaNetMax, Kernel::LaPasso;
int totalConDesc;
int Kernel::numSim = 0;

void Kernel::Load(){
	int Npontos;
	int aux, numReg, numCores;
	//Adquire o numero de nos:
	InpOut::topology>>aux;
    Topology::setNumNodes(aux);
	cout<<"Number of Nodes: "<<Topology::getNumNodes()<<endl;
    Node *node;
    //CoreSwitch
    InpOut::topology>>aux;
    Topology::setCS(aux);
    if(aux)
        cout<<"Core Switch: YES"<<endl;
    else
        cout<<"Core Switch: NO"<<endl;
    //Inserir nós com regenerador
    for(int i = 0; i < Topology::getNumNodes(); i++){
        InpOut::topology>>numReg;
        node = new Node(i, numReg);
        Topology::insertNode(node);
    }
    //Adquire o número de fibras e as insere
    InpOut::topology>>aux;
    Topology::setNumFibers(aux);
	cout<<"Number of Fibers: "<<Topology::getNumFibers()<<endl;
    InpOut::topology>>aux;
    Topology::setNumCores(aux);
    cout<<"Number of Cores/Fiber: "<<Topology::getNumCores()<<endl;
    //Adquire o numero de enlaces
	InpOut::topology>>aux;
    Topology::setNumLinks(aux);
    cout<<"Number of Links: "<<Topology::getNumLinks()<<endl;
    //Adquire o numero de slots por enlace
    InpOut::topology>>aux;
    Topology::setNumSlots(aux);
	cout<<"Number of Slots per Link: "<<Topology::getNumSlots()<<endl;
    //Adquire todos os enlaces da rede
    int orNode, deNode; double length; int nSec; Link *link;
    Fiber *fiber;
    for(aux = 0; aux < Topology::getNumLinks(); aux++){
        vector<Core*> core;
        InpOut::topology>>orNode;
        InpOut::topology>>deNode;
        InpOut::topology>>length;
        InpOut::topology>>nSec;
        link = new Link(orNode, deNode, length, nSec);
        Topology::insertLink(link);
        for(int y = 0; y < Topology::getNumCores();y++){
            core.push_back(NULL);
            core.at(y) = new Core(y,Topology::getNumSlots());

        }
        //Seta os núcleos adjacentes
        for(int y = 0; y < Topology::getNumCores();y++){
            core.at(y)->setAdjCores(Topology::getNumCores());

        }
        fiber = new Fiber(aux,core,orNode,deNode);
        Topology::insertFiber(fiber);
    }
    cout<<"Request Traffic Intensities: ";
    InpOut::traffic>>aux;  //Nmero de possibilidades de requisio em bps
    double rate_bps;
    for(int i = 0; i < aux; i++){
        InpOut::traffic>>rate_bps; //Taxa em bps
        Traffic::addTraffic(rate_bps);
        cout<<rate_bps<<"  ";
    }
    cout<<endl;
    //////Routing:
	cout<<"* Routing:  "<<RoutingDJK<<" - DJK    "<<RoutingYEN<<" - YEN "<< endl;
	cout<<"Insert Desired Routing Algorithm: ";
	cin >> Alg_Routing;
    Def::setRoutingType((enum RoutingType) Alg_Routing);

    cout<<"* Spectrum Assignment:  "<<SpectrumAssignmentFirstFitSlot<<" - FirstFitSlot \t"<<SpectrumAssignmentFirstFitOrdenado<<" - FirstFitOrdenado \t"<<SpectrumAssignmentFirstFitCore<<" - FirstFitCore \t"
    <<SpectrumAssignmentRandomCore<<" - RandomCore \t"<<SpectrumAssignmentMSCL<<" - MSCL \t"<<SpectrumAssignmentFirstFitAG<<" - FirstFitAG \t"<<SpectrumAssignmentFirstFitAreas<<" - FirstFitAreas \t"
    <<SpectrumAssignmentNucleoExaustivo<<" - NucleoExaustivo \t"<<SpectrumAssignmentPropostaFuji<<" - PropostaFuji \t"<<SpectrumAssignmentPSO<<" - PSO \t"<<SpectrumAssignmentCPFFXT<<" - CP-FF-XT \t"
    <<SpectrumAssignmentXTGreedy<<" - XTGreedy \t"<<SpectrumAssignmentFirstFitXT<<" - FFCXT \t"<<endl;
	cout<<"Insert Desired Spectrum Assignment: ";
	cin>> Alg_Aloc;
    Def::setSpectrumAssignmentType((enum SpectrumAssignmentType) Alg_Aloc);

    //Intensidade do trfego:
    cout<<"*Traffic Parameters:"<<endl;
	cout<<"Insert Connection Deactivation Rate: ";  cin>> Kernel::mu; //mu = taxa de desativacao das conexoes;
	cout<<"LaNet Min= "; cin >> LaNetMin; // La = Connection Arrival Rate;
    cout<<"LaNet Max= "; cin >> LaNetMax;
	cout<<"#Points in the Graph= "; cin >> Npontos;
	LaPasso = (LaNetMax-LaNetMin)/(Npontos-1);
	long double nReq;
	cout<<"Insert Number of Requests: ";  cin>> nReq;
	Def::setNumReqMax(nReq);
	Topology::setTotalSlots();
	if(Def::getRoutingType() == 1 && Def::getSpectrumAssignmentType() == 5){
        Routing::Dijkstra();
        Topology::setInterRoutes();
	}
	else if(Def::getRoutingType() == 2){
        Routing::Yen(1);//Define a quantidade de rotas!
        //Vetor para verificação da quantidade de requisições alocadas em cada rota
        for(int r = 0;r < Routing::KYEN;r++){
            Topology::numReqPerRoute.push_back(0);
        }
    }
    else
        Routing::Dijkstra();
}

long double Kernel::Simulation(int ElementPosition,int vetor){
    long double pb;
    for(Kernel::laNet = Kernel::LaNetMin; Kernel::laNet <= Kernel::LaNetMax; Kernel::laNet += Kernel::LaPasso){
        //totalConDesc = 0;
        cout<<"New Simulation"<<endl;
        Kernel::InitialiseAll();
        cout<<"Simulating..."<<endl;
		Kernel::Simulate(ElementPosition,vetor);
        Kernel::FinaliseAll();
        pb = (long double) Def::numReq_Bloq/Def::numReq;
        cout<<endl<<endl;
        return pb;
	}
}

void Kernel::Simulate(int ElementPosition,int vetor){
	//Create an event to be the first request:
    Event *evt = new Event();
    Event::setRequestEvent(evt, Schedule::getSimTime());
    Schedule::scheduleEvent(evt);
	while(Def::numReq < Def::getNumReqMax()){
        if(Def::log){
                cout<<endl<<"New Event"<<endl;
                Schedule::printAllEvents();
                Topology::printAllNodes();
        }
        //Atualiza o evento atual e o tempo de simulação do tempo anterior
        Event *curEvent = Schedule::getCurrentEvent();
		if(curEvent->getType() == Req){ //Chegou uma requisição
            //cout<<"Req="<<Def::numReq<<"  ";
	        ConnectionRequest(curEvent,ElementPosition,vetor);
            //Define the next Request Event
            long double IAT = General::exponential(laNet); //Inter-arrival time
            Event::setRequestEvent(curEvent, Schedule::getSimTime() + IAT); //Reuse the same Event Object
            assert(evt->getType() == Req);
            assert(evt->getConnection() == NULL);
            //Prepara o próximo evento
            Schedule::scheduleEvent(curEvent);
        }
	    else
		if(curEvent->getType() == Desc){ //Desconnection Request
            ConnectionRelease(curEvent);
            delete curEvent;
		}
        else
        if(curEvent->getType() == Exp){ //Chegou um pedido de expansao
			assert(ExpComp); //Um evento deste tipo so pode ocorrer se ExpComp=true;
            Heuristics::ExpandConnection(curEvent->getConnection());
            //DefineNextEventOfCon(curEvent);
            Schedule::scheduleEvent(curEvent);
        }
        else
        if(curEvent->getType() == Comp){ //Chegou um pedido de compressao
			assert(ExpComp); //Um evento deste tipo so pode ocorrer se ExpComp=true;
            Heuristics::CompressConnection(curEvent->getConnection());
            //DefineNextEventOfCon(curEvent);
            Schedule::scheduleEvent(curEvent);
        }
	}
}

void Kernel::ConnectionRequest(Event* evt,int ElementPosition,int vetor){
    Def::numReq++;
    int orN, deN;
    //Choose Request Parameters:
    Traffic::sourceDestinationTrafficRequest(orN, deN);
    assert(Topology::nodeValid(orN) && Topology::nodeValid(deN) && (orN!=deN));
    double bps = Traffic::bitRateTrafficRequest(); //Amount of Bits-per-second
    Assignment *assignment = new Assignment(orN, deN);
    assignment->setBitRate(bps);
    assignment->setDeactivationTime(Schedule::getSimTime() + General::exponential(mu));
    //Call RSA:
    //Physical Layer
    if(Def::getSpectrumAssignmentType() == 11 || Def::getSpectrumAssignmentType() == 12){
        if(Def::getSpectrumAssignmentType() == 11){
            if(RSA_XT_FF(assignment,ElementPosition,vetor)){ //The request was accepted
                Def::numReq_Acc++;
                ConnectionSetup(assignment);
            }
            else{
                Def::numReq_Bloq++;

            }
        }
        if(Def::getSpectrumAssignmentType() == 12){
            if(RSA_XT_Greedy(assignment,ElementPosition,vetor)){ //The request was accepted
                Def::numReq_Acc++;
                ConnectionSetup(assignment);
            }
            else{
                Def::numReq_Bloq++;
            }
        }
    }
    else{
        if(RSA(assignment,ElementPosition,vetor)){
            Def::numReq_Acc++;
        }
        else{
            Def::numReq_Bloq++;
            delete assignment;
        }
    }
}

void Kernel::coreRequestCountAdd(Assignment* assignment,bool sub_add){
Route* route = assignment->getRoute();
Fiber* fiber;
Core* core;
unsigned int req_slots = assignment->getNumSlots(),orNode,deNode,hops = route->getNumHops();
    for(unsigned int h = 0;h < hops;h++){
        orNode = route->getNode(h);
        deNode = route->getNode(h+1);
        fiber  = Topology::getFiber(orNode,deNode);
        core   = fiber->getFiberCore(assignment->getHopCoreId(h));
        if(req_slots == 3){
            if(sub_add)
                core->num_req_3slots++;
            else
                core->num_req_3slots--;
        }
        if(req_slots == 4){
            if(sub_add)
                core->num_req_4slots++;
            else
                core->num_req_4slots--;
        }
        if(req_slots == 5){
            if(sub_add)
                core->num_req_5slots++;
            else
                core->num_req_5slots--;
        }
    }
}

bool Kernel::RSA_XT_Greedy(Assignment* assignment,int ElementPosition,int vetor){
double ber = Traffic::getBER(),br = assignment->getBitRate();
Route *route;
bool phyLayer;
int M,numRoutes = Routing::KYEN,ns;
bool alocOSNR = false,aloc = false;
    //Tenta UMA SUBPORTADORA  -> (QAM-64,QAM-32 MAIS DE UMA SUBPORTADORA -> (QAM-16, QAM-8, QAM-4))
    if(assignment->getBitRate() == 42800000000 || assignment->getBitRate() == 112000000000)
            M = 6;
    else
            M = 4;
    int polarization = Traffic::getPolarization();
    double bandwidth; //= Modulation::bandwidthQAM(M, assignment->getBitRate(), polarization);
    Heuristics::Routing(assignment);//Fill assignment with the set of routes to be tryed
    assignment->initializeModMatrix(M - 1);
    assignment->initializeNumSlotsMatrix(M - 1);
    assignment->initializePosAloc(numRoutes,M - 1);
        //Routing was successful
        if(assignment->isThereTrialRoute()){
            //Para todas as rotas tenta associar um núcleo e um conjunto de slots
            for(int num = 0;num < numRoutes;num++){
                int modulation = M;
                do{
                      if(assignment->getBitRate() == 1120000000000 && modulation == 2)
                            int a = 0;
                      bandwidth = Modulation::bandwidthQAM(modulation, assignment->getBitRate(), polarization);
                      assignment->setNumSlotsMatrix(num,M - modulation,ceil(bandwidth/Def::slotBW + 1));
                      //Reseta possibilidades de alocação para a nova quantidade de slots
                      //assignment->resetTempCore(num);
                      //assignment->resetTempSlot(num);
                      //assignment->resetXTVec(num);
                      //Verifica disponibilidade
                      Heuristics::SpectrumAssignment(assignment,0,0,num,M - modulation,assignment->getNumSlotsMatrix(num,M - modulation));
                      if(assignment->getTempCoreSize(num,M - modulation) > 0){
                        assignment->setModulationMatrix(num,M - modulation,modulation);//Modulação 1 - NÃO EXISTE
                        //Existe capacidade
                        aloc = true;
                      }
                      else{
                        assignment->setModulationMatrix(num,M - modulation,1);
                      }
                      modulation--;
                }while(modulation > 1);
            }
            //Não há capacidade de núcleo e slots
            if(!aloc){
                //Alocação não foi possível, bloqueio por XT!
                Topology::resourceBloq++;
                assignment->setRoute(NULL);
                assignment->clearTrialRoutes();
                delete assignment;
                return false;
            }
            //Teste de OSNR
            for(int num = 0;num < numRoutes;num++){
                     int modulation = M;
                     route = assignment->getTrialRoute(num);
                     assignment->setRoute(route);
                     //Verificar os formatos de modulação para a adequação da OSNR
                     do{
                        if(assignment->getModulationMatrix(num,M - modulation) != 1){
                             phyLayer = false;
                             //Se passar por essa condição a potência do sinal é setada no vetor de potência!
                             if(phyLayer = Topology::checkOSNRVec(route,Modulation::getOSNRLimit(assignment->getModulationMatrix(num,M - modulation),ber,br),assignment,num)){
                                //Existe rota e formato de modulação possíveis
                                alocOSNR = true;
                             }
                             //Rota não alcança o OSNR necessário do formato de modulação
                             else
                                assignment->setModulationMatrix(num,M - modulation,1);
                        }
                        modulation--;
                     }while(modulation > 1);
            }
             //Nenhuma rota e formato de modulação conseguem ser maior que o limite de OSNR
            if(!alocOSNR){
                //Alocação não foi possível, bloqueio por XT!
                Topology::resourceBloq++;
                assignment->setRoute(NULL);
                assignment->clearTrialRoutes();
                delete assignment;
                return false;
            }
            //Verificar se o XT das possíveis alocações estão abaixo do Threshold
            for(int num = 0;num < numRoutes;num++){
                for(int modulation = M;modulation > 1;modulation--){
                    //Verifica se a rota tem uma possível alocação
                    if(assignment->getModulationMatrix(num,M - modulation) != 1){
                        route = assignment->getTrialRoute(num);
                        //Varia as possíveis alocações da rota
                        for(int a = 0;a < assignment->getTempCoreSize(num,M - modulation);a++){
                                //Seta o XT da possível alocação
                                //Verifica se o XT da possivel alocação está abaixo do limite
                                if(!Modulation::getXT_Index(assignment,num,a,true,M - modulation)){
                                   //Possível alocação não atende ao críterio de XT
                                   assignment->setTempCore(num,M - modulation,a,Def::MAX_INT);
                                }
                                else if(assignment->getXTVec(num,M - modulation,a) == 0){
                                //Caso a possível alocação não sofra com XT e não interfira em nenhuma outra conexão, a alocação é realizada imediatamente
                                    Topology::numReqPerCore.at(assignment->getTempCore(num,M - modulation,a))++;
                                    Topology::numReqPerRoute.at(num)++;
                                    assignment->setCoreId(assignment->getTempCore(num,M - modulation,a));
                                    assignment->setPower(assignment->getPowerVec(num));
                                    assignment->setFirstSlot(assignment->getTempSlot(num,M - modulation,a));
                                    assignment->setNumSlots(assignment->getNumSlotsMatrix(num,M - modulation));
                                    assignment->setLastSlot(assignment->getTempSlot(num,M - modulation,a) + assignment->getNumSlots() - 1);
                                    assignment->setRoute(assignment->getTrialRoute(num));
                                    assignment->setXTReal(assignment->getXTVec(num,M - modulation,a));
                                    assignment->setXTLimit(Modulation::getXTLimit(assignment->getModulationMatrix(num,M - modulation),assignment->getBitRate(),Modulation::getOutagePb(),ber));
                                    assignment->setOSNRth(Modulation::getOSNRLimit(assignment->getModulationMatrix(num,M - modulation), ber,assignment->getBitRate()));
                                    Topology::xtMed = Topology::xtMed + assignment->getXTReal();
                                    if(assignment->isLightPathValid()){
                                        assignment->clearTrialRoutes(); //Clear the remaining routes in assignment;
                                        return true;
                                    }
                                }
                        }
                }
               }
            }
            //Apenas as possiveis alocações que atendem aos limites de OSNR e XT seguirão como possibilidades no assignment
            if(Modulation::setInterConnectionsXT(assignment,numRoutes,M - 1)){//Conexão alocada de acordo com o algoritmo do artigo
                    if(assignment->isLightPathValid()){
                         Topology::xtMed = Topology::xtMed + assignment->getXTReal();
                         assignment->clearTrialRoutes(); //Clear the remaining routes in assignment;
                         return true;
                    }
            }
            else{//Conexão não foi alocada,bloqueio!
                Topology::xtBloq++;
                assignment->setRoute((NULL));
                assignment->clearTrialRoutes();
                delete assignment;
                return false;
            }
        }
        //Quando não há rotas!
        else{
            assignment->setRoute((NULL));
            assignment->clearTrialRoutes();
            delete assignment;
            return false;
        }
}
//Lógica para a alocação FF com camada física
//Para a heurística CorePriorized com 7 núcleos e 1 no centro, a ordem deve ser 0,2,4,5,3/1,1/3,6
//Fazendo do jeito que eu acho que elesimplementaram, no entato não faz muito sentido para mim!
bool Kernel::RSA_XT_CP(Assignment* assignment,int ElementPosition,int vetor){
/*double ber = Traffic::getBER(),br = assignment->getBitRate();
Route *route;
bool phyLayer;
int M,numRoutes = Routing::KYEN,ns;
bool alocOSNR = false,aloc = false;
    //Tenta UMA SUBPORTADORA  -> (QAM-64,QAM-32 MAIS DE UMA SUBPORTADORA -> (QAM-16, QAM-8, QAM-4))
    if(assignment->getBitRate() == 42.8*pow(10,9) || assignment->getBitRate() == 112*pow(10,9))
            M = 6;
    else
            M = 4;
    int polarization = Traffic::getPolarization();
    double bandwidth; //= Modulation::bandwidthQAM(M, assignment->getBitRate(), polarization);
    Heuristics::Routing(assignment);//Fill assignment with the set of routes to be tryed
    assignment->initializeModMatrix(M - 1);
    assignment->initializeNumSlotsMatrix(M - 1);
        //Routing was successful
        if(assignment->isThereTrialRoute()){
            //Para todas as rotas tenta associar um núcleo e um conjunto de slots
            for(int num = 0;num < numRoutes;num++){
                int modulation = M;
                do{
                      bandwidth = Modulation::bandwidthQAM(modulation, assignment->getBitRate(), polarization);
                      assignment->setNumSlotsMatrix(num,M - modulation,ceil(bandwidth/Def::slotBW + 1));
                      //Reseta possibilidades de alocação para a nova quantidade de slots
                      assignment->resetTempCore(num);
                      assignment->resetTempSlot(num);
                      assignment->resetXTVec(num);
                      //Verifica disponibilidade
                      Heuristics::SpectrumAssignment(assignment,0,0,num,assignment->getNumSlotsMatrix(num,M - modulation));
                      if(assignment->getTempCoreSize(num) > 0){
                        assignment->setModulationMatrix(num,M - modulation,modulation);//Modulação 1 - NÃO EXISTE
                        //Existe capacidade
                        aloc = true;
                      }
                      else{
                        assignment->setModulationMatrix(num,M - modulation,1);
                      }
                      modulation--;
                }while(modulation > 1);
            }
            //Não há capacidade de núcleo e slots
            if(!aloc){
                //Alocação não foi possível, bloqueio por XT!
                Topology::resourceBloq++;
                assignment->setRoute(NULL);
                assignment->clearTrialRoutes();
                delete assignment;
                return false;
            }
            //Teste de OSNR
            for(int num = 0;num < numRoutes;num++){
                     int modulation = M;
                     route = assignment->getTrialRoute(num);
                     assignment->setRoute(route);
                     //Verificar os formatos de modulação para a adequação da OSNR
                     do{
                         phyLayer = false;
                         //Se passar por essa condição a potência do sinal é setada no vetor de potência!
                         if(phyLayer = Topology::checkOSNRVec(route,Modulation::getOSNRLimit(modulation,ber,br),assignment,num)){
                            //Existe rota e formato de modulação possíveis
                            alocOSNR = true;
                         }
                         //Rota não alcança o OSNR necessário do formato de modulação
                         else
                            assignment->setModulationMatrix(num,M - modulation,1);
                         modulation--;
                     }while(modulation > 1);
            }
             //Nenhuma rota e formato de modulação conseguem ser maior que o limite de OSNR
            if(!alocOSNR){
                //Alocação não foi possível, bloqueio por XT!
                Topology::resourceBloq++;
                assignment->setRoute(NULL);
                assignment->clearTrialRoutes();
                delete assignment;
                return false;
            }
            //Teste de XT
            for(int num = 0;num < numRoutes;num++){
                for(int modulation = M;modulation > 1;modulation--){
                    //Formato de modulação é possível
                    if(assignment->getModulationMatrix(num,M - modulation) != 1){
                          //Reseta possibilidades de alocação para a nova quantidade de slots
                          assignment->resetTempCore(num);
                          assignment->resetTempSlot(num);
                          assignment->resetXTVec(num);
                          //Verifica disponibilidade
                          Heuristics::SpectrumAssignment(assignment,0,0,num,assignment->getNumSlotsMatrix(num,M - modulation));
                          if(assignment->getTempCoreSize(num) > 0){
                                 //Varia as possíveis alocações da rota
                                 for(int posAloc = 0;posAloc < assignment->getTempCoreSize(num);posAloc++){
                                      if(Modulation::getXT(assignment,num,posAloc,false,M - modulation)){//Função, verifica limites de XT para conexões interferentes e para a possível alocação,retorna TRUE caso todos estejam abaixo do limite de XT
                                            //XT das conexões interferentes já é atualizado na função!
                                            //Alocação será feita
                                            Topology::numReqPerRoute.at(num)++;
                                            Topology::numReqPerCore.at(assignment->getTempCore(num,posAloc))++;
                                            assignment->setCoreId(assignment->getTempCore(num,posAloc));
                                            assignment->setPower(assignment->getPowerVec(num));
                                            assignment->setOSNRth(Modulation::getOSNRQAM(assignment->getModulationMatrix(num,M - modulation), ber, assignment->getBitRate(), polarization));
                                            assignment->setXTReal(assignment->getXTVec(num,posAloc));//Linear
                                            assignment->setXTLimit(Modulation::getXTLimit(assignment->getModulationMatrix(num,M - modulation),assignment->getBitRate(),Modulation::getOutagePb(),ber));
                                            assignment->setFirstSlot(assignment->getTempSlot(num,posAloc));
                                            assignment->setNumSlots(assignment->getNumSlotsMatrix(num,M - modulation));
                                            assignment->setLastSlot(assignment->getTempSlot(num,posAloc) + assignment->getNumSlots() - 1);
                                            assignment->setRoute(assignment->getTrialRoute(num));
                                            Topology::xtMed = Topology::xtMed + assignment->getXTReal();
                                            //cout <<"Average XT ="<< Topology::xtMed << endl;
                                            if(assignment->isLightPathValid()){
                                                  assignment->clearTrialRoutes(); //Clear the remaining routes in assignment;
                                                  ConnectionSetup(assignment);
                                                  return true;
                                            }
                                        }
                                 }
                          }
                    }
                }
            }
        }
        //Alocação não foi possível, bloqueio por XT!
        Topology::xtBloq++;
        assignment->setRoute(NULL);
        assignment->clearTrialRoutes();
        delete assignment;
        return false;*/
}

bool Kernel::RSA_XT_Random(Assignment* assignment,int ElementPosition,int vetor){
double ber = Traffic::getBER(),br = assignment->getBitRate();
Route *route;
bool phyLayer;
int M,numRoutes = Routing::KYEN,ns;
bool alocOSNR = false,aloc = false;
    //Tenta UMA SUBPORTADORA  -> (QAM-64,QAM-32 MAIS DE UMA SUBPORTADORA -> (QAM-16, QAM-8, QAM-4))
    if(assignment->getBitRate() == 42.8*pow(10,9) || assignment->getBitRate() == 112*pow(10,9))
            M = 6;
    else
            M = 4;
    int polarization = Traffic::getPolarization();
    double bandwidth; //= Modulation::bandwidthQAM(M, assignment->getBitRate(), polarization);
    Heuristics::Routing(assignment);//Fill assignment with the set of routes to be tryed
    assignment->initializeModMatrix(M - 1);
    assignment->initializeNumSlotsMatrix(M - 1);
    assignment->initializePosAloc(numRoutes,M - 1);

        //Routing was successful
        if(assignment->isThereTrialRoute()){
            //Para todas as rotas tenta associar um núcleo e um conjunto de slots
            for(int num = 0;num < numRoutes;num++){
                int modulation = M;
                do{
                      bandwidth = Modulation::bandwidthQAM(modulation, assignment->getBitRate(), polarization);
                      assignment->setNumSlotsMatrix(num,M - modulation,ceil(bandwidth/Def::slotBW + 1));
                      //Verifica disponibilidade
                      Heuristics::SpectrumAssignment(assignment,0,0,num,M - modulation,assignment->getNumSlotsMatrix(num,M - modulation));
                      if(assignment->getTempCoreSize(num,M - modulation) > 0){
                        assignment->setModulationMatrix(num,M - modulation,modulation);//Modulação 1 - NÃO EXISTE
                        //Existe capacidade
                        aloc = true;
                      }
                      else{
                        assignment->setModulationMatrix(num,M - modulation,1);
                      }
                      modulation--;
                }while(modulation > 1);
            }
            //Não há capacidade de núcleo e slots
            if(!aloc){
                //Alocação não foi possível, bloqueio por XT!
                Topology::resourceBloq++;
                assignment->setRoute(NULL);
                assignment->clearTrialRoutes();
                delete assignment;
                return false;
            }
            //Teste de OSNR
            for(int num = 0;num < numRoutes;num++){
                     int modulation = M;
                     route = assignment->getTrialRoute(num);
                     assignment->setRoute(route);
                     //Verificar os formatos de modulação para a adequação da OSNR
                     do{
                        if(assignment->getModulationMatrix(num,M - modulation) != 1){
                             phyLayer = false;
                             //Se passar por essa condição a potência do sinal é setada no vetor de potência!
                             if(phyLayer = Topology::checkOSNRVec(route,Modulation::getOSNRLimit(assignment->getModulationMatrix(num,M - modulation),ber,br),assignment,num)){
                                //Existe rota e formato de modulação possíveis
                                alocOSNR = true;
                             }
                             //Rota não alcança o OSNR necessário do formato de modulação
                             else
                                assignment->setModulationMatrix(num,M - modulation,1);
                        }
                        modulation--;
                     }while(modulation > 1);
            }
             //Nenhuma rota e formato de modulação conseguem ser maior que o limite de OSNR
            if(!alocOSNR){
                //Alocação não foi possível, bloqueio por XT!
                Topology::resourceBloq++;
                assignment->setRoute(NULL);
                assignment->clearTrialRoutes();
                delete assignment;
                return false;
            }
            //Testa a possibilidade de alocação randomicamente tentando a melhor rota com capacidade e que passou pela OSNR
            for(int num = 0;num < numRoutes;num++){
                 for(int modulation = M;modulation > 1;modulation--){
                    if(assignment->getModulationMatrix(num,M - modulation) != 1){
                        //Reseta possibilidades de alocação para a nova quantidade de slots
                        //assignment->resetTempCore(num);
                        //assignment->resetTempSlot(num);
                        //assignment->resetXTVec(num);
                        //Verifica disponibilidade
                        //Heuristics::SpectrumAssignment(assignment,0,0,num,assignment->getNumSlotsMatrix(num,M - modulation),M - modulation);
                        //Verifica se o formato de modulação passou pelos testes de capacidade e OSNR
                        if(assignment->getTempCoreSize(num,M - modulation) > 0){
                            for(int posAloc = 0; posAloc < assignment->getTempCoreSize(num,M - modulation);posAloc++){
                               //Cada tentativa aleatória sem sucesso,possivel alocação deve ser retirada do vetor
                               int random;
                               //srand(time(NULL));
                               random = rand()% assignment->getTempCoreSize(num,M - modulation);
                               if(Modulation::getXT_Index(assignment,num,random,false,M - modulation)){
                                    //Alocação será feita
                                    Topology::numReqPerRoute.at(num)++;
                                    Topology::numReqPerCore.at(assignment->getTempCore(num,M - modulation,random))++;
                                    assignment->setCoreId(assignment->getTempCore(num,M - modulation,random));
                                    assignment->setPower(assignment->getPowerVec(num));
                                    //assignment->setOSNRth(Modulation::getOSNRQAM(assignment->getModulationMatrix(num,M - modulation), ber, assignment->getBitRate(), polarization));
                                    //XT Real serve para o modelo de Cartaxo, para o modelo tradicional não usamos esse atributo
                                    assignment->setXTReal(assignment->getXTVec(num,M - modulation,random));
                                    assignment->setXTLimit(Modulation::getXTLimit(assignment->getModulationMatrix(num,M - modulation),assignment->getBitRate(),Modulation::getOutagePb(),ber));
                                    assignment->setFirstSlot(assignment->getTempSlot(num,M - modulation,random));
                                    assignment->setNumSlots(assignment->getNumSlotsMatrix(num,M - modulation));
                                    assignment->setLastSlot(assignment->getTempSlot(num,M - modulation,random) + assignment->getNumSlots() - 1);
                                    assignment->setRoute(assignment->getTrialRoute(num));
                                    Topology::xtMed = Topology::xtMed + assignment->getXTReal();
                                    if(assignment->isLightPathValid()){
                                          assignment->clearTrialRoutes(); //Clear the remaining routes in assignment;
                                          return true;
                                    }
                               }
                               else{
                                    if(assignment->getTempCoreSize(num,M - modulation) > 0){
                                        assignment->delTempCore(num,M - modulation,random);
                                        assignment->delTempSlot(num,M - modulation,random);
                                    }
                               }
                            }
                        }
                    }
                 }
            }
            //Alocação não foi possível, bloqueio por XT!
            Topology::xtBloq++;
            assignment->setRoute(NULL);
            assignment->clearTrialRoutes();
            delete assignment;
            return false;
        }
}

bool Kernel::RSA_XT_FF(Assignment* assignment,int ElementPosition,int vetor){
double ber = Traffic::getBER(),br = assignment->getBitRate();
Route *route;
bool phyLayer;
int M,numRoutes = Routing::KYEN,ns;
bool alocOSNR = false,aloc = false;
    //Tenta UMA SUBPORTADORA  -> (QAM-64,QAM-32 MAIS DE UMA SUBPORTADORA -> (QAM-16, QAM-8, QAM-4))
    if(assignment->getBitRate() == 42800000000 || assignment->getBitRate() == 112000000000)
            M = 6;
    else
            M = 4;
    int polarization = Traffic::getPolarization();
    double bandwidth; //= Modulation::bandwidthQAM(M, assignment->getBitRate(), polarization);
    Heuristics::Routing(assignment);//Fill assignment with the set of routes to be tryed
    assignment->initializeModMatrix(M - 1);
    assignment->initializeNumSlotsMatrix(M - 1);
    assignment->initializePosAloc(numRoutes,M - 1);
        //Routing was successful
        if(assignment->isThereTrialRoute()){
            //Para todas as rotas tenta associar um núcleo e um conjunto de slots
            for(int num = 0;num < numRoutes;num++){
                int modulation = M;
                do{
                      bandwidth = Modulation::bandwidthQAM(modulation, assignment->getBitRate(), polarization);
                      assignment->setNumSlotsMatrix(num,M - modulation,ceil(bandwidth/Def::slotBW + 1));
                      //Verifica disponibilidade
                      Heuristics::SpectrumAssignment(assignment,0,0,num,M - modulation,assignment->getNumSlotsMatrix(num,M - modulation));
                      if(assignment->getTempCoreSize(num,M - modulation) > 0){
                        assignment->setModulationMatrix(num,M - modulation,modulation);
                        //Existe capacidade
                        aloc = true;
                      }
                      else{
                        assignment->setModulationMatrix(num,M - modulation,1);
                      }
                      modulation--;
                }while(modulation > 1);
            }
            //Não há capacidade de núcleo e slots
            if(!aloc){
                //Alocação não foi possível, bloqueio por XT!
                Topology::resourceBloq++;
                assignment->setRoute(NULL);
                assignment->clearTrialRoutes();
                delete assignment;
                return false;
            }
            //Teste de OSNR
            for(int num = 0;num < numRoutes;num++){
                     int modulation = M;
                     route = assignment->getTrialRoute(num);
                     assignment->setRoute(route);
                     //Verificar os formatos de modulação para a adequação da OSNR
                     do{
                        if(assignment->getModulationMatrix(num,M - modulation) != 1){
                             phyLayer = false;
                             //Se passar por essa condição a potência do sinal é setada no vetor de potência!
                             //Topology::printRouteState(assignment->getTrialRoute(num),assignment->getNumSlotsMatrix(num,M - modulation));
                             if(phyLayer = Topology::checkOSNRVec(route,Modulation::getOSNRLimit(assignment->getModulationMatrix(num,M - modulation),ber,br),assignment,num)){
                                //Existe rota e formato de modulação possíveis
                                alocOSNR = true;
                             }
                             //Rota não alcança o OSNR necessário do formato de modulação
                             else
                                assignment->setModulationMatrix(num,M - modulation,1);
                        }
                        modulation--;
                     }while(modulation > 1);
            }
             //Nenhuma rota e formato de modulação conseguem ser maior que o limite de OSNR
            if(!alocOSNR){
                //Alocação não foi possível, bloqueio por XT!
                Topology::resourceBloq++;
                assignment->setRoute(NULL);
                assignment->clearTrialRoutes();
                delete assignment;
                return false;
            }
            //Teste de XT
            for(int num = 0;num < numRoutes;num++){
                for(int modulation = M;modulation > 1;modulation--){
                    //Formato de modulação é possível
                    if(assignment->getModulationMatrix(num,M - modulation) != 1){
                          if(assignment->getTempCoreSize(num,M - modulation) > 0){
                                 //Varia as possíveis alocações da rota
                                 for(int posAloc = 0;posAloc < assignment->getTempCoreSize(num,M - modulation);posAloc++){
                                      //Modelo tradicional,sem ser o de Cartaxo com índice
                                      if(Modulation::getXT_Index(assignment,num,posAloc,false,M - modulation)){//Função, verifica limites de XT para conexões interferentes e para a possível alocação,retorna TRUE caso todos estejam abaixo do limite de XT
                                            //XT das conexões interferentes já é atualizado na função!
                                            //Alocação será feita
                                            Topology::numReqPerRoute.at(num)++;
                                            Topology::numReqPerCore.at(assignment->getTempCore(num,M - modulation,posAloc))++;
                                            assignment->setCoreId(assignment->getTempCore(num,M - modulation,posAloc));
                                            assignment->setPower(assignment->getPowerVec(num));
                                            assignment->setOSNRth(Modulation::getOSNRQAM(assignment->getModulationMatrix(num,M - modulation), ber, assignment->getBitRate(), polarization));
                                            //XT Real serve para o modelo de Cartaxo, para o modelo tradicional não usamos esse atributo
                                            assignment->setXTReal(assignment->getXTVec(num,M - modulation,posAloc));//Linear
                                            assignment->setXTLimit(Modulation::getXTLimit(assignment->getModulationMatrix(num,M - modulation),assignment->getBitRate(),Modulation::getOutagePb(),ber));
                                            assignment->setRoute(assignment->getTrialRoute(num));
                                            assignment->setFirstSlot(assignment->getTempSlot(num,M - modulation,posAloc));
                                            assignment->setNumSlots(assignment->getNumSlotsMatrix(num,M - modulation));
                                            assignment->setLastSlot(assignment->getTempSlot(num,M - modulation,posAloc) + assignment->getNumSlots() - 1);
                                            Topology::xtMed = Topology::xtMed + assignment->getXTReal();
                                            if(assignment->isLightPathValid()){
                                                  assignment->clearTrialRoutes(); //Clear the remaining routes in assignment;
                                                  return true;
                                            }
                                        }
                                 }
                          }
                    }
                }
            }
        }
        //Alocação não foi possível, bloqueio por XT!
        Topology::xtBloq++;
        assignment->setRoute(NULL);
        assignment->clearTrialRoutes();
        delete assignment;
        return false;
}

bool Kernel::RSA(Assignment* assignment,int ElementPosition,int vetor){
double ber = Traffic::getBER();
Route *route;
bool phyLayer;
int M = 4;
int polarization = Traffic::getPolarization();
        //Try QAM-16, QAM-8, QAM-4, QAM-2
        //While para tentar outros formatos de modulação
        //Roteamento:
        Heuristics::Routing(assignment);//Fill assignment with the set of routes to be tryed
        bool success = false;
        //Variaveis para verificar se houve bloqueio por recurso e parar a rotina
        int blockAntXt = Topology::xtBloq, blockAntRes = Topology::resourceBloq;
        if(assignment->isThereTrialRoute()){ //Routing was successful
            do{
                route = assignment->popTrialRoute();
                do{
                    double bandwidth = Modulation::bandwidthQAM(M, assignment->getBitRate(), polarization);
                    assignment->setBandwidth(bandwidth);
                    assignment->setNumSlots(ceil(bandwidth/Def::slotBW));
                    int ns = assignment->getNumSlots();
                    phyLayer = true;
                    if(phyLayer){
                        assignment->setRoute(route);
                        Heuristics::SpectrumAssignment(assignment,ElementPosition,vetor,0,0,0); //Atribuição de slots:
                        if(assignment->isLightPathValid()){ //A set of slots was found for the connection
                            //Request was accepted
                            success = true;
                            assignment->clearTrialRoutes(); //Clear the remaining routes in assignment;
                            ConnectionSetup(assignment);
                            break;

                        }
                        else{
                            //Ocorreu bloqueio por recurso
                            if(Topology::resourceBloq == blockAntRes + 1)
                                break;
                        }
                    }
                    if(!success){
                        M--;
                    }
                    else
                        break;
                }while(M > 1);
                if(success)
                    break;
                else{
                    delete route;
                    assignment->setRoute(NULL);
                }
            }while(assignment->isThereTrialRoute());
        }
        if(success)
            return true;
        else{
            assignment->setRoute((NULL));
            assignment->clearTrialRoutes();
            return false;
        }
}

void Kernel::ConnectionSetup(Assignment* assignment){
    Connection *newConnection = new Connection(assignment);
    Topology::Connect(newConnection);
    Event *evtNewCon = new Event;
    Event::setReleaseEvent(evtNewCon, newConnection);
    Schedule::scheduleEvent(evtNewCon);
    //totalConDesc++;
    //Topology::updateNU();
    //AFTER SETUP UPDATE NU
}

void Kernel::ConnectionRelease(Event* evt){
    Connection *connection = evt->getConnection();
    Topology::releaseConnection(connection);
    //After release UPDATE NU
    //Update core count requests-PSO
    //coreRequestCountAdd(connection->getAssignment(),false);
    //totalConDesc++;
    //Topology::updateNU();
    delete connection;
}

void Kernel::InitialiseAll() { //Para o inicio geral.
    cout<<"InitialiseAll"<<endl;
    Topology::initialise();
    Def::initialise();
    Schedule::initialise();
    assert(Schedule::isEmpty());
    assert(!Topology::areThereOccupiedSlots());
}

void Kernel::FinaliseAll() {
    cout<<"FinaliseAll"<<endl;
    //
    cout <<"Simulation Time= " << Schedule::getSimTime() << "  numReq=" << Def::numReq << endl;
    cout << "nu0= " << laNet << "   PbReq= " << (long double) Def::numReq_Bloq/Def::numReq <<
    "   PbSlots= " << (long double) Def::numSlots_Bloq/Def::numSlots_Req << " HopsMed= " <<
    (long double) Def::numHopsPerRoute/(Def::numReq-Def::numReq_Bloq) << " netOcc= " <<
    (long double) Def::netOccupancy << endl;
    cout <<"Requisitions accepted= "<< Def::numReq_Acc << "   Requisitions blocked= " << Def::numReq_Bloq << endl;
    cout <<"XT Block= "<< Topology::xtBloq << "   Resource Block= " << Topology::resourceBloq << endl;
    //for(int core = 0;core < Topology::getNumCores();core++)
    //    cout <<"Number of req in core "<< core << " = " << Topology::numReqPerCore.at(core) << endl;
    //for(int route = 0;route < Routing::KYEN;route++)
    //    cout <<"Number of req in route "<< route << " = " << Topology::numReqPerRoute.at(route) << endl;
    //cout <<"Average XT ="<< Topology::xtMed/Def::numReq << endl;
    //cout <<"Average XT ="<< General::linearTodB(Topology::xtMed/Def::numReq_Acc) << endl;
    InpOut::Result << laNet << "\t" << (long double) Def::numReq_Bloq/Def::numReq <<endl;
    //cout <<Topology::getNU()/float(totalConDesc) << endl;
    //cout <<totalConDesc<<endl;
    Connection *con;
    Event *evtPtr = Schedule::getCurrentEvent();
    while(evtPtr != NULL){
        con = evtPtr->getConnection();
		if(con != NULL){ //This is a Connection
            Topology::releaseConnection(con);
			delete con;
		}
        delete evtPtr;
        evtPtr = Schedule::getCurrentEvent();
    }
	//Check whether the simulator is correct:
    assert(!Topology::areThereOccupiedSlots());
    assert(Schedule::isEmpty());

} // End Of FinaliseAll()

void Kernel::AccountForBlocking(int NslotsReq, int NslotsUsed){
	if(NslotsUsed <= 0) //A conexão foi bloqueada
		Def::numReq_Bloq++;
	Def::numSlots_Bloq += (NslotsReq - NslotsUsed);
}

void Kernel::ImprimeFreqCores(){
    for(int a = 2;a < 9;a= a + a){
        for(int i = 0;i <7;i++){
            if(a == 2){
                cout <<"Requisições tamanho "<< a << "core" << i << "=" << Def::numReq_Acc_2slots.at(i) << endl;
                InpOut::Core_Freq <<"Requisições tamanho "<< a << " core " << i << " = " << Def::numReq_Acc_2slots.at(i) << endl;
            }
            else if(a == 4){
                cout <<"Requisições tamanho "<< a << "core" << i << "=" << Def::numReq_Acc_4slots.at(i) << endl;
                InpOut::Core_Freq <<"Requisições tamanho "<< a << " core " << i << " = " << Def::numReq_Acc_4slots.at(i) << endl;
            }
            else if(a == 8){
                cout <<"Requisições tamanho "<< a << "core" << i << "=" << Def::numReq_Acc_8slots.at(i) << endl;
                InpOut::Core_Freq <<"Requisições tamanho "<< a << " core " << i << " = " << Def::numReq_Acc_8slots.at(i) << endl;
            }
        }
    }
}

void Kernel::addNumSim(){
    numSim++;
}

int Kernel::getNumSim(){
    return numSim;
}


