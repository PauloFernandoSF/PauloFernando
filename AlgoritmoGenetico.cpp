#include "AlgoritmoGenetico.h"
// Algoritmos Genéticos
#include <iomanip>

// parâmetros do algoritmo genético
int AlgoritmoGenetico::tam_genes = 6; 		// quantidade de genes - numero de nucleos
int AlgoritmoGenetico::tam_pop = 100; 		// quantidade de indivíduos da população
int AlgoritmoGenetico::geracoes = 50;     // quantidade de gerações
int AlgoritmoGenetico::num_tam_req = 3; 	//quantidade de requisições de tamanhos diferentes
int AlgoritmoGenetico::rand_sel = 3;        //número de indivíduos selecionados aleatoreamente
int AlgoritmoGenetico::mutacao_prob = 15;    //probabilidade de mutação dos genes
int AlgoritmoGenetico::crossover_prob = 90;
int AlgoritmoGenetico::ressim = 3;
int AlgoritmoGenetico::ressim_indiv = 3;
long double Melhor_Pb = 0,Pior_Pb = 0;

vector<Individuo*> AlgoritmoGenetico::populacao(0),AlgoritmoGenetico::populacao_Aux(0),AlgoritmoGenetico::pop_Mut(0);
vector<Individuo*> AlgoritmoGenetico::todosIndividuos(0);


//Funções para os individuos
void Individuo::setFitness(long double fit){
    fitness = fit;
}

long double Individuo::getFitness(){
    return fitness;
}
////////////////////////////////////////////
//Funções para o Algoritmo Genetico
void AlgoritmoGenetico::TorneioAG(){
    Print_Log();
    //Initial Random Population with tam_pop individuals
    InicializaPopulacao();
    //Select one individual through roullete whell process
    Torneio();

    Imprime_Pop_Final();

}

void AlgoritmoGenetico::Torneio(){
int indice_pais_1,indice_pais_2;
long double mu,p;
    populacao_Aux.push_back(NULL);
    for(int g = 0;g < geracoes;g++){
        indice_pais_1 = SelecaoRoleta_Torneio();
        Individuo *pai = populacao.at(indice_pais_1),*filho_MU = new Individuo();
        filho_MU->combinacoes.resize(num_tam_req,vector<int>(tam_genes));
        Mutacao(pai,filho_MU);
        indice_pais_2 = Rand_Sel();
        mu = filho_MU->getFitness();
        if(filho_MU->getFitness() <= populacao.at(indice_pais_2)->getFitness())
            populacao.at(indice_pais_2) = filho_MU;
        sort(populacao.begin(),populacao.end(),Compara);
        Save_BestWorst(g);
    }
}

void AlgoritmoGenetico::Save_BestWorst(int g){
Melhor_Pb = populacao.at(populacao.size() - 1)->getFitness();
Pior_Pb = populacao.at(0)->getFitness();
        if(g > 9){
                InpOut::GA_Better_Worst<<fixed << setprecision(7);
                InpOut::GA_Better_Worst<< g << " " << Melhor_Pb <<" "<< Pior_Pb <<endl;
        }
        else{
                InpOut::GA_Better_Worst<<fixed << setprecision(7);
                InpOut::GA_Better_Worst<< g << "  " << Melhor_Pb <<" "<< Pior_Pb <<endl;
        }
}

void AlgoritmoGenetico::InicializaAG(){

Print_Log();

InicializaPopulacao();

AG_1();

}

void AlgoritmoGenetico::Print_Log(){

    InpOut::GA_Log<< "Generations"  <<":    "<< geracoes                <<endl;
    InpOut::GA_Log<< "Population"   <<":     "<< tam_pop                 <<endl;
    InpOut::GA_Log<< "Requisitions" <<":   "<< num_tam_req             <<endl;
    InpOut::GA_Log<< "Cores(genes)" <<":   "<< tam_genes               <<endl;
    InpOut::GA_Log<< "Mutation"     <<":       "<< mutacao_prob            <<endl;
    InpOut::GA_Log<< "CrossOver"    <<":      "<< crossover_prob          <<endl;
    InpOut::GA_Log<< "Random"       <<":         "<< rand_sel                <<endl;
    InpOut::GA_Log<< "Slos/Core"    <<":      "<< Topology::getNumSlots() <<endl;
    InpOut::GA_Log<< "Resim_Times"  <<":    "<<ressim  <<endl;
    InpOut::GA_Log<< "Resim_Indiv"  <<":    "<<ressim_indiv  <<endl;

}

