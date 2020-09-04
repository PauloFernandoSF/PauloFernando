#include "Route.h"
#include "Topology.h"

////////////////////////////////////////////////////////////////
Route::Route(vector<int>& path){
    Path.clear();
    for(unsigned int i = 0; i < path.size(); i++)
        Path.push_back(path.at(i));
}

////////////////////////////////////////////////////////////////
int Route::getOrN() const{
	assert(Path.size()>0);
	return Path.front();
}

// ------------------------------------------------ //
int Route::getDeN() const{
	return Path.back();
}

// ------------------------------------------------ //
unsigned int Route::getNumHops() const{
	return (unsigned int) Path.size()-1;
}

// ------------------------------------------------ //
unsigned int Route::getNumNodes() const{
    return (unsigned int) Path.size();
}

////////////////////////////////////////////////////////////////
int Route::getNode(unsigned int p) const{
	assert(p >= 0 && p < Path.size());
	return Path.at(p);
}

// ------------------------------------------------- //
void Route::addNodeAt(unsigned int p, int node){
    assert(Topology::nodeValid(node));
    int aux = Path.at(p);
    for(unsigned int i = p; i < Path.size(); i++){
        Path.at(i) = node;
        aux = Path.at(i+1);
        Path.at(i+1) = aux;
        node = 0;
    }
}
////////////////////////////////////////////////////////////////
void Route::print() const{
	for(unsigned int i = 0; i < Path.size(); i++)
		cout<<Path.at(i)<<"-";
}

////////////////////////////////////////////////////////////////
vector<int>* Route::getPath(){
    return &Path;
}

vector<int> Route::GetPath(){
    return Path;
}

// ------------------------------------------------------ //
double Route::getCost(){
    Link *link;
    double sum = 0.0;
    for(unsigned int i = 0; i < Path.size()-1; i++){
        link = Topology::getLink(Path.at(i), Path.at(i+1));
        sum += link->getCost();
    }
    return sum;
}


////////////////////////////////////////////////////////////////
Route* Route::createPartialRoute(int n1Ind, int n2Ind){
    vector<int> vRoute;
    for(int i = n1Ind; i <= n2Ind; i++){
        int a = this->getNode(i);
        vRoute.push_back(this->getNode(i));
    }
    return new Route(vRoute);
}

Route* Route::addRoute(Route* route){
    assert(this->getDeN() == route->getOrN());
    vector<int> newPath = this->GetPath();

    for(unsigned int a = 1; a < route->getNumNodes(); a++){
        newPath.push_back(route->getNode(a));
    }
    Route* r = new Route(newPath);
    return  r;
}
