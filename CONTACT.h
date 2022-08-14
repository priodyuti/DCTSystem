
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


class CONTACT{
private:
      long UID;
      long long CVID;
public:
    CONTACT() { UID = 0; CVID = 0; }
    CONTACT(  long parUID,   long long parCVID) { UID = parUID; CVID = parCVID; }
    void SetUID(  long parUID) { UID = parUID; }
    void SetCVID(  long long parCVID) { CVID = parCVID; }
    long long GetCVID() { return CVID; }
    long GetUID() { return UID; }
    friend ostream& operator<<(ostream& out, CONTACT& parOut);
    int Size();
};