void AlgoritmoGenetico::AG_1(){

vector<int> indice_pais(2);

     for(int i = 0; i < geracoes; i++){
				  int t = populacao.size(), y = populacao_Aux.size();
				  for(unsigned int a = 0;a < tam_pop;a++){
                        populacao_Aux.push_back(NULL);
                        populacao_Aux.at(a) = populacao.at(a);
				  }
				  //Each individual has a chance to C.O
				  for(int a = 0;a < tam_pop;a++){
                        int crossover = rand() % 100;

                        if(crossover < crossover_prob){
                        //metodo de seleção roleta para os pais
                            populacao_Aux.push_back(NULL);
                            populacao_Aux.push_back(NULL);
                            SelecaoRoleta(indice_pais,0,true);
                            //Individuo *pai1 = populacao.at(indice_pais.at(0)),*filho_1_CO = new Individuo();
                            Individuo *pai1 = populacao.at(a),*filho_1_CO = new Individuo();
                            Individuo *pai2 = populacao.at(indice_pais.at(1)),*filho_2_CO = new Individuo();
                            filho_1_CO->combinacoes.resize(num_tam_req,vector<int>(tam_genes));
                            filho_2_CO->combinacoes.resize(num_tam_req,vector<int>(tam_genes));
                        //aplica o cruzamento de 1 ponto e atualiza populaçao com os dois novos filhos
                        //simular e atribuir a Pb para os dois filhos
                            Cruzamento(pai1, pai2, filho_1_CO, filho_2_CO,a);
                        }
                 }

                int aux_size = populacao_Aux.size();
                vector<Individuo*> p = populacao_Aux;
                sort(populacao_Aux.begin(),populacao_Aux.end(),Compara);

				//Rotina para gerar 50 individuos por Mutacao
				for(int b = 0;b < aux_size;b++){
				//for(int b = 0;b < tam_pop;b++){
                    int mutation = rand() % 100;
                    if(mutation < mutacao_prob){
                        //metodo de seleção roleta para os pais
                        //SelecaoRoleta(indice_pais,1);
                        populacao_Aux.push_back(NULL);
                        Individuo *pai1 = populacao_Aux.at(b);
                        //Individuo *pai2 = populacao.at(indice_pais.at(1));
                        Individuo* filho_1_PERM = new Individuo();//* filho_2_PERM = new Individuo();
                        filho_1_PERM->combinacoes.resize(num_tam_req,vector<int>(tam_genes));
                        //filho_2_PERM->combinacoes.resize(num_tam_req,vector<int>(tam_genes));
                        Mutacao_1(pai1, filho_1_PERM, b);//com prob_mut
                    }
				}
				SelecionaMelhores(true);
				//Imprime o melhor e o pior de cada geração
				if(i > 9){
                        InpOut::GA_Better_Worst<<fixed << setprecision(7);
                        //InpOut::GA_Better_Worst<< "Geração " << i << " - " << "Melhor Pb = " << Melhor_Pb <<" Pior Pb = " << Pior_Pb <<endl;
                        InpOut::GA_Better_Worst<< i << " " << Melhor_Pb <<" "<< Pior_Pb <<endl;
            	}
				else{
                        InpOut::GA_Better_Worst<<fixed << setprecision(7);
                        //InpOut::GA_Better_Worst<< "Geração " << i << "  - " << "Melhor Pb = " << Melhor_Pb << " Pior Pb = " << Pior_Pb <<endl;
                        InpOut::GA_Better_Worst<< i << "  " << Melhor_Pb <<" "<< Pior_Pb <<endl;
                }
				///////////////////////////////////////////
       }
       Imprime_Pop_Final();
}

void AlgoritmoGenetico::AG_2(){
vector<int> indice_pais(2);

     for(int i = 0; i < geracoes; i++){
				  for(unsigned int a = 0;a < tam_pop;a++){
                        populacao_Aux.push_back(NULL);
                        populacao_Aux.at(a) = populacao.at(a);
				  }
				  //Each individual has a chance to C.O
				  for(int a = 0;a < tam_pop/2;a++){
                        populacao_Aux.push_back(NULL);
                        populacao_Aux.push_back(NULL);
                        SelecaoRoleta(indice_pais,0,true);
                        Individuo *pai1 = populacao.at(indice_pais.at(0)),*filho_1_CO = new Individuo();
                        Individuo *pai2 = populacao.at(indice_pais.at(1)),*filho_2_CO = new Individuo();
                        filho_1_CO->combinacoes.resize(num_tam_req,vector<int>(tam_genes));
                        filho_2_CO->combinacoes.resize(num_tam_req,vector<int>(tam_genes));
                        //aplica o cruzamento de 1 ponto e atualiza populaçao com os dois novos filhos
                        //simular e atribuir a Pb para os dois filhos
                        Cruzamento(pai1, pai2, filho_1_CO, filho_2_CO,a);
                 }
                //Pass crossover individuals to pop_Mut in order to operate the mutations
				for(unsigned int a = populacao.size();a < populacao_Aux.size() - 1;a++){
                            pop_Mut.push_back(populacao_Aux.at(a));
				}
				//Rotina para gerar 50 individuos por Mutacao
				for(int b = 0;b < tam_pop;b++){
                        //metodo de seleção roleta para os pais
                        populacao_Aux.push_back(NULL);
                        SelecaoRoleta(indice_pais,1,true);
                        Individuo *pai1 = populacao.at(indice_pais.at(0));
                        Individuo* filho_1_PERM = new Individuo();
                        filho_1_PERM->combinacoes.resize(num_tam_req,vector<int>(tam_genes));
                        Mutacao_1(pai1, filho_1_PERM, b);
                }

                //Escolhe os melhores 40 individuos e os outros 10 são escolhidos aleatoriamente e transferidos para populacao
				SelecionaMelhores(true);
				//Imprime o melhor e o pior de cada geração
				if(i > 9){
                        InpOut::GA_Better_Worst<<fixed << setprecision(7);
                        //InpOut::GA_Better_Worst<< "Geração " << i << " - " << "Melhor Pb = " << Melhor_Pb <<" Pior Pb = " << Pior_Pb <<endl;
                        InpOut::GA_Better_Worst<< i << " " << Melhor_Pb <<" "<< Pior_Pb <<endl;
            	}
				else{
                        InpOut::GA_Better_Worst<<fixed << setprecision(7);
                        //InpOut::GA_Better_Worst<< "Geração " << i << "  - " << "Melhor Pb = " << Melhor_Pb << " Pior Pb = " << Pior_Pb <<endl;
                        InpOut::GA_Better_Worst<< i << "  " << Melhor_Pb <<" "<< Pior_Pb <<endl;
                }
				///////////////////////////////////////////
       }
       Imprime_Pop_Final();
}

void AlgoritmoGenetico::GeraAleatorios(int numeros[],int quantNumeros,int Limite){
    srand(time(NULL));
    int v;
    for(int i=0;i<quantNumeros;i++){
        v = rand() % Limite;
        while(Existe(numeros,i,v)){
            v = rand() % Limite;
        }
        numeros[i] = v;
    }

}

bool AlgoritmoGenetico::Existe(int valores[],int tam, int valor){
    for(int i = 0;i<tam;i++){
        if(valores[i]==valor)
            return true;
    }
    return false;
}

int AlgoritmoGenetico::FuncaoComparacao(const void* a,const void* b){
	Individuo* elemento_1 = (Individuo*)a;
	Individuo* elemento_2 = (Individuo*)b;
	if(elemento_1->getFitness() > elemento_2->getFitness())
		return +1;
	if(elemento_1->getFitness() < elemento_2->getFitness())
		return -1;
	else
		return 0;

}

bool AlgoritmoGenetico::Compara(Individuo* a,Individuo* b){
	return (a->getFitness() > b->getFitness());
}

bool AlgoritmoGenetico::ComparaPior(Individuo* a,Individuo* b){
	return (a->getFitness() < b->getFitness());
}

