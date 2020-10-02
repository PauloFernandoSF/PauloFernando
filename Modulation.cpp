//
//  Modulation.cpp
//  EONsimulator
//
//  Created by Raul Almeida on 27/02/15.
//  Copyright (c) 2015 UFPE - EON. All rights reserved.
//

#include "Modulation.h"

double Modulation::rollOff  = 0.0;
double Modulation::outagePb = 1*(pow (10.0 ,-5));

////////////////////////////////////////////////////////////////////////
double Modulation::bandwidthQAM(int M, double Rbps, double pol){
//Variavel que indica a distância entre subportadoras em um supercanal

    //Verificar se é supercanal ou não, retorna valores diferentes
    if(Rbps == 42800000000 || Rbps == 112000000000){
            if(Rbps == 42800000000)
                return ((1.0+rollOff)*42800000000/(pol*M));
            else
                return ((1.0+rollOff)*112000000000/(pol*M));
    }
    else{
        //MUDAR
        if(Rbps == 224000000000)
            return  (1.0+rollOff)*2*112000000000/(pol*M); //+ 112000000000/(pol*M);//(2 - 1)*((1.0+rollOff))*2*112000000000/(pol*M) + ((1.0+rollOff))*2*112000000000/(pol*M);
        if(Rbps == 448000000000)
            return  (1.0+rollOff)*4*112000000000/(pol*M); //+ 3*112000000000/(pol*M);//(4 - 1)*((1.0+rollOff))*4*112000000000/(pol*M) + (1.0+rollOff)*4*112000000000/(pol*M);
        if(Rbps == 1120000000000)
            return  (1.0+rollOff)*10*112000000000/(pol*M); //+ 9*112000000000/(pol*M);//(10 - 1)*((1.0+rollOff))*10*112000000000/(pol*M) + (1.0+rollOff)*10*112000000000/(pol*M);
    }

}
////////////////////////////////////////////////////////////////////////
double Modulation::getOSNRQAM(int M, double ber, double bitRate, int polarization){
    double snrb = Modulation::getsnrbQAM(M, ber);
    return General::linearTodB(bitRate*snrb/(polarization*Def::Bref));
}
// ----------------------------------------------------- //
double Modulation::getSNRbQAM(int M, double ber){
    //if (static_cast<double>((ber == pow(static_cast<float>(10),static_cast<int>(-3))))){ b
    if(float(ber) == float(pow(10.0, -3))){
        switch (M) {
            case 2: //QAM-4
                return 6.79;
                break;
            case 3: //QAM - 8
                return 9.03;
                break;
            case 4: //QAM-16
                return 10.52;
                break;
            case 5: //QAM-32
                return 12.57;
                break;
            case 6: //QAM-64
                return 14.77;
                break;
            default:
                cout<<"Unknown Modulation Format";
                break;
        }
    }
    else
    if(float(ber) == float(3.8*(pow (10.0 ,-3)))){
        switch (M) {
            case 2: //QAM-4
                return 5.52;
                break;
            case 3: //QAM - 8
                return 7.83;
                break;
            case 4: //QAM-16
                return 9.17;
                break;
            case 5: //QAM-32
                return 11.23;
                break;
            case 6: //QAM-64
                return 13.34;
                break;
            default:
                cout<<"Unknown Modulation Format";
                break;
        }
    }
    cout<<"Problem in Modulation::getSNRbQAM"<<endl;
    return 0.0;
}
// ----------------------------------------------------- //
double Modulation::getOSNRLimit(int M, double ber, double bitRate){
    if(bitRate == 42800000000){
        if(float(ber) == float(3.8*(pow (10.0 ,-3)))){
            switch (M) {
                case 2: //QAM-4
                    return 11.86;
                    break;
                case 3: //QAM - 8
                    return 14.17;
                    break;
                case 4: //QAM-16
                    return 15.51;
                    break;
                case 5: //QAM-32
                    return 17.57;
                    break;
                case 6: //QAM-64
                    return 19.68;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
        if(float(ber) == float(1.5*(pow (10.0 ,-2)))){
            switch (M) {
                case 2: //QAM-4
                    return 10.06;
                    break;
                case 3: //QAM - 8
                    return 12.52;
                    break;
                case 4: //QAM-16
                    return 13.56;
                    break;
                case 5: //QAM-32
                    return 15.62;
                    break;
                case 6: //QAM-64
                    return 18.57;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
    }
    else if(float(bitRate) == float(112*pow(10,9))){
        if(float(ber) == float(3.8*(pow (10.0 ,-3)))){
            switch (M) {
                case 2: //QAM-4
                    return 16.03;
                    break;
                case 3: //QAM - 8
                    return 18.34;
                    break;
                case 4: //QAM-16
                    return 19.68;
                    break;
                case 5: //QAM-32
                    return 21.74;
                    break;
                case 6: //QAM-64
                    return 23.85;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
        if(float(ber) == float(1.5*(pow (10.0 ,-2)))){
            switch (M) {
                case 2: //QAM-4
                    return 14.23;
                    break;
                case 3: //QAM - 8
                    return 16.69;
                    break;
                case 4: //QAM-16
                    return 17.73;
                    break;
                case 5: //QAM-32
                    return 19.79;
                    break;
                case 6: //QAM-64
                    return 21.74;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
    }
    else if(float(bitRate) == float(224*pow(10,9))){
        if(float(ber) == float(3.8*(pow (10.0 ,-3)))){
            switch (M) {
                case 2: //QAM-4
                    return 20.54;
                    break;
                case 3: //QAM - 8
                    return 22.85;
                    break;
                case 4: //QAM-16
                    return 24.19;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
        if(float(ber) == float(1.5*(pow (10.0 ,-2)))){
            switch (M) {
                case 2: //QAM-4
                    return 20.54;
                    break;
                case 3: //QAM - 8
                    return 22.85;
                    break;
                case 4: //QAM-16
                    return 24.19;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
    }
    else if(float(bitRate) == float(448*pow(10,9))){
        if(float(ber) == float(3.8*(pow (10.0 ,-3)))){
            switch (M) {
                case 2: //QAM-4
                    return 23.55;
                    break;
                case 3: //QAM - 8
                    return 25.86;
                    break;
                case 4: //QAM-16
                    return 27.20;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
        if(float(ber) == float(1.5*(pow (10.0 ,-2)))){
            switch (M) {
                case 2: //QAM-4
                    return 23.55;
                    break;
                case 3: //QAM - 8
                    return 25.86;
                    break;
                case 4: //QAM-16
                    return 27.20;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
    }
    else if(float(bitRate) == float(112*pow(10,10))){
        if(float(ber) == float(3.8*(pow (10.0 ,-3)))){
            switch (M) {
                case 2: //QAM-4
                    return 27.53;
                    break;
                case 3: //QAM - 8
                    return 29.84;
                    break;
                case 4: //QAM-16
                    return 31.18;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
        if(float(ber) == float(1.5*(pow (10.0 ,-2)))){
            switch (M) {
                case 2: //QAM-4
                    return 25.73;
                    break;
                case 3: //QAM - 8
                    return 28.19;
                    break;
                case 4: //QAM-16
                    return 29.23;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
    }

    cout<<"Problem in Modulation::getOSNRLIMIT"<<endl;
    return 0.0;
}
// ----------------------------------------------------- //
float Modulation::getXTLimit(int M, double bitRate,double outage,double ber){
    double a  = 42.8*pow(10,9);
    if(bitRate == 42800000000){
        if(float(outage) == float(1*(pow (10.0 ,-5))) && float(ber) == float(3.8*pow(10,-3))){
            switch (M) {
                case 2: //QAM-4
                    return -20.7;
                    break;
                case 3: //QAM - 8
                    return -24.78;
                    break;
                case 4: //QAM-16
                    return -27.36;
                    break;
                case 5: //QAM-32
                    return -30.39;
                    break;
                case 6: //QAM-64
                    return -33.29;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
        if(float(outage) == float(1*(pow (10.0 ,-5))) && float(ber) == float(1.5*pow(10,-2))){
            switch (M) {
                case 2: //QAM-4
                    return -18.9;
                    break;
                case 3: //QAM - 8
                    return -23.13;
                    break;
                case 4: //QAM-16
                    return -25.41;
                    break;
                case 5: //QAM-32
                    return -28.44;
                    break;
                case 6: //QAM-64
                    return -31.18;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
    }
    else if(float(bitRate) == float(112*pow(10,9))){
        if(float(outage) == float(1*(pow (10.0 ,-5))) && float(ber) == float(3.8*pow(10,-3))){
            switch (M) {
                case 2: //QAM-4
                    return -20.7;
                    break;
                case 3: //QAM - 8
                    return -24.77;
                    break;
                case 4: //QAM-16
                    return -27.36;
                    break;
                case 5: //QAM-32
                    return -30.39;
                    break;
                case 6: //QAM-64
                    return -33.29;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
        if(float(outage) == float(1*(pow (10.0 ,-5))) && float(ber) == float(1.5*pow(10,-2))){
            switch (M) {
                case 2: //QAM-4
                    return -18.9;
                    break;
                case 3: //QAM - 8
                    return -23.20;
                    break;
                case 4: //QAM-16
                    return -25.32;
                    break;
                case 5: //QAM-32
                    return -28.45;
                    break;
                case 6: //QAM-64
                    return -31.18;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
    }
    else if(float(bitRate) == float(224*pow(10,9))){
        if(float(outage) == float(1*(pow (10.0 ,-5))) && float(ber) == float(3.8*pow(10,-3))){
            switch (M) {
                case 2: //QAM-4
                    return -22.19;
                    break;
                case 3: //QAM - 8
                    return -26.27;
                    break;
                case 4: //QAM-16
                    return -28.86;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
        if(float(outage) == float(1*(pow (10.0 ,-5))) && float(ber) == float(1.5*pow(10,-2))){
            switch (M) {
                case 2: //QAM-4
                    return -20.4;
                    break;
                case 3: //QAM - 8
                    return -24.62;
                    break;
                case 4: //QAM-16
                    return -26.91;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
    }
    else if(float(bitRate) == float(448*pow(10,9))){
        if(float(outage) == float(1*(pow (10.0 ,-5))) && float(ber) == float(3.8*pow(10,-3))){
            switch (M) {
                case 2: //QAM-4
                    return -22.19;
                    break;
                case 3: //QAM - 8
                    return -26.27;
                    break;
                case 4: //QAM-16
                    return -28.86;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
        if(float(outage) == float(1*(pow (10.0 ,-5))) && float(ber) == float(1.5*pow(10,-2))){
            switch (M) {
                case 2: //QAM-4
                    return -20.4;
                    break;
                case 3: //QAM - 8
                    return -24.62;
                    break;
                case 4: //QAM-16
                    return -26.91;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
    }
    else if(float(bitRate) == float(112*pow(10,10))){
        if(float(outage) == float(1*(pow (10.0 ,-5))) && float(ber) == float(3.8*pow(10,-3))){
            switch (M) {
                case 2: //QAM-4
                    return -22.2;//-22.2
                    break;
                case 3: //QAM - 8
                    return -26.67; //-26.27
                    break;
                case 4: //QAM-16
                    return -28.86;  //-28.86
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
        if(float(outage) == float(1*(pow (10.0 ,-5))) && float(ber) == float(1.5*pow(10,-2))){
            switch (M) {
                case 2: //QAM-4
                    return -20.4;
                    break;
                case 3: //QAM - 8
                    return -24.62;
                    break;
                case 4: //QAM-16
                    return -26.91;
                    break;
                default:
                    cout<<"Unknown Modulation Format";
                    break;
            }
        }
    }
    cout<<"Problem in Modulation::getXTLIMIT"<<endl;
    return 0.0;
}
// ----------------------------------------------------- //
double Modulation::getsnrbQAM(int M, double ber){
    double SNRb = Modulation::getSNRbQAM(M, ber);
    return General::dBtoLinear(SNRb);
}
// ----------------------------------------------------- //
bool Modulation::getXT_Index(Assignment* assignment,int route,int pos,bool greedy,int modulationPos){
float h = 0,k,beta,R,A,Pxt = 0,XTmed = 0,XTtotal = 0,Pj = 0;
vector<float> intIndex, vecXt;
vector<Assignment*> vecAssignment;
double L;
int ori,iniSlot = assignment->getTempSlot(route,modulationPos,pos),adjCoreIndex;
Core* adjCore;
int de,numHops = assignment->getTrialRoute(route)->getNumHops(),nucleo = assignment->getTempCore(route,modulationPos,pos);
Fiber* fiber;
Link* link;
//coeficiente de acoplamento
k =  float(0.012);//0.00584;//
//constante de propagação
beta = pow(10,7);
//raio de curvatura
R = double(0.01);
//core pitch
A = double(4.5*pow(10,-5));
//
h = double((2*pow(k,2)*R)/(beta*A));
//DEBUG
//if((int*)assignment == (int*)0x6b53a0){
//    int a = 0;
//    Topology::printRouteState(assignment->getTrialRoute(route),assignment->getNumSlotsMatrix(route,modulationPos));
//}
    for(int c = 0;c < numHops;c++){
        ori = assignment->getTrialRoute(route)->getNode(c);de = assignment->getTrialRoute(route)->getNode(c + 1);
        fiber = Topology::getFiber(ori,de);
        link = Topology::getLink(ori,de);
        L = link->getLength()*1000;
        XTmed = 0;
        //Variation of adj cores
        for(int adj = 0;adj < fiber->getFiberCore(assignment->getTempCore(route,modulationPos,pos))->getNumAdjCores();adj++){
            //Topology::printRouteState(assignment->getTrialRoute(route),assignment->getNumSlotsMatrix(route,modulationPos));
            Assignment* antAssignment = NULL;
            adjCoreIndex = fiber->getFiberCore(assignment->getTempCore(route,modulationPos,pos))->getAdjCoreIndex(adj);
            adjCore = fiber->getFiberCore(adjCoreIndex);
            int position = Def::MAX_INT;
            //índice de interferencia-Verificar quantas conexões se sobrepoem
            for(int slot = iniSlot;slot < (iniSlot + assignment->getNumSlotsMatrix(route,modulationPos) - 1);slot++){
                    //Condição para impedir que a banda de guarda seja contada
                    if(fiber->getCoreOccupation(slot,adjCore) && fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot)->getLastSlot() != slot){
                            if(fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot) != antAssignment){
                                            bool existe = false;
                                            //Verificar se o assignment interferente está no vetor
                                            for(int a = 0;a < vecAssignment.size();a++){
                                                if(fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot) == vecAssignment.at(a)){
                                                //Caso o assignment interferente esteja no vetor
                                                    existe = true;
                                                    intIndex.at(a)++;
                                                    position = a;
                                                    antAssignment = vecAssignment.at(a);
                                                    break;
                                                }
                                            }
                                            if(!existe){
                                                intIndex.push_back(1);
                                                position = intIndex.size() - 1;
                                                vecAssignment.push_back(fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot));
                                                vecXt.push_back(fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot)->getXTReal());
                                                float x;
                                                x = fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot)->getXTReal();
                                                x = vecXt.at(position);
                                                antAssignment = fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot);
                                            }
                            }
                            else if(fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot) == antAssignment){
                                    intIndex.at(position)++;
                            }
                    }

            }
            //Verifica se alguma conexão interferiu no núcleo
            if(intIndex.size() != 0){
                //Varia entre todas as conexões interferentes naquele núcleo
                for(int tam = 0;tam < intIndex.size();tam++){
                    //Apenas posição que sofreu interferencia será acessada
                    if(intIndex.at(tam) > 0){
                        //Cálculos sem considerar índice
                        //Verificar vecAssignment não está atualizando a posição
                        //XT para a possível alocação
                        int interfSlots = vecAssignment.at(tam)->getNumSlots();
                        float index = 0, xtInter = 0,x;
                        index = intIndex.at(tam)/(vecAssignment.at(tam)->getNumSlots() - 1);
                        Pj = vecAssignment.at(tam)->getPower();
                        //cálculo da potência de XT
                        Pxt = index*Pj*h*L;
                        //potência média de XT no link
                        XTmed = XTmed + Pxt/(assignment->getPowerVec(route));
                        x = General::linearTodB(XTmed);
                        //XT para a conexão adjacente
                        index = intIndex.at(tam)/(assignment->getNumSlotsMatrix(route,modulationPos) - 1);
                        Pj = assignment->getPowerVec(route);
                        //cálculo da potência de XT
                        Pxt = index*Pj*h*L;
                        xtInter = Pxt/(vecAssignment.at(tam)->getPower());
                        //Verificar se este valor XT deixa a conexão fora do limite, se sim a possível alocação NÃO é uma possibilidade
                        vecXt.at(tam) = vecXt.at(tam) + xtInter;
                        x = General::linearTodB(vecXt.at(tam));
                        float j,u = 0;
                        //j = General::linearTodB(vecXt.at(tam));
                        //u = vecAssignment.at(tam)->getXTLimit();
                        //Possível alocação faz o XT da conexão interferida passar do limite?
                        //if(General::linearTodB(vecXt.at(tam)) > vecAssignment.at(tam)->getXTLimit()){
                            //Sim,retorna FALSE
                        //    return false;
                        //}
                        //else{
                        //    int t =0;
                    }
                }
            }
            //Atualizar XT da possível alocação
            //XTtotal += XTmed;
            //Resetar o vetor de intIndex para a identifiicação de conexões interferentes no próximo núcleo adjacente
            for(int a = 0;a < intIndex.size();a++){
                   intIndex.at(a) = 0;
            }
        }
        XTtotal += XTmed;
    }
    float t,y;
    t = Modulation::getXTLimit(assignment->getModulationMatrix(route,modulationPos),assignment->getBitRate(),Modulation::getOutagePb(),Traffic::getBER());
    y = General::linearTodB(XTtotal);
    //Verificar se XTTotal == 0 e retornar true
    if(XTtotal == 0){
        assignment->setXTVec(route,modulationPos,pos,XTtotal);//Linear
        return true;
    }
    if(General::linearTodB(XTtotal) < Modulation::getXTLimit(assignment->getModulationMatrix(route,modulationPos),assignment->getBitRate(),Modulation::getOutagePb(),Traffic::getBER())){
        //Atualizar o XT das conexões afetadas pela alocação, caso o algoritmo Greedy seja utilizado, a atualização do XT das conexões interferentes não é realizada agora
        if(greedy){
            assignment->setXTVec(route,modulationPos,pos,XTtotal);//Linear
            return true;
        }
        else{
            float u,lim;
            //Topology::printRouteState(assignment->getTrialRoute(route),assignment->getNumSlotsMatrix(route,modulationPos));
            for(int c = 0;c < vecAssignment.size();c++){
                //Divide todo o XT visto pela conexão interferente pela potência da possível alocação
                if(General::linearTodB(vecXt.at(c)) < vecAssignment.at(c)->getXTLimit()){
                    //Topology::printRouteState(assignment->getTrialRoute(route),assignment->getNumSlotsMatrix(route,modulationPos));
                    lim = vecAssignment.at(c)->getXTLimit();
                    u   = General::linearTodB(vecAssignment.at(c)->getXTReal());
                    vecAssignment.at(c)->setXTReal(vecXt.at(c));//Linear
                    u   = General::linearTodB(vecAssignment.at(c)->getXTReal());
                }
                else{
                    return false;
                }
            }
            assignment->setXTVec(route,modulationPos,pos,XTtotal);//Linear
            //Topology::printRouteState(assignment->getTrialRoute(route),assignment->getNumSlotsMatrix(route,modulationPos));
            return true;
        }
    }
    else{
        //int f  = assignment->getTempCore(route,modulationPos,pos);
        //Topology::printRouteState(assignment->getTrialRoute(route),assignment->getNumSlotsMatrix(route,modulationPos));
        return false;
    }
}
//
bool Modulation::getXT(Assignment* assignment,int s,int core,Route* route,int numSlots,int modulacao){
float h = 0,k,beta,R,A;
float xtMed = 0;
vector<float> reqXt,vecXt;
vector<int> hopVec,slotVec, slotVecReq;
vector<Core*> coreVec;
vector<Fiber*> fiberVec,fiberVecReq;
vector<float> slotXT;
double L;
bool interfere = false;
int ori,de,iniSlot = s,adjCoreIndex,index;
Core* adjCore;
int numHops = route->getNumHops(),nucleo = core;
Fiber* fiber;
Link* link;

//coeficiente de acoplamento
k =  float(0.012);//(0.012); //0.00584;
//constante de propagação
beta = pow(10,7);//7
//raio de curvatura
R = double(0.01);
//core pitch
A = double(4.5*pow(10,-5));//4,5
//
h = double((2*pow(k,2)*R)/(beta*A));

    for(int c = 0;c < numHops;c++){
        ori = route->getNode(c);de = route->getNode(c + 1);
        fiber = Topology::getFiber(ori,de);
        link = Topology::getLink(ori,de);
        L = link->getLength()*200;
        //Variation of adj cores
        for(int adj = 0;adj < fiber->getFiberCore(nucleo)->getNumAdjCores();adj++){
            adjCoreIndex = fiber->getFiberCore(nucleo)->getAdjCoreIndex(adj);
            adjCore = fiber->getFiberCore(adjCoreIndex);
            //índice de interferencia-Verificar quantas conexões se sobrepoem
            for(int slot = iniSlot;slot < (iniSlot + numSlots - 1);slot++){
                    //Vetor com o XT dos slots da requisição
                    if(adj == 0){
                        reqXt.push_back(0);
                        slotVecReq.push_back(slot);
                        fiberVecReq.push_back(fiber);
                        index = reqXt.size() - 1;
                    }
                    //Condição para impedir que a banda de guarda seja contada
                    if(fiber->getCoreOccupation(slot,adjCore) && fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot)->getLastSlot() != slot){
                            //Topology::printRouteState(route,numSlots,nucleo,s);
                            interfere = true;
                            float xt = (1 - exp(-2*h*L))/(1 + exp(-2*h*L)),xt_adj;
                            //VERIFICAR XT DAS CONEXÕES ADJ
                            if(fiber->getFiberCore(adjCoreIndex)->getSlotXT(slot) != 0){
                                xt_adj = xt + fiber->getFiberCore(adjCoreIndex)->getSlotXT(slot);
                            }
                            else
                                xt_adj = xt;
                            xt_adj = General::linearTodB(xt_adj);
                            //Debug
                            Assignment* u;
                            float o = fiber->getFiberCore(adjCoreIndex)->getSlotXT(slot),b = fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot)->getXTLimit();
                            if(o < 0){
                                u = fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot);
                            }
                            //XTmed += xt;
                            //Possível alocação faz o XT da conexão interferida passar do limite?
                            //VERIFICAR ATRIBUIÇÃO DE XT DE SLOTS ADJ!!!!!!!!!!!!!!!!!!!!!!!!!
                            if(xt_adj > fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot)->getXTLimit()){
                                //Sim,retorna FALSE
                                return false;
                            }
                            else{
                                //Informações salvas referentes ao slot interferente,precisam ser guardadas para atualizar o XT ao final do processo, caso a alocação seja feita
                                fiberVec.push_back(fiber);
                                coreVec.push_back(adjCore);
                                slotVec.push_back(slot);
                                vecXt.push_back(General::dBtoLinear(xt_adj));
                                //Atualiza o XT da possível alocação,verificar se o slot já foi adicionado
                                bool existe = false;
                                for(int a = 0;a < slotVecReq.size();a++){
                                    if(slotVecReq.at(a) == slot && fiberVecReq.at(a) == fiber){
                                        reqXt.at(a) += xt;
                                        float p = General::linearTodB(reqXt.at(a)), m = Modulation::getXTLimit(modulacao,assignment->getBitRate(),Modulation::getOutagePb(),Traffic::getBER());
                                        //Caso o Xt da possível alocação esteja fora do limite, a alocação não é possível para esta modulação
                                        if(General::linearTodB(reqXt.at(a))  > Modulation::getXTLimit(modulacao,assignment->getBitRate(),Modulation::getOutagePb(),Traffic::getBER()))
                                            return false;
                                        existe = true;
                                        break;
                                    }
                                }
                                if(!existe){
                                    reqXt.at(index) = xt;
                                    //Caso o Xt da possível alocação esteja fora do limite, a alocação não é possível para esta modulação
                                    if(General::linearTodB(reqXt.at(index))  > Modulation::getXTLimit(modulacao,assignment->getBitRate(),Modulation::getOutagePb(),Traffic::getBER()))
                                        return false;
                                    slotVecReq.at(index) = slot;
                                }
                            }

                    }
            }

        }
    }
    if(!interfere){
        //Topology::printRouteState(route,numSlots,nucleo,s);
        for(int c = 0;c < numHops;c++){
            for(int d = iniSlot;d <  (iniSlot + numSlots - 1);d++){
                int ori = route->getNode(c);de = route->getNode(c + 1);
                Fiber* fiber = Topology::getFiber(ori,de);
                fiber->getFiberCore(nucleo)->setSlotXT(d,0);
            }
        }
        assignment->setXTLimit(Modulation::getXTLimit(modulacao,assignment->getBitRate(),Modulation::getOutagePb(),Traffic::getBER()));
        assignment->setRoute(route);
        return true;
    }
    //Caso chegue aqui, houve interferência e nenhum slot ultrapassou seu limite de XT,seta o XT real de todos os slots envolvidos
    //Atualiza o XT dos slots interferentes
    //Topology::printRouteState(route,numSlots,nucleo,s);
    for(int num = 0;num <= slotVec.size() - 1;num++){
        Core*  c  = coreVec.at(num);
        int    e = slotVec.at(num),b = c->getSlotAssignment(e)->getLastSlot();
        float  x = vecXt.at(num),l;
        if(x <= 0){
            int t = 0;
        }
        c->setSlotXT(e,x);
        //Topology::xtMed = Topology::xtMed + x;
    }
    //Atualiza o XT dos slots da própria requisição
    int cont = 0;
    //Topology::printRouteState(route,numSlots,nucleo,s);
    for(int num = 0;num < numHops;num++){
            for(int f = iniSlot;f < (iniSlot + numSlots - 1);f++){
                int ori = route->getNode(num);de = route->getNode(num + 1);
                Fiber* fiber = Topology::getFiber(ori,de);
                float y;
                y = reqXt.at(cont);
                if(y <= 0){
                    int r = 0;
                }
                y = General::linearTodB(y);
                fiber->getFiberCore(nucleo)->setSlotXT(f,reqXt.at(cont));
                //Topology::xtMed = Topology::xtMed + reqXt.at(cont);
                cont++;
            }
    }
    assignment->setXTLimit(Modulation::getXTLimit(modulacao,assignment->getBitRate(),Modulation::getOutagePb(),Traffic::getBER()));
    assignment->setRoute(route);
    return true;
}

bool Modulation::getXT(Assignment* assignment,int route,int pos,bool greedy,int modulationPos){
float h = 0,k,beta,R,A, limite = Modulation::getXTLimit(assignment->getModulationMatrix(route,modulationPos),assignment->getBitRate(),Modulation::getOutagePb(),Traffic::getBER());
float xtMed = 0;
vector<float> reqXt,vecXt;
vector<int> hopVec,slotVec, slotVecReq;
vector<Core*> coreVec;
vector<Fiber*> fiberVec;
vector<float> slotXT;
double L;
bool interfere = false;
int ori,de,iniSlot = assignment->getTempSlot(route,modulationPos,pos),adjCoreIndex,index;
Core* adjCore;
int numHops = assignment->getTrialRoute(route)->getNumHops(),nucleo = assignment->getTempCore(route,modulationPos,pos);
Fiber* fiber;
Link* link;
//coeficiente de acoplamento
k =  float(0.012); //0.00584;
//constante de propagação
beta = pow(10,7);
//raio de curvatura
R = double(0.01);
//core pitch
A = double(4.5*pow(10,-5));
//
h = double((2*pow(k,2)*R)/(beta*A));
//DEBUG
//if(nucleo == 1 && iniSlot == 205)
//     Topology::printRouteState(assignment->getTrialRoute(route),assignment->getNumSlotsMatrix(route,modulationPos));

    for(int c = 0;c < numHops;c++){
        ori = assignment->getTrialRoute(route)->getNode(c);de = assignment->getTrialRoute(route)->getNode(c + 1);
        fiber = Topology::getFiber(ori,de);
        link = Topology::getLink(ori,de);
        L = link->getLength();
        //Variation of adj cores
        for(int adj = 0;adj < fiber->getFiberCore(assignment->getTempCore(route,modulationPos,pos))->getNumAdjCores();adj++){
            //Topology::printRouteState(assignment->getTrialRoute(route),assignment->getNumSlotsMatrix(route,modulationPos));
            adjCoreIndex = fiber->getFiberCore(assignment->getTempCore(route,modulationPos,pos))->getAdjCoreIndex(adj);
            adjCore = fiber->getFiberCore(adjCoreIndex);
            //índice de interferencia-Verificar quantas conexões se sobrepoem
            for(int slot = iniSlot;slot < (iniSlot + assignment->getNumSlotsMatrix(route,modulationPos) - 1);slot++){
                    //Vetor com o XT dos slots da requisição
                    reqXt.push_back(0);
                    slotVecReq.push_back(Def::MAX_INT);
                    index = reqXt.size() - 1;
                    //Condição para impedir que a banda de guarda seja contada
                    if(fiber->getCoreOccupation(slot,adjCore) && fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot)->getLastSlot() != slot){
                            interfere = true;
                            float xt = (1 - exp(-2*h*L))/(1 + exp(-2*h*L));
                            if(fiber->getFiberCore(adjCoreIndex)->getSlotXT(slot) != 0)
                                xt += fiber->getFiberCore(adjCoreIndex)->getSlotXT(slot);
                            xt = General::linearTodB(xt);
                            //Debug
                            float a = fiber->getFiberCore(adjCoreIndex)->getSlotXT(slot),b = fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot)->getXTLimit();
                            //XTmed += xt;
                            //Possível alocação faz o XT da conexão interferida passar do limite?
                            if(xt > fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot)->getXTLimit()){
                                //Sim,retorna FALSE
                                //Topology::printRouteState(assignment->getTrialRoute(route),assignment->getNumSlotsMatrix(route,modulationPos));
                                return false;
                            }
                            else{
                                //Informações salvas referentes ao slot interferente,precisam ser guardadas para atualizar o XT ao final do processo, caso a alocação seja feita
                                fiberVec.push_back(fiber);
                                coreVec.push_back(adjCore);
                                slotVec.push_back(slot);
                                vecXt.push_back(General::dBtoLinear(xt));
                                //Atualiza o XT da possível alocação,verificar se o slot já foi adicionado
                                bool existe = false;
                                for(int a = 0;a < slotVecReq.size() - 1;a++){
                                    if(slotVecReq.at(a) == slot){
                                        reqXt.at(a) += General::dBtoLinear(xt);
                                        existe = true;
                                        break;
                                    }
                                }
                                if(!existe){
                                    slotVecReq.at(index) = slot;
                                    reqXt.at(index) = General::dBtoLinear(xt);
                                }
                            }

                    }
            }

        }
    }
    //Verificar se XTTotal == 0 e retornar true
    if(!interfere){
        for(int c = 0;c < numHops;c++){
            for(int s = iniSlot;s <  (iniSlot + assignment->getNumSlotsMatrix(route,modulationPos) - 1);s++){
                int ori = assignment->getTrialRoute(route)->getNode(c);de = assignment->getTrialRoute(route)->getNode(c + 1);
                Fiber* fiber = Topology::getFiber(ori,de);
                fiber->getFiberCore(nucleo)->setSlotXT(s,0);
            }
        }
        //Topology::printRouteState(assignment->getTrialRoute(route),assignment->getNumSlotsMatrix(route,modulationPos));
        assignment->setXTVec(route,modulationPos,pos,0);//Linear
        return true;
    }
    //Caso chegue aqui, houve interferência e nenhum slot ultrapassou seu limite de XT,seta o XT real de todos os slots envolvidos
    //Atualiza o XT dos slots interferentes
    for(int num = 0;num < slotVec.size() - 1;num++){
        //Fiber* f = fiberVec.at(a);
        Core*  c  = coreVec.at(num);
        int    s = slotVec.at(num);
        float  x = vecXt.at(num);
        c->setSlotXT(s,x);

        //Topology::xtMed = Topology::xtMed + x;
    }
    //Atualiza o XT dos slots da própria requisição
    int cont = 0;

    for(int num = 0;num < numHops;num++){
            for(int s = iniSlot;s < (iniSlot + assignment->getNumSlotsMatrix(route,modulationPos) - 1);s++){
                int ori = assignment->getTrialRoute(route)->getNode(num);de = assignment->getTrialRoute(route)->getNode(num + 1);
                Fiber* fiber = Topology::getFiber(ori,de);
                fiber->getFiberCore(nucleo)->setSlotXT(s,reqXt.at(cont));
                //Topology::xtMed = Topology::xtMed + reqXt.at(cont);
                cont++;
            }
    }
    return true;
}


bool Modulation::setInterConnectionsXT(Assignment* assignment,int numRoutes,int mod){
float h,k,beta,R,A,Pxt,XTmed = 0,XTtotal = 0,melhorXT = 1000000,Pj;
vector <Assignment*> melhorVecAssignment, vecAssignmentAtual;
vector <float> melhorVecXt, vecXtAtual;
vector <float> intIndex;
double L, ber = Traffic::getBER();
int ori,iniSlot,adjCoreIndex,finalRouteIndex = Def::MAX_INT,finalAlocIndex = Def::MAX_INT,finalModIndex = Def::MAX_INT;
Core* adjCore;
int de,numHops;
Fiber* fiber;
Link* link;
//coeficiente de acoplamento
k = 0.012;//0.00584;//0.012
//constante de propagação
beta = pow(10,7);
//raio de curvatura
R = 0.01;
//core pitch
A = 4.5*pow(10,-5);
        //Variation of the routes
        for(int aloc = 0;aloc < numRoutes;aloc++){
            for(int modulacao = 0;modulacao < mod;modulacao++){
                if(assignment->getModulationMatrix(aloc,modulacao) != 1){
                    //Cálculos apenas para a primeira rota que passa pela OSNR e possui capacidade
                    //Variação das possiveis alocações
                    for(int p = 0;p < assignment->getTempCoreSize(aloc,modulacao);p++){
                        //Possível Alocação não passou pelos critérios anteriores
                        if(assignment->getTempCore(aloc,modulacao,p) == Def::MAX_INT){
                            continue;
                        }
                        numHops = assignment->getTrialRoute(aloc)->getNumHops();
                        iniSlot = assignment->getTempSlot(aloc,modulacao,p);
                        //Variation of the routes' links
                        for(int c = 0;c < numHops;c++){
                            ori = assignment->getTrialRoute(aloc)->getNode(c);de = assignment->getTrialRoute(aloc)->getNode(c + 1);
                            fiber = Topology::getFiber(ori,de);
                            link = Topology::getLink(ori,de);
                            //Variation of adj cores
                            for(int adj = 0;adj < fiber->getFiberCore(assignment->getTempCore(aloc,modulacao,p))->getNumAdjCores();adj++){
                                Assignment* antAssignment = NULL;
                                adjCoreIndex = fiber->getFiberCore(assignment->getTempCore(aloc,modulacao,p))->getAdjCoreIndex(adj);
                                adjCore = fiber->getFiberCore(adjCoreIndex);
                                int position = Def::MAX_INT;
                                //índice de interferencia-Verificar quantas conexões se sobrepoem
                                for(int slot = iniSlot;slot < (iniSlot + assignment->getNumSlotsVec(aloc) - 1) ;slot++){
                                        if(fiber->getCoreOccupation(slot,adjCore) && adjCore->getSlotAssignment(slot)->getLastSlot() != slot){//Verifica se não é o slot da banda de guarda
                                                if(fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot) != antAssignment){
                                                    bool existe = false;
                                                    //Verificar se o assignment interferente está no vetor
                                                    for(int a = 0;a < vecAssignmentAtual.size();a++){
                                                        if(fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot) == vecAssignmentAtual.at(a)){
                                                        //Caso o assignment interferente esteja no vetor
                                                            existe = true;
                                                            intIndex.at(a)++;
                                                            position = a;
                                                            antAssignment = vecAssignmentAtual.at(a);
                                                            break;
                                                        }
                                                    }
                                                    if(!existe){
                                                        intIndex.push_back(1);
                                                        position = intIndex.size() - 1;
                                                        vecAssignmentAtual.push_back(fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot));
                                                        vecXtAtual.push_back(fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot)->getXTReal());
                                                        antAssignment = fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot);
                                                    }
                                                }
                                                else if(fiber->getFiberCore(adjCoreIndex)->getSlotAssignment(slot) == antAssignment){
                                                    intIndex.at(position)++;
                                                }
                                        }

                                }
                                //Calcular o tamanho da interferencia da possivel alocação em um dos nucleos adjacentes
                                for(int d = 0;d < intIndex.size();d++){
                                    if(intIndex.at(d) > 0){
                                    //Conexão afetada pela possível alocação
                                            intIndex.at(d) = intIndex.at(d)/(assignment->getNumSlotsVec(aloc) - 1);
                                            Pj = assignment->getPowerVec(aloc);
                                            h = 2*pow(k,2)*R/(beta*A);
                                            //cálculo da potência de XT
                                            Pxt = intIndex.at(d)*Pj*h*L;
                                            //potência média de XT no link
                                            int q = vecAssignmentAtual.size();
                                            XTmed += Pxt/(vecAssignmentAtual.at(d)->getPower());
                                            //Verificar se este valor XT deixa a conexão fora do limite, se sim a possível alocação NÃO é uma possibilidade
                                            vecXtAtual.at(d) = vecXtAtual.at(d) + XTmed;
                                            if(General::linearTodB(vecXtAtual.at(d)) > vecAssignmentAtual.at(d)->getXTLimit()){
                                                //Possível alocação ultrapassa o limite de XT de alguma conexão
                                                assignment->setTempCore(aloc,modulacao,p,Def::MAX_INT);
                                                assignment->setTempSlot(aloc,modulacao,p,Def::MAX_INT);
                                                XTmed = 0;
                                                break;
                                            }
                                            XTmed = 0;
                                   }
                                }
                                //Possivel alocação NAO atinge os valores de XT,sair do Loop de núcleos adjacentes
                                if(assignment->getTempCore(aloc,modulacao,p) == Def::MAX_INT){
                                    break;
                                }
                                //Resetar o vetor de intIndex para a identifiicação de conexões interferentes no próximo núcleo adjacente
                                for(int a = 0;a < intIndex.size();a++){
                                    intIndex.at(a) = 0;
                                }
                            }
                            //Possivel alocação NAO atinge os valores de XT,sair do Loop de hops
                            if(assignment->getTempCore(aloc,modulacao,p) == Def::MAX_INT){
                                 break;
                            }
                        //Proximo hop
                        }
                        //Possivel alocação NAO atinge os valores de XT,ir para a próxima possível alocação
                        if(assignment->getTempCore(aloc,modulacao,p) == Def::MAX_INT){
                             vecAssignmentAtual.clear();
                             vecXtAtual.clear();
                             intIndex.clear();
                             continue;
                        }
                        float xtTotal = 0;
                        //Diferença XT da possível alocação
                        //VERIFICAR CALCULOS EM LINEAR OU DB
                        xtTotal = General::dBtoLinear(Modulation::getXTLimit(assignment->getModulationMatrix(aloc,modulacao),assignment->getBitRate(),Modulation::getOutagePb(),ber)) - assignment->getXTVec(aloc,modulacao,p);//Linear
                        //Ao fim do cálculo da interferência da possível alocação nas conexões em núcleos adjacentes,subtrair as diferenças do limit e do xt real,somar e dividir pelo toatal de conexões afetadas
                        for(int b = 0;b < vecAssignmentAtual.size();b++){
                            xtTotal += General::dBtoLinear(vecAssignmentAtual.at(b)->getXTLimit()) - vecXtAtual.at(b);//Linear
                        }
                        xtTotal = xtTotal/(vecAssignmentAtual.size() + 1);
                        assignment->setXTMed(aloc,modulacao,p,xtTotal);
                        //Comparar o xtTotal da possível alocação com o melhor xt até aqui
                        if(xtTotal < melhorXT){
                            melhorXT = xtTotal;
                            finalAlocIndex = p;
                            finalRouteIndex = aloc;
                            finalModIndex = modulacao;
                            //Atualiza melhorVecAssignment e melhorVecXt
                            melhorVecAssignment.clear();
                            melhorVecXt.clear();
                            for(int a = 0;a < vecAssignmentAtual.size();a++){
                                melhorVecAssignment.push_back(vecAssignmentAtual.at(a));
                                melhorVecXt.push_back(vecXtAtual.at(a));//Linear
                            }
                        }
                        //Reseta vecAssignmentAtaul e vecXtAtual para a próxima possibilidade de alocação
                        vecAssignmentAtual.clear();
                        vecXtAtual.clear();
                        intIndex.clear();
                    }
                //Próximo formato de modulação
                }
        //Próxima rota
        }
        //Todas as possibilidades de alocação calculadas
        if(melhorXT == 1000000){
        //Nenhuma alocação atende aos critérios de XT,BLOQUEIO!
            return false;
        }
        //Atualizar o XT das conexões afetadas pela alocação
        for(int c = 0;c < melhorVecAssignment.size();c++){
                melhorVecAssignment.at(c)->setXTReal(melhorVecXt.at(c));
        }
       //Seta o XT da conexão e o limite de XT
       Topology::numReqPerCore.at(assignment->getTempCore(finalRouteIndex,finalModIndex,finalAlocIndex))++;
       Topology::numReqPerRoute.at(finalRouteIndex)++;
       assignment->setXTReal(assignment->getXTVec(finalRouteIndex,finalModIndex,finalAlocIndex));
       assignment->setPower(assignment->getPowerVec(finalRouteIndex));
       assignment->setXTLimit(Modulation::getXTLimit(assignment->getModulationMatrix(finalRouteIndex,finalModIndex),assignment->getBitRate(),Modulation::getOutagePb(),ber));
       //assignment->setOSNRth(Modulation::getOSNRLimit(assignment->getModulation(finalRouteIndex), Traffic::getBER(),assignment->getBitRate()));
       //Seta nucleo,slots,numero slots e rota
       assignment->setCoreId(assignment->getTempCore(finalRouteIndex,finalModIndex,finalAlocIndex));
       assignment->setFirstSlot(assignment->getTempSlot(finalRouteIndex,finalModIndex,finalAlocIndex));
       assignment->setNumSlots(assignment->getNumSlotsMatrix(finalRouteIndex,finalModIndex));
       assignment->setLastSlot(assignment->getTempSlot(finalRouteIndex,finalModIndex,finalAlocIndex) + assignment->getNumSlots() - 1);//Banda de guarda
       assignment->setRoute(assignment->getTrialRoute(finalRouteIndex));
       return true;
    }
}


void Modulation::setOutagePb(double outage){
    outagePb = outage;
}

double Modulation::getOutagePb(){
    return outagePb;
}
