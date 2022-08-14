#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<string.h>
#include<stdio.h>
#include<cstdio>
#include<random>
#include<math.h>
#include<stdlib.h>
#include<set>
#include<utility>
#include<iterator>
#include<algorithm>

using namespace std;

class RandomGraph{
private:
	long NoOfNodes;
	long AvgDegree;
    #ifdef _WIN32
        char GraphFileName[44] = "C:\\DCTSystem\\Data\\RandGraph.bin";
    #elif defined __linux__
        char GraphFileName[51] = "/home/gautam/DCTSystem/Data/RandGraph.bin";
    #endif
	int ModelNo;
public:
	RandomGraph();
	RandomGraph(long parNoOfNodes,long parAvgDegree,int parModelNo);
	void Show();
	void Draw();
};