void AlgoritmoGenetico::SelecionaMelhores(bool BestWorst){
int numeros[rand_sel], limite;
int g = populacao_Aux.size();
//If BestWorst - normal GA,otherwise GA to maximize Pb
if(BestWorst)
    sort(populacao_Aux.begin(),populacao_Aux.end(),Compara);
else
    sort(populacao_Aux.begin(),populacao_Aux.end(),ComparaPior);
///////////////////////////////////////////////////////
    //simular novamente para os ressim_indiv melhores individuos
    for(int t = populacao_Aux.size() - 1;t >= populacao_Aux.size() - ressim_indiv;t--){
        //casos em q haverá resimulação
        if(populacao_Aux.at(t)->cont_sim <= ressim){
            Ressim(1,t,1);
        }
    }
    /////////////////////////////////////////////////
    //If BestWorst - normal GA,otherwise GA to maximize Pb
    if(BestWorst)
        sort(populacao_Aux.begin(),populacao_Aux.end(),Compara);
    else
        sort(populacao_Aux.begin(),populacao_Aux.end(),ComparaPior);
    ///////////////////////////////////////////////////////
    Melhor_Pb = populacao_Aux.at(populacao_Aux.size() - 1)->getFitness();
    Pior_Pb = populacao_Aux.at(0)->getFitness();
    //SELECTION OF BETTER INDIVIDUALS
	for(int a = rand_sel;a < tam_pop;a++){
		populacao.at(a) = populacao_Aux.at(populacao_Aux.size() + (a - tam_pop));
	}
	//RANDOM SELECTION
	limite = populacao_Aux.size() - (tam_pop - rand_sel);
	GeraAleatorios(numeros,rand_sel,limite);
	for(int b = 0;b < rand_sel;b++){
		populacao.at(b) = populacao_Aux.at(numeros[b]);
	}
	populacao_Aux.clear();
	//If BestWorst - normal GA,otherwise GA to maximize Pb
    if(BestWorst)
        sort(populacao_Aux.begin(),populacao_Aux.end(),Compara);
    else
        sort(populacao_Aux.begin(),populacao_Aux.end(),ComparaPior);
    ///////////////////////////////////////////////////////
}

void AlgoritmoGenetico::InicializaPopulacao(){
	//Aleatório
	srand(time(NULL));
	//Repetido
	srand(1);
	int a = num_tam_req;
	long double fit,v;
    int num;
    //vetor para auxiliar a gerar números aleatórios diferentes
    std::vector<unsigned int> vectorAux;
	//Inicio da inicialização do vetor de individuos
	for(int i = 0; i < tam_pop; i++){
        populacao.push_back(NULL);
        populacao.at(i) = new Individuo();
        //redimensiona a matriz de combinações
        populacao.at(i)->combinacoes.resize(num_tam_req,vector<int>(tam_genes));
        //inicializa a matriz com valores aleatorios
		for(unsigned int a = 0;a < num_tam_req;a++)
            for(unsigned int b = 0;b < tam_genes;b++){
                do
                    num = rand()%tam_genes;
                while(std::find(vectorAux.begin(), vectorAux.end(), num)!= vectorAux.end());
                vectorAux.push_back(num);
                populacao.at(i)->combinacoes.at(a).at(b) = num;
                //cout<< num << endl;
                if(b == tam_genes - 1)
                    vectorAux.clear();
            }
		////////////////////////////////////////////
        fit = Kernel::Simulation(i,0);
        populacao.at(i)->setFitness(fit);
        populacao.at(i)->cont_sim = 1;
        Ressim(0,i,ressim);
	}
	std::sort(populacao.begin(),populacao.end(),Compara);
    for(int c = 0;c < tam_pop;c++){
           InpOut::GA_Initial_Pop << populacao.at(c)->getFitness();
           InpOut::GA_Initial_Pop<<endl;
    }
    /////////////////////////////////////////////////////
}

void AlgoritmoGenetico::SelecaoRoleta(vector<int>& indices_pais,bool process,bool BestWorst){

vector<Individuo*> selected_pop;
int limite = 0;
//Para crossover
if(!process){
    selected_pop = populacao;
    limite = populacao.size() - 1;
}
//Para mutação
else{
    selected_pop = pop_Mut;
    limite = pop_Mut.size() - 1;
}
long double  valorAleatorio;
int c = 0, tam = selected_pop.size();
long double subFitness = 0,sumFitness = 0, pior_pb, melhor_pb;
//If BestWorst - normal GA,otherwise GA to maximize Pb
if(BestWorst)
    sort(selected_pop.begin(),selected_pop.begin()+limite,Compara);
else
    sort(selected_pop.begin(),selected_pop.begin()+limite,ComparaPior);
//////////////////////////////////////////////////////
//Pegar melhor pb pra usar na roleta
melhor_pb = selected_pop.at(limite)->getFitness();
pior_pb = selected_pop.at(0)->getFitness();

	//Soma as Pb's
	for(int a = 0;a <= limite;a++){
		sumFitness = sumFitness + 1/(selected_pop.at(a)->getFitness() + melhor_pb);
	}
	valorAleatorio =  (float(rand())/RAND_MAX)*sumFitness;
	for(int b = 0;b <= limite;b++){
        if(b == 0){
            //Novo jeito para roleta
            subFitness = valorAleatorio - 1/(selected_pop.at(b)->getFitness() + melhor_pb);
            if(subFitness <= 0){
                subFitness = 0;
                indices_pais.at(0) = b;
                break;
            }
        }
		else{
            subFitness = subFitness - 1/(selected_pop.at(b)->getFitness() + melhor_pb);
            if(subFitness <= 0){
                subFitness = 0;
                indices_pais.at(0) = b;
                break;
            }
		}
	}
	do{
		valorAleatorio = (float(rand())/RAND_MAX)*sumFitness;
		for(c = 0;c <= limite;c++){
			if(c == 0){
                subFitness = valorAleatorio - 1/(selected_pop.at(c)->getFitness() + melhor_pb);
                if(subFitness <= 0){
                    if(indices_pais.at(0) == c){
                        subFitness = 0;
                        break;
                    }
                    else{
                        indices_pais.at(1) = c;
                        break;
                    }
                }
            }
            else{
                subFitness = subFitness - 1/(selected_pop.at(c)->getFitness() + melhor_pb);
                if(subFitness <= 0){
                    if(indices_pais.at(0) == c){
                        subFitness = 0;
                        break;
                    }
                    else{
                        indices_pais.at(1) = c;
                        break;
                    }
                }
            }
		}
	}while(indices_pais.at(0) == c );
}

int AlgoritmoGenetico::SelecaoRoleta_Torneio(){

long double  valorAleatorio;
int c = 0, tam = populacao.size(), indices_pais;
long double subFitness = 0,sumFitness = 0, pior_pb, melhor_pb;
//sort(populacao.begin(),populacao.end(),Compara);
sort(populacao.begin(),populacao.end(),Compara);
//Pegar melhor pb pra usar na roleta
melhor_pb = populacao.at(populacao.size() - 1)->getFitness();
pior_pb = populacao.at(0)->getFitness();

	//Soma as Pb's
	for(int a = 0;a < populacao.size();a++){
		sumFitness = sumFitness + 1/(populacao.at(a)->getFitness() + melhor_pb);
	}
	valorAleatorio =  (float(rand())/RAND_MAX)*sumFitness;
	for(int b = 0;b < populacao.size();b++){
        if(b == 0){
            //Novo jeito para roleta
            subFitness = valorAleatorio - 1/(populacao.at(b)->getFitness() + melhor_pb);
            if(subFitness <= 0){
                subFitness = 0;
                indices_pais = b;
                break;
            }
        }
		else{
            subFitness = subFitness - 1/(populacao.at(b)->getFitness() + melhor_pb);
            if(subFitness <= 0){
                subFitness = 0;
                indices_pais = b;
                break;
            }
		}
	}
	return indices_pais;
}

void AlgoritmoGenetico::Mutacao(Individuo* pai1,Individuo* filho1_PERM){
	// escolhe um gene aleatoriamente no intervalo [0, tam_genes - 1]
	srand(time(NULL));
	long double fit,pb_old,pb_new,pb_med;
	bool perm = false;//variavel que verifica se alguma permutação foi feita
	int gene_1, prob_mut;
	int gene_2,aux;
	//Copia filho exatamente igual ao pai
	for(int i = 0;i < num_tam_req;i++){
            for(int j = 0;j < tam_genes;j++){
                     filho1_PERM->combinacoes.at(i).at(j) = pai1->combinacoes.at(i).at(j);
                     //filho2_PERM->combinacoes.at(i).at(j) = pai2->combinacoes.at(i).at(j);
            }
    }
    //////////////////////////////////////
	//inicio do processo de permutação
	for(int i = 0;i < num_tam_req;i++){
		for(int j = 0;j < tam_genes;j++){
            prob_mut = rand() % 100; //probabilidade de mutação do cromossomo
            if(prob_mut < mutacao_prob){
                perm = true;//pelo menos uma permutação foi feita
                gene_1 = j;
                do{
                    gene_2 = rand() % tam_genes;
                }while(gene_2 == gene_1);
                //Permutação das posições
                aux = filho1_PERM->combinacoes.at(i).at(gene_1);
                filho1_PERM->combinacoes.at(i).at(gene_1) = filho1_PERM->combinacoes.at(i).at(gene_2);
                filho1_PERM->combinacoes.at(i).at(gene_2) = aux;
                ///////////////////////////
            }
            else{
               //Cromossomo não permutado
            }
		}
	}

	populacao_Aux.at(0) = filho1_PERM;

    fit = Kernel::Simulation(0,1);

    populacao_Aux.at(0)->setFitness(fit);

	if(perm){//Ocorreu permutação em algum cromossomo
         //Ressimulate individual for 2 times
         Ressim(1,0,ressim);
    }
	/////////////////////////////////////////////////////
}

void AlgoritmoGenetico::Mutacao_1(Individuo* pai1, Individuo* filho1_PERM, int b){
	// escolhe um gene aleatoriamente no intervalo [0, tam_genes - 1]
	long double fit;
	bool perm = false;//variavel que verifica se alguma permutação foi feita
	int gene_1, prob_mut;
	int gene_2,aux;
	//Copia filho exatamente igual ao pai
	for(int i = 0;i < num_tam_req;i++){
            for(int j = 0;j < tam_genes;j++){
                     filho1_PERM->combinacoes.at(i).at(j) = pai1->combinacoes.at(i).at(j);
                     //filho2_PERM->combinacoes.at(i).at(j) = pai2->combinacoes.at(i).at(j);
            }
    }
    //////////////////////////////////////
	//inicio do processo de permutação
	for(int i = 0;i < num_tam_req;i++){
		for(int j = 0;j < tam_genes;j++){
            prob_mut = rand() % 100; //probabilidade de mutação do cromossomo
            if(prob_mut < mutacao_prob){
                perm = true;//pelo menos uma permutação foi feita
                gene_1 = j;
                do{
                    gene_2 = rand() % tam_genes;
                }while(gene_2 == gene_1);
                //Permutação das posições
                aux = filho1_PERM->combinacoes.at(i).at(gene_1);
                filho1_PERM->combinacoes.at(i).at(gene_1) = filho1_PERM->combinacoes.at(i).at(gene_2);
                filho1_PERM->combinacoes.at(i).at(gene_2) = aux;
                ///////////////////////////
            }
            else{
               //Cromossomo não permutado
            }
		}
	}

	populacao_Aux.at(populacao_Aux.size() - 1) = filho1_PERM;

	//Simulação e atribuição de Pb para o elementos criados
	if(perm){//Ocorreu permutação em algum cromossomo
        fit = Kernel::Simulation(populacao_Aux.size() - 1,1);
        filho1_PERM->setFitness(fit);
	}
	else{
        filho1_PERM->setFitness(pai1->getFitness());
	}
	filho1_PERM->cont_sim = 1;
	/////////////////////////////////////////////////////
}

void AlgoritmoGenetico::Cruzamento(Individuo* pai1, Individuo* pai2, Individuo* filho_1, Individuo *filho_2,int a){
	// escolhe o ponto do crossover
	srand(time(NULL));
	int num = rand() % num_tam_req;
	long double fit;
	for(int i = 0; i < num_tam_req ; i++)
		for(int j = 0; j < tam_genes; j++){
			if(i <= num ){
				filho_1->combinacoes.at(i).at(j) = pai1->combinacoes.at(i).at(j);
				filho_2->combinacoes.at(i).at(j) = pai2->combinacoes.at(i).at(j);
			}
			else{
				filho_1->combinacoes.at(i).at(j) = pai2->combinacoes.at(i).at(j);
				filho_2->combinacoes.at(i).at(j) = pai1->combinacoes.at(i).at(j);
			}
		}

	populacao_Aux.at(populacao_Aux.size() - 2) = filho_1;
	populacao_Aux.at(populacao_Aux.size() - 1) = filho_2;
	//Simulação e atribuição de Pb para o elementos criados
	fit = Kernel::Simulation(populacao_Aux.size() - 2,1);
	filho_1->setFitness(fit);
	filho_1->cont_sim = 1;//primeira simulação

	fit = Kernel::Simulation(populacao_Aux.size() - 1,1);
	filho_2->setFitness(fit);
	filho_2->cont_sim = 1;//primeira simulação

}

Individuo* AlgoritmoGenetico::GetPopulationElement(int position){
    return populacao.at(position);
}

Individuo* AlgoritmoGenetico::GetPopulationAuxElement(int position){
    return populacao_Aux.at(position);
}
//Gerar todos individuos para 7 padroes de requisições e 3 nucleos - 279936 individuos
void AlgoritmoGenetico::GeraTodos(){

int numCores = Topology::getNumCores(),numMelhores = 50;
int numTotalLinhas = 1, aux = numCores,aux_2 = 0,cont = 0,bestPosition,cont_aux = 0;
double fit, best = Def::MAX_DOUBLE , worst = 0;
std::vector<int> comb = {0,1,2};
std::sort(comb.begin(), comb.end());
vector< vector<int> > todasLinhas;
Individuo* bestInd = new Individuo();
Individuo* worstInd = new Individuo();
std::vector<Individuo*> melhores(numMelhores);
    //gerar todas combinações possiveis para uma linha
    for(int i = aux;i > 1;i = i -1)
        numTotalLinhas  = i*numTotalLinhas;
    //////////////////////////////////////////////////
    todasLinhas.resize(numTotalLinhas,vector<int> (numCores));
    //gerar todas a combinções possíveis para uma linha
    do{

        for(int i = 0;i < numCores;i++)
            todasLinhas.at(aux_2).at(i) = comb[i];
        aux_2++;
    } while(std::next_permutation(comb.begin(), comb.end()));
    //gerar todas as combinações possíveis de indíviduos
    for(int a = 0;a < numTotalLinhas;a++)
        for(int b = 0;b < numTotalLinhas;b++)
            for(int y = 0;y < numTotalLinhas;y++)
               for(int z = 0;z < numTotalLinhas;z++)
                  for(int x = 0;x < numTotalLinhas;x++)
                    for(int w = 0;w < numTotalLinhas;w++)
                      for(int c = 0;c < numTotalLinhas;c++){
                         todosIndividuos.push_back(NULL);
                         todosIndividuos.at(cont) = new Individuo();
                         todosIndividuos.at(cont)->combinacoes.resize(num_tam_req,vector<int>(tam_genes));
                         for(int d = 0;d < numCores;d++){
                            todosIndividuos.at(cont)->combinacoes.at(0).at(d) = todasLinhas.at(a).at(d);
                            todosIndividuos.at(cont)->combinacoes.at(1).at(d) = todasLinhas.at(b).at(d);
                            todosIndividuos.at(cont)->combinacoes.at(2).at(d) = todasLinhas.at(y).at(d);
                            todosIndividuos.at(cont)->combinacoes.at(3).at(d) = todasLinhas.at(z).at(d);
                            todosIndividuos.at(cont)->combinacoes.at(4).at(d) = todasLinhas.at(x).at(d);
                            todosIndividuos.at(cont)->combinacoes.at(5).at(d) = todasLinhas.at(w).at(d);
                            todosIndividuos.at(cont)->combinacoes.at(6).at(d) = todasLinhas.at(c).at(d);
                         }
                         fit = Kernel::Simulation(cont,1);
                         todosIndividuos.at(cont)->setFitness(fit);

                         if(fit > worst){
                            worst = fit;
                            worstInd = todosIndividuos.at(cont);
                         }
                         cont++;
                   }
            //Order all indiiduals based on Bp,best Bp in last position
            sort(todosIndividuos.begin(),todosIndividuos.end(),Compara);
            /*Resimulate numMelhores best individuals*/
            for(unsigned int a = todosIndividuos.size() - 1;a > todosIndividuos.size() - (1 + numMelhores); a--){
                for(unsigned int i = 0;i < 4;i++){
                    fit = Kernel::Simulation(a,1);
                    todosIndividuos.at(a)->setFitness((fit + todosIndividuos.at(a)->getFitness())/2);
                }
                melhores.at(cont_aux) = todosIndividuos.at(a);
                cont_aux++;
            }
            sort(melhores.begin(),melhores.end(),Compara);
            bestInd = melhores.at(melhores.size() - 1);
            ///////////////////////////////////
            //todosIndividuos.at(bestPosition)->setFitness(best);
            Imprime_Individuo_GeraTodos(bestInd);
            Imprime_Individuo_GeraTodos(worstInd);
            for(int a = 0;a < melhores.size();a++)
                Imprime_Individuo_GeraTodos(melhores.at(a));
    //////////////////////////////////////////////////////
    //Debug para imprimir os individuos gerados
    /*for(int c = 0;c < 7776;c++){
           //InpOut::GA_Initial_Pop << "Fitness Elemento" << "-" << c << "=" <<populacao.at(c)->getFitness();
                 for(int a = 0;a < num_tam_req;a++){
                        InpOut::GA_Initial_Pop<< " -Ordem do FFC- ";
                        for(int b = 0;b < tam_genes;b++){
                            InpOut::GA_Initial_Pop<<todosIndividuos.at(c)->combinacoes.at(a).at(b);
                            if(b == 3){
                               InpOut::GA_Initial_Pop<<"-";
                            }
                        }
                if(a == num_tam_req - 1)
                    InpOut::GA_Initial_Pop<<endl;
                }
    }*/
}

