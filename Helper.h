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
#include<list>

#ifdef _WIN32
#include <direct.h>
#elif defined __linux__
#include <sys/stat.h>
#include <unistd.h>
#endif

using namespace std;

class Helper{
};
//note the const
template<class T>
void VectorDisplay(const vector<T>& v);

std::vector<bool> writeAsBits(const std::vector<char>& inBytes);
bool writeAsBits(const vector<char>& inBytes, vector<bool>& outBits);
void writeAsBytes(const vector<bool>& inBits, vector<char>& outBytes);
char* writeAsBytes(const vector<bool>& inBits);
std::string RemoveSpaces(std::string parStr);
std::string ReplaceChar(std::string parStr, char parChar1, char parChar2);
vector<std::string> Split(std::string Str, char Delm);
bool CheckExistingDCTSystem();
bool RemoveExistingDCTSystem();
bool CreateNewDCTSystemStructure();
void FieldSort(std::list<std::string> &parVals);


typedef struct Contact_Tag{
    long UID;
    long long CVID;
    Contact_Tag* Link;
}Contact;

Contact* GetNode();
Contact* Insert(Contact* Start,long parUID,long long parCVID);
void Display(Contact* Start);

class Date{
private:
    char Day, Month;
    short Year;
public:
    Date();
    Date(  char parDay,   char parMonth,   int parYear);
    void SetDay(  char parDay);
    void SetMonth(  char parMonth);
    void SetYear(  int parYear);
    char GetDay();
    char GetMonth();
    int GetYear();
    Date& Get();
    void Set(Date parDate);
    friend ostream& operator<<(ostream& out, const Date& parDate);
    friend istream& operator>>(istream& out, Date& parDate);
};
