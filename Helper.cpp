#include "Helper.h"

using namespace std;

//****************************************************************************************
//************************* Helper Library ***********************************************
//****************************************************************************************
inline bool IsFileExists(const std::string &name){
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

bool CheckExistingDCTSystem(){
    int Status;
    Status = chdir("/home/gautam/DCTSystem/Data");
    if (Status == 0)
        return true;
    return false;
}

bool IsDCTSystemFullyExists(){
    int Status;
    Status = chdir("/home/gautam/DCTSystem/Data");
    if (Status != 0)
        return false;
    if (!IsFileExists("/home/gautam/DCTSystem/Data/SysConfig.bin"))
        return false;
    if (!IsFileExists("/home/gautam/DCTSystem/Data/PhiHeader.bin"))
        return false;
    if (!IsFileExists("/home/gautam/DCTSystem/Data/Phi.bin"))
        return false;
    if (!IsFileExists("/home/gautam/DCTSystem/Data/PhiOvAvailList.bin"))
        return false;
    if (!IsFileExists("/home/gautam/DCTSystem/Data/NoUpdateList.bin"))
        return false;
    if (!IsFileExists("/home/gautam/DCTSystem/Data/ThetaHeader.bin"))
        return false;
    if (!IsFileExists("/home/gautam/DCTSystem/Data/Theta.bin"))
        return false;
    if (!IsFileExists("/home/gautam/DCTSystem/Data/Omega.bin"))
        return false;
    if (!IsFileExists("/home/gautam/DCTSystem/Data/OmegaHeader.bin"))
        return false;
    if (!IsFileExists("/home/gautam/DCTSystem/Data/AvailCVList.bin"))
        return false;

    return true;
}

bool RemoveExistingDCTSystem(){
    int Status;
    remove("/home/gautam/DCTSystem/Data/SysConfig.bin");
    remove("/home/gautam/DCTSystem/Data/PhiHeader.bin");
    remove("/home/gautam/DCTSystem/Data/Phi.bin");
    remove("/home/gautam/DCTSystem/Data/PhiOvAvailList.bin");
    remove("/home/gautam/DCTSystem/Data/NoUpdateList.bin");
    remove("/home/gautam/DCTSystem/Data/ThetaHeader.bin");
    remove("/home/gautam/DCTSystem/Data/Theta.bin");
    remove("/home/gautam/DCTSystem/Data/AvailCVList.bin");
    remove("/home/gautam/DCTSystem/Data/Omega.bin");
    remove("/home/gautam/DCTSystem/Data/OmegaHeader.bin");
    
    Status = rmdir("/home/gautam/DCTSystem/Data");
    if (Status != 0)
        return false;
    return true;
}

bool CreateNewDCTSystemStructure(){
    int Status;
    //Status = mkdir("/home/gautam/Dropbox/DCTSystem",0777);
    //if (Status != 0)
    //    return false;
    Status = mkdir("/home/gautam/DCTSystem/Data",0777);
    if (Status != 0)
        return false;
    return true;
}

//note the const
template <class T>
void VectorDisplay(const vector<T> &v){
    //std::cout << endl;
    //std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, " "));
    for(auto e: v)
        std::cout<<e;
    std::cout << "(" << v.size() << ")";
}

Contact *GetNode(){
    Contact *New;
    New = (Contact *)malloc(sizeof(Contact));
    New->CVID = 0;
    New->UID = 0;
    New->Link = NULL;
    return New;
}
Contact *Insert(Contact *Start,   long parUID,   long long parCVID){
    Contact *New;
    New = GetNode();
    New->CVID = parCVID;
    New->UID = parUID;
    New->Link = Start;
    Start = New;
    return Start;
}

void Display(Contact *Start){
    Contact *Ptr;
    Ptr = Start;
    while (Ptr != NULL){
        cout << "(" << Ptr->UID << "," << Ptr->CVID << ")";
        Ptr = Ptr->Link;
    }
}

std::vector<bool> writeAsBits(const std::vector<char> &inBytes){
    std::vector<bool> V;
    int n,i,j;
    try{
        V.clear();
        n = inBytes.size();
        for (i = 0; i < n; i++){
              char Mask = 0x80;
            if (Mask & inBytes[i])
                V.push_back(true);
            else
                V.push_back(false);
            Mask = 0x40;
            for (j = 0; j < 7; j++){
                if (Mask & inBytes[i])
                    V.push_back(true);
                else
                    V.push_back(false);
                Mask = Mask >> 1;
            }
        }
        return V;
    }
    catch (std::out_of_range &e){
        std::cout << "Helper::writeAsBits(inBytes) " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc &e){
        std::cout << "Helper::writeAsBits(inBytes) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception &e){
        std::cout << "Helper::writeAsBits(inBytes) " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}
/*
bool writeAsBits(const vector<char>& inBytes, vector<bool>& outBits)
{
    int Size;
    Size = outBits.size();
    const int n = (int)inBytes.size();
    if ((n * 8) < Size) return false;
    int i = 0, s = 0;
    while (true)
    {
        if (s == Size)return true;
        char c, Mask = 0x80;
        c = inBytes[i];
        if (Mask & c)
            outBits[s] = true;
        else
            outBits[s] = false;
        Mask = 0x40;
        s++;
        while (Mask)
        {
            if (s == Size)return true;
            if (Mask & c)
                outBits[s] = true;
            else
                outBits[s] = false;
            Mask = Mask >> 1;
            s++;
        }
        i++;
    }
}
*/

bool writeAsBits(const vector<char> &inBytes, vector<bool> &outBits){
    int Size;
    try{
        outBits.clear();
        //Size = outBits.size();
        //std::cout<<"In Function outBits.size()="<<outBits.size()<<std::endl;
        const int n = inBytes.size();
        //if ((n * 8) < Size)
        //    return false;
        Size = n * 8;
        outBits.reserve(Size);
        for (int i = 0; i < n; i++){
              char Mask = 0x80;
            if (Mask & inBytes[i])
                outBits.push_back(true);
            else
                outBits.push_back(false);
            Mask = 0x40;
            for (int j = 0; j < 7; j++){
                if (Mask & inBytes[i])
                    outBits.push_back(true);
                else
                    outBits.push_back(false);
                Mask = Mask >> 1;
            }
        }
        return true;
    }
    catch (std::out_of_range &e){
        std::cout << "Helper::writeAsBits(inBytes,outBits) " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc &e){
        std::cout << "Helper::writeAsBits(inBytes,outBits) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception &e){
        std::cout << "Helper::writeAsBits(inBytes,outBits) " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

void writeAsBytes(const vector<bool> &inBits, vector<char> &outBytes){
    int n,Size,s=0,i=0;
    char c;
    try{
        n = inBits.size();
        Size = (int)ceil((double)n/8.0);
        outBytes.clear();
        outBytes.reserve(Size);
        s = 0, i = 0;
        while (true){
            c = 0x00;
            if (inBits[s])
                c = 0x80;
            s++;
            if (s == n){
                outBytes.push_back(c);
                return;
            }
            i = 1;
            char Mask = 0x40;
            while (i < 8){
                if (inBits[s])
                    c = c | Mask;
                Mask = Mask >> 1;
                i++;
                s++;
                if (s == n){
                    outBytes.push_back(c);
                    return;
                }
            }
            outBytes.push_back(c);
        }
    }
    catch (std::out_of_range &e){
        std::cout << "Helper::writeAsBits(inBytes,outBits) " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc &e){
        std::cout << "Helper::writeAsBits(inBytes,outBits) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception &e){
        std::cout << "Helper::writeAsBits(inBytes,outBits) " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

char *writeAsBytes(const vector<bool> &inBits){
    int n, N;
    int s = 0, i = 0;
    char c;
    char Mask;
    try{
        n = inBits.size();
        N = ceil((double)n / 8.0);
        char *outBytes = new char[N + 1];
        char *Ptr;
        Ptr = outBytes;
        s = 0, i = 0;
        while (true){
            c = 0x00;
            if (inBits[s])
                c = 0x80;
            s++;
            if (s == n){
                *Ptr = c;
                Ptr++;
                *Ptr = '\0';
                return outBytes;
            }
            i = 1;
            Mask = 0x40;
            while (i < 8){
                if (inBits[s])
                    c = c | Mask;
                Mask = Mask >> 1;
                i++;
                s++;
                if (s == n){
                    *Ptr = c;
                    Ptr++;
                    *Ptr = '\0';
                    return outBytes;
                }
            }
            std::cout << " c=" << c;
            *Ptr = c;
            Ptr++;
        }
    }
    catch (std::out_of_range &e){
        std::cout << "Helper::writeAsBytes(inBits) " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc &e){
        std::cout << "Helper::writeAsBytes(inBits) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception &e){
        std::cout << "Helper::writeAsBytes(inBits) " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

// Count number of bytes needed to contain the bits
// and then copy 8 bit block as bytes.
// https://stackoverflow.com/questions/551579/get-bytes-from-a-stdvectorbool, Accessed on 11/09/2021
//void writeAsBytes(const vector<bool>& inBits, vector<uint8_t>& outBytes)
/*
void writeAsBytes(const vector<bool>& inBits, vector<char>& outBytes) {
    int bitOffset = 0;
    const int maxBitOffset = (int)inBits.size();
    const bool emitMSB = true;
    int numBytes = (int)inBits.size() / 8;
    if ((inBits.size() % 8) != 0) {
        numBytes += 1;
    }
    for (int bytei = 0; bytei < numBytes; bytei++) {
        // Consume next 8 bits
        uint8_t byteVal = 0;
        for (int biti = 0; biti < 8; biti++) {
            if (bitOffset >= maxBitOffset) {
                break;
            }
            bool bit = inBits[bitOffset++];
            // Flush 8 bits to backing array of bytes.
            // Note that bits can be written as either
            // LSB first (reversed) or MSB first (not reversed).
            if (emitMSB) {
                byteVal |= (bit << (7 - biti));
            }
            else {
                byteVal |= (bit << biti);
            }
        }
        outBytes.push_back(byteVal);
    }
}
*/

std::string RemoveSpaces(std::string parStr){
    std::string Str;
    for(auto c : parStr)
        if(c !=' ')
            Str = Str + c;
    return Str;
}

std::string ReplaceChar(std::string parStr, char parChar1, char parChar2){
    std::string Str;
    for (auto c: parStr)
    {
        if (c == parChar1)
            Str = Str + parChar2;
        else Str = Str + c;
        /* code */
    }
    return Str;
}

std::vector<std::string> Split(std::string Str, char Delm){
    std::vector<std::string> Token;
    std::string str;
    try{
        str.clear();
        for (auto &ch : Str){
            if (ch != Delm){
                if(ch !=' ' && ch != '\n')
                    str.push_back(ch);
            }
            else{
                Token.push_back(str);
                str.clear();
            }
        }
        if(!str.empty())
            Token.push_back(str);
        return Token;
    }
    catch (std::out_of_range &e){
        std::cout << "Helper::Split(Str,Delm) " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc &e){
        std::cout << "Helper::Split(Str,Delm) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception &e){
        std::cout << "Helper::Split(Str,Delm) " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Comp(std::string parVal1, std::string parVal2) {
    std::vector<std::string> Vals1, Vals2;
    Vals1 = Split(parVal1,'_');
    Vals2 = Split(parVal2,'_');
    return (stoll(Vals1[1],nullptr,10) < stoll(Vals2[1],nullptr,10)); 
}

void FieldSort(std::list<std::string> &parVals){
    parVals.sort(Comp);
}


Date::Date(){
    Day = 1;
    Month = 1;
    Year = 1900;
}

Date::Date(char parDay,char parMonth,int parYear){
    Day = parDay;
    Month = parMonth;
    Year = parYear;
}

void Date::SetDay(char parDay){
    Day = parDay;
}
void Date::SetMonth(char parMonth){
    Month = parMonth;
}
void Date::SetYear(int parYear){
    Year = parYear;
}
char Date::GetDay(){
    return Day;
}
char Date::GetMonth(){
    return Month;
}
int Date::GetYear(){
    return Year;
}
Date &Date::Get(){
    return *this;
}
void Date::Set(Date parDate){
    Day = parDate.Day;
    Month = parDate.Month;
    Year = parDate.Year;
}

ostream &operator<<(ostream &out, const Date &parDate){
    out << parDate.Day << "/" << parDate.Month << "/" << parDate.Year;
    return out;
}
istream &operator>>(istream &in, Date &parDate){
    //Date in (dd/mm/yyyy) format
    char ch1, ch2;
    in >> parDate.Day;
    in >> ch1;
    in >> parDate.Month;
    in >> ch2;
    in >> parDate.Year;
    return in;
}
//******************************** End of Helper Library *********************************