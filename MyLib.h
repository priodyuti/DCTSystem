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

#include<chrono>


#include "Helper.h"
#include "CONTACT.h"
#include "Theta.h"
#include "PhiRec.h"
#include "PhiRecOverflow.h"
#include "Omega.h"

#ifdef _WIN32
#include <direct.h>
#elif defined __linux__
#include <sys/stat.h>
#include <unistd.h>
#endif

using namespace std;

class Phi{
private:
	#ifdef _WIN32
	    char PhiFileName[39] = "C:\\DCTSystem\\Data\\Phi.bin"; //File name where data is stored.
	    char PhiFileHeaderName[45] = "C:\\DCTSystem\\Data\\PhiHeader.bin"; //File name where data is stored.
		char PhiOvAvailListFileName[56] = "C:\\DCTSystem\\Data\\PhiOvAvailList.bin";
		char NonUpdatedContactsFileName[49] = "C:\\DCTSystem\\Data\\NoUpdatedList.bin";
		char UsedCVIDListFileName[47] = "C:\\DCTSystem\\Data\\UsedCVIDList.bin";
    #elif defined __linux__
        char PhiFileName[45] = "/home/gautam/DCTSystem/Data/Phi.bin";
        char PhiFileHeaderName[51] = "/home/gautam/DCTSystem/Data/PhiHeader.bin";
		char PhiOvAvailListFileName[62] = "/home/gautam/DCTSystem/Data/PhiOvAvailList.bin";
		char NoUpdateListFileName[56] = "/home/gautam/DCTSystem/Data/NoUpdateList.bin";
		char UsedCVIDListFileName[54] = "/home/gautam/DCTSystem/Data/UsedCVIDList.bin";
    #endif
    long N = 100u;//Total population smart-phone users // 4 bytes
	long q = 10u;//Average number individuals may come in close contacts during last D days of observation period.
	float f = 1.0; 
	long long NoOfPhiRec = N * ceil(q * f); // 8 byte - (u,v) and (v,u) are stored
	long long NoOfPhiAvailOvRec=0;
	long long NoOfPhiOvRec=0;
public:
	Phi();
	Phi(long parN, long parq, float parf);
	~Phi(){}
	Phi& operator=(const Phi& parPHI);
	friend ostream& operator<<(ostream& out, const Phi& parPhi);
	inline long GetN() { return N; }
	long long GetPhiRecCount();
	bool GetPhiRecIndex(long parSID,long parDID,bool &parOvFlag,long long &parPhiRecIndex);
	bool GetCVID(long parSID,bool parOvFlag,long long parPhiRecIndex,long long& parCVID);
	bool GetCVID(long parSID,long parDID,long long& parCVID,bool& parOvFlag,long long& parPhiRecIndex);
	void GetContactList(long parSID, vector<CONTACT>& parCL);
	bool Insert(long parSID,long parDID,long long parCVID,bool& parOvFlag,long long& parPhiRecIndex);
	bool Delete(long parSID, bool parOvFlag, long long parPhiRecIndex);
	bool Delete(long parSID, long parDID);
	inline int Size();
	void StorageSize(long long &parCalSize,long long &parObsrvSize);
	bool GetPhiRec(const long long parRecNo, PhiRec& R);
	bool SetPhiRec(const long long parRecNo, PhiRec& R);
	bool GetNode(long long &parPhiOvRecIndex);
	bool FreeNode(long long parPhiOvRecIndex);
	bool SetResetUpdateFlag(long parSID,long parDID,bool parUpdateFlag);
	bool SetUpdateFlag(long parSID,long parDID,bool parOvFlag,long long parPhiRecIndex);
	bool SetUpdateFlag(long parSID,bool parOvFlag,long long parPhiRecIndex);
	bool GetUpdateFlag(long parSID,bool parOvFlag,long long parPhiRecIndex);
	bool ResetAllUpdateFlags();
	bool GetNoUpdateList();
	void ShowNoUpdateList();
	void GetNoUpdateListFileName(char parFileName[55]);
	void GetUsedCVIDListFileName(char parFileName[55]);
	bool GetUsedCVIDList();
	bool GetDgree(long parSID, long &parDegree);
	long GetMaxDegree();
	long GetMinDegree();
	bool GetDegreeDistribution(std::string parFLName);
	long long GetTotalDgree();
};

class ContactGraph{
private:
	Phi *PHI=NULL;
	Theta *TH=NULL;
	short NoRecFlag;
public:
	ContactGraph();
	ContactGraph(long parN,long parq,int parNoOfDays,int parDayDuration,int parSlotDuration,
		int parDelta, float parf);
	void Show();
	friend ostream& operator<<(ostream& out, const ContactGraph& parCG);
	set<long> GetContactTrace(set<long> parIndividuals);
	void ProcessOmega(Omega parOmega,int &parCCFront,int &parCCRear,int &parDCFront,int &parDCRear);
	bool InitializeContact(int parCCFront,int parCCRear,int parDCFront,int parDCRear);
	bool BuildContactGraph(string parDataFName,int &parCCFront,int &parCCRear,int &parDCFront,int &parDCRear);
	bool ClearContactGraph(int &parCCFront,int &parCCRear,int &parDCFront,int &parDCRear);	
	bool Add(string parDataFName,int &parCCFront,int &parCCRear,int &parDCFront,int &parDCRear,long double &parExecTime);
	int Size();
	void StorageSize(long long &parCalSize, long long &parObsrvSize);
	void StorageSize(long long &parPhiCalSize,long long &parPhiObsrvSize,
			long long &parThetaCalSize,long long &parThetaObsrvSize);
	int GetNoOfObservationDays();
    int GetSlotDuration();
    int GetDayDuration();
    long GetN();
    long Getq();
	int GetNoOfCloseContactSlots();
	int GetNoOfCloseContactsDuringADay();
};