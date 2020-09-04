#ifndef AlgoritmoGenetico_H
#define AlgoritmoGenetico_H

#include "Kernel.h"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
using namespace std;

class Individuo{
public:
    void setFitness(long double);
    long double getFitness();
    vector< vector<int> > combinacoes;
    int cont_sim;
private:

    long double fitness;
};

class AlgoritmoGenetico{
public:
     static void InicializaAG();
     //GA Without random elements,with CrossOver Probability and Mutation Probability
     static void AG_1();
     //Classical GA, double population with C.O and later with Mutation
     static void AG_2();
     static void TorneioAG();
     static void GeraAleatorios(int numeros[],int ,int);
     static bool Existe(int valores[],int tam, int valor);
     static int  FuncaoComparacao(const void* a,const void* b);
     static bool Compara(Individuo* ,Individuo* );
     static bool ComparaPior(Individuo* ,Individuo* );
     static void SelecionaMelhores(bool);
     static void InicializaPopulacao();
     static void SelecaoRoleta(vector<int>& indice_pais,bool process,bool);
     static int  SelecaoRoleta_Torneio();
     static void Mutacao(Individuo* ,Individuo*);
     static void Mutacao_1(Individuo* ,Individuo* ,int);
     static void Cruzamento(Individuo* , Individuo* , Individuo* , Individuo*,int );
     static Individuo* GetPopulationElement(int);
     static Individuo* GetPopulationAuxElement(int);
     static void GeraTodos();
     static Individuo* GetIndividuo(int);
     static void Imprime_Individuo_GeraTodos(Individuo*);
     static void Imprime_Pop_Final();
     static int Rand_Sel();
     static void Torneio();
     static void Save_BestWorst(int);
     static void Print_Log();
     static void Ressim(int,int,int);

private:
    static int tam_genes; 		// quantidade de genes - numero de nucleos
    static int tam_pop; 		// quantidade de indivíduos da população
    static int geracoes; 	// quantidade de gerações
    static int num_tam_req; 	//quantidade de requisições de tamanhos diferentes
    static int rand_sel;    //indivíduos selecionados randomicamente para os próximos processos
    static int mutacao_prob;
    static int crossover_prob;
    static int ressim;
    static int ressim_indiv;
    static vector<Individuo*> populacao,populacao_Aux,pop_Mut;
    static vector<Individuo*> todosIndividuos;
};

#endif
