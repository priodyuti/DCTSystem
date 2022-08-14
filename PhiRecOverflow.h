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

class PhiRecOverflow{
private:
	long NeighborID;
	long HostID;
	long long CVID;
	bool DelFlag;
	bool UpdateFlag;//When a day slot for (u,v) is updated this is set true, so same action for (v,u) is not required 
	long long FLink = -1,BLink=-1;
public:
	PhiRecOverflow();
	PhiRecOverflow(long long parCVID, long parHostID, long parNeighborID);
	friend ostream& operator<<(ostream& out, const PhiRecOverflow& parPRec);
	int Size();
	long GetNeighborID();
	long GetHostID();
	long long GetCVID();
	bool GetDelFlag();
	bool GetUpdateFlag();
	void SetNeighborID(long parNeighborID);
	void SetHostID(long parHostID);
	void SetCVID(  long long parCVID);
	void SetDelFlag(bool parFlag);
	void SetUpdateFlag(bool parFlag);
	long long GetNext();
	long long GetPrevious();
	bool SetNext(long long parFLink);
	bool SetPrevious(long long parBLink);
};