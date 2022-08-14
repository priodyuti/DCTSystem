#pragma once
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
#include<dirent.h>
#include<list>
#include<regex>

#ifdef _WIN32
#include <direct.h>
#elif defined __linux__
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "MyLib.h"


using namespace std;


class DCTSystem{
private:
      int ID;    //Each DCT System has unique ID
    #ifdef _WIN32
        char AppPath[23] = "C:\\DCTSystem"; 
        char SysConfigFName[44] = "C:\\DCTSystem\\Data\\SysConfig.bin";
    #elif defined __linux__
        char AppPath[31] = "/home/gautam/Dropbox/DCTSystem";
        char SysConfigFName[50] = "/home/gautam/DCTSystem/Data/SysConfig.bin";
    #endif
    long N = 1000u; //Total population smart-phone users // 4 bytes
    //  long N = 10000000u; //Total population smart-phone users // 4 bytes
    long q = 40u;       //Average number individuals may come in close contacts during last D days of observation period.
    float f = 1.0;
    int D = 14;     //Number of days under observation
    int d = 15;     //SlotDuration - Minimum Time Duration for a close contact
    int Delta = 3;  //Sampling Time Quanta
    int Tau = ceil((double)d / Delta);          //Sliding Window size
    int T; //DayDuration - Interval for contact data upload
    int CCFront=-1, CCRear=-1, DCFront=-1, DCRear=-1;
    Date StartDate, CurrentDate;
    ContactGraph *CG = NULL;
public:
    DCTSystem();
    DCTSystem(int parID,long parN,long parq,int parNoOfDays,int parSlotDuration,int parDelta,
      int parDayDuration, float parf,Date parStartDate, Date parCurrentDate);
    friend ostream& operator<<(ostream& out, const DCTSystem& parDCTSys);
    void Show();
    void Simulate();
    void SimulateV1(std::string parDataFilePathName);
};