//Retorna determinado individuo do conjunto de todos
Individuo* AlgoritmoGenetico::GetIndividuo(int pos){
    return todosIndividuos.at(pos);
}

void AlgoritmoGenetico::Imprime_Individuo_GeraTodos(Individuo* ind){
        double pb = ind->getFitness();
        for(int a = 0;a < num_tam_req;a++){
           InpOut::Result << "Ordem do FFC: ";
           for(int b = 0;b < tam_genes;b++){
                InpOut::Result << ind->combinacoes.at(a).at(b);
                if(b == tam_genes - 1)
                    InpOut::Result << endl;
           }
        }
        InpOut::Result << "Pb : " << pb << endl;
}

void AlgoritmoGenetico::Imprime_Pop_Final(){

    for(int c = 0;c < tam_pop;c++){
               cout<<"Fitness elemento"<<c<<"="<<populacao.at(c)->getFitness()<<endl;
               InpOut::GA_Results << "Fitness Elemento" << "-" << c << " = " <<populacao.at(c)->getFitness();
                     for(int a = 0;a < num_tam_req;a++){
                            cout<<"Ordem do FFC"<<endl;
                            InpOut::GA_Results<< " -Ordem do FFC- ";
                            for(int b = 0;b < tam_genes;b++){
                                cout<<populacao.at(c)->combinacoes.at(a).at(b);
                                InpOut::GA_Results<<populacao.at(c)->combinacoes.at(a).at(b);
                                if(b == 6){
                                   cout<<endl;
                                   InpOut::GA_Results<<"-";
                                }
                            }
                    if(a == num_tam_req - 1)
                        InpOut::GA_Results<<endl;
                    }
    }

}

int AlgoritmoGenetico::Rand_Sel(){
    int num;
    srand(time(NULL));
    num = rand() % populacao.size();
return num;
}
//Function to ressimulate individual in the vector -vec- in the position -pos- for number of times -num_times-
void AlgoritmoGenetico::Ressim(int vec,int pos,int num_times){
long double pb_old,pb_new,pb_med;
vector<Individuo*> select_pop;
    if(vec == 0)
        select_pop = populacao;
    else
        select_pop = populacao_Aux;
    for(int a = 0;a < num_times;a++){
            pb_old = select_pop.at(pos)->getFitness();
            pb_new = Kernel::Simulation(pos,vec);
            pb_med = (pb_old + pb_new)/2;
            select_pop.at(pos)->cont_sim = select_pop.at(pos)->cont_sim + 1;
            select_pop.at(pos)->setFitness(pb_med);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
