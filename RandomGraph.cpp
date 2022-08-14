#include "RandomGraph.h"

using namespace std;

//****************************************************************************************
//*********************************** Random Graph Library *******************************
//****************************************************************************************

void ErdosRenyi(  long parNoOfNodes, float parProb, char parGraphFileName[20]){
	fstream Fpt;
	Fpt.open(parGraphFileName, ios::out | ios::binary);
	if (!Fpt.is_open()){
		std::cout << "Error in ErdosRenyi() " << parGraphFileName << " file cannot be created" << std::endl;
		exit(1);
	}
	Fpt.write((char*)&parNoOfNodes, sizeof(parNoOfNodes));
	std::pair<  long,   long> E;
	for(long u = 0;u<parNoOfNodes;u++)
		for(long v = u + 1;v<parNoOfNodes;v++)
			if ((rand() / RAND_MAX) < parProb){
				E = make_pair(u, v);
				Fpt.write((char*)&E, sizeof(E));
			}
	Fpt.close();
}

void Barabasi(  long parNoOfNodes, float parProb, char parGraphFileName[20]){
	fstream Fpt;
	Fpt.open(parGraphFileName, ios::out | ios::binary);
	if (!Fpt.is_open()){
		std::cout << "Error in ErdosRenyi() " << parGraphFileName << " file cannot be created" << std::endl;
		exit(1);
	}
	Fpt.write((char*)&parNoOfNodes, sizeof(parNoOfNodes));
	std::pair<  long,   long> E;
}

RandomGraph::RandomGraph(){

}

RandomGraph::RandomGraph(long parNoOfNodes,long parAvgDegree,int parModelNo){
	if (parNoOfNodes < 1){
		std::cout << "Invalid number of nodes for the graph" << std::endl;
		exit(1);
	}
	NoOfNodes = parNoOfNodes;

	if (parAvgDegree <1 || parAvgDegree >= parNoOfNodes){
		std::cout << "Invalid degree value for the graph" << std::endl;
		exit(1);
	}
	AvgDegree = parAvgDegree;
	if (parModelNo < 1 || parModelNo >2){
		std::cout << "Invalid random graph model number" << std::endl;
		exit(1);
	}
	ModelNo = parModelNo;
	switch (ModelNo){
	case 1: //Erdos-Renyi Model
	case 2: //Barabasi Model
	default:
		std::cout << "Invalid data model" << std::endl;
	}
}
void RandomGraph::Show(){

}
void RandomGraph::Draw(){

}
//************************************ End of Random Graph Library **********************