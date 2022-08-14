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


class ContactVector{
private:
    int NoOfDays = 14; //Number of days under observation (D days). Default value is 14 Days
    int DayDuration = 24;
    int SlotDuration =15; //Minimum Time duration (d mins) within maximum admissible separation consider as close contact. Default value is 15mins
    vector<bool> CloseContacts; //It is a boolean type vector with NoOfCloseContacts number of components
    vector<bool> DayContacts;//It is a boolean type vector with NoOfDays number of components
    int CCFront, CCRear;
    int DCFront, DCRear;    
      int NoOfCloseContacts; //Number of close contacts (n). Formula for it is n = ceil((D*24*60)/d)
public:
    ContactVector(); 
    ContactVector(const   int parNoOfDays, const   int parDayDuration, const   int parSlotDuration);

    void ShowConfig();
    int CVSize();
    int CVSizeInBytes();
    int CloseContactsSize();
    int DayContactsSize();
    int GetNoOfDays();
    int GetSlotDuration();
    int GetDayDuration();

    std::vector<bool> GetContactVector();
    std::vector<bool> GetCloseContacts();
    std::vector<bool> GetDayContacts();

    bool SetACloseContact(const   int parPos, bool parVal);
    bool SetADayContact(const   int parPos, bool parVal);

    char* GetByteString();
    std::vector<char> GetByteVector();
    void GetByteVector(std::vector<char>& parByteVec);

    bool SetUseByteVector(const std::vector<char>& parByteVec);

    friend ostream& operator<<(ostream& parOut, const ContactVector& parCV);
    bool IsFalse();
    int CCQueueSize();
    int DCQueueSize();
    int GetCCQueueFront();
    int GetCCQueueRear();
    int GetDCQueueFront();
    int GetDCQueueRear();
    bool SetCCQueueFront(const int parCCFront);
    bool SetCCQueueRear(const int parCCRear);
    bool SetDCQueueFront(const int parDCFront);
    bool SetDCQueueRear(const int parDCRear);
    bool IsQueueEmpty();
    bool IsQueueFull();
    bool IsCCQueueEmpty();
    bool IsCCQueueFull();
    bool IsDCQueueEmpty();
    bool IsDCQueueFull();
    void InitializeQueue();
    void InitializeCCQueue();
    void InitializeDCQueue();
    bool EnQueue(std::vector<bool>& parVector);
    bool DeQueue(std::vector<bool>& parVector);
    bool ClearNextDay();
};
