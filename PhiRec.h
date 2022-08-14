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

class PhiRec{
private:
	long NeighborID;
	long long CVID;
	bool DelFlag;
	bool UpdateFlag;//When a day slot for (u,v) is updated this is set true, so same action for (v,u) is not required 
public:
	PhiRec();
	PhiRec(long long parCVID, long parNeighborID);
	friend ostream& operator<<(ostream& out, const PhiRec& parPRec);
	int Size();
	long GetNeighborID();
	long long GetCVID();
	bool GetDelFlag();
	bool GetUpdateFlag();
	void SetNeighborID(long parNeighborID);
	void SetCVID(long long parCVID);
	void SetDelFlag(bool parFlag);
	void SetUpdateFlag(bool parFlag);
};