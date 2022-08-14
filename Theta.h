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

#include "ContactVector.h"

#ifdef _WIN32
#include <direct.h>
#elif defined __linux__
#include <sys/stat.h>
#include <unistd.h>
#endif

using namespace std;


class Theta{
private:
    #ifdef _WIN32
        char ThetaFileHeaderName[43] = "C:\\DCTSystem\\Data\\ThetaHeader.bin";
        char AvailCVIDListFileName[43] = "C:\\DCTSystem\\Data\\AvailCVList.bin";
        char ThetaFileName[40] = "C:\\DCTSystem\\Data\\Theta.bin";
    #elif defined __linux__
        char ThetaFileHeaderName[53] = "/home/gautam/DCTSystem/Data/ThetaHeader.bin";
        char AvailCVIDListFileName[53] = "/home/gautam/DCTSystem/Data/AvailCVList.bin";
        char ThetaFileName[47] = "/home/gautam/DCTSystem/Data/Theta.bin";
    #endif
    int NoOfDays = 14; // 4 bytes, days unit
    int DayDuration = 24;
    int SlotDuration = 15; // 4 bytes, in min unit
    int NoOfCloseContactSlots = (int)ceil(((NoOfDays * DayDuration) * 60.00) / SlotDuration); // 4 bytes
    long N = 100u;//Total population smart-phone users // 4 bytes
    long q = 10u;//Average number individuals may come in close contacts during last D days of observation period.
    float f=1.0;
    int Delta;
    long long NoOfContactVectors = (long long)(N * (long)(q*f)); // 8 bytes
    long long NoOfUsedContactVectors=0;
public:
    Theta();
    Theta(long parN,long parq,int parNoOfDays,int parDayDuration,int parSlotDuration, int parDelta, float parf);
    ~Theta() {}
    int GetNoOfCloseContactSlots();
    int GetNoOfDays();
    int GetSlotDuration();
    int GetDayDuration();
    long long GetNoOfContactVectors();
    long GetN();
    long Getq();
    int GetDelta();
    int Size();
    void StorageSize(long long &parCalSize, long long &parObsrvSize);
    friend ostream& operator<<(ostream& parOut, const Theta& parTH);
    void Show();
    bool GetCV(long long parCVID, ContactVector& parCV);
    bool SetCV(long long parCVID, ContactVector& parCV);
    bool GetEmptyCV(long long& parCVID);//Get a ContactVector from the available list.
    bool FreeCV(long long parCVID);//Store the unused ContactVector in the available list.
    /*Add a given number of Contact Vector in the Theta and declare these as available*/
    bool AddCV(int parCount); 
    /*Add a only one Contact Vector in the Theta and declare these as available*/
    bool AddCV();
    /*Add only one Contact Vector in the Theta and passed the ID of this vector. It will 
    not be put on the Available CV list*/
    bool AddCV(long long &parCVID);  
    bool RemoveCV(int parCount);
};