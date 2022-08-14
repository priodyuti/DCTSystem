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

#ifdef _WIN32
#include <direct.h>
#elif defined __linux__
#include <sys/stat.h>
#include <unistd.h>
#endif

using namespace std;

class Omega
{
private:
	std::set<std::pair<long, std::vector<bool>>> Edges;
	long UID;
	unsigned long NoOfSlots;
	int Delta;
	long N;
    #ifdef _WIN32
        char OmegaHeaderFName[45] = "C:\\DCTSystem\\Data\\OmegaHeader.bin";
		char OmegaFName[42] = "C:\\DCTSystem\\Data\\Omega.bin";
    #elif defined __linux__
        char OmegaHeaderFName[52] = "/home/gautam/DCTSystem/Data/OmegaHeader.bin";
		char OmegaFName[46] = "/home/gautam/DCTSystem/Data/Omega.bin";
    #endif

public:
	Omega();
	Omega(long parUID,int parNoOfSlots,long parN,int parDelta);
	Omega(long parUID,int parNoOfSlots,long parN,int parDelta,std::string parDataFName);
	~Omega() {}
	long GetUID();
	unsigned long GetNoOfSlots();
	bool GetOmegaFName(char parOmegaFName[50]);
	long GetN();
	bool CreateEdge(long parUID);
	bool CreateEdge(long parUID, std::vector<bool> parEdgeLabel);
	bool RemoveEdge(long& parUID, std::vector<bool>& parEdgeLabel);
	bool GetEdgeLabel(long parUID, std::vector<bool>& parEdgeLabel);
	bool GetEdges(std::set<std::pair<long, std::vector<bool>>> &parEdges);
	bool GetASlot(long parUID,unsigned long parSlotNo, bool& parSlotVal);
	bool SetASlot(long parUID,unsigned long parSlotNo, bool parSlotVal);
	friend ostream& operator<<(ostream& parOut, const Omega& parOmega);
	bool Build(string parDataFName);
	bool BuildRandom(string parDataFName);
	bool Reset(long parUID);
	//friend istream& operator>>(istream& parIn, Omega& parOmega);
	void Show();
};
