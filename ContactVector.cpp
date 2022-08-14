#include "ContactVector.h"
#include "Helper.h"

using namespace std;

//****************************************************************************************
//******************************** Library for Contact Vector ****************************
//****************************************************************************************
ContactVector::ContactVector(){
    //Construct a contact vector for default D=14 and d=15min. 
    NoOfDays = 14;
    DayDuration = 24;
    SlotDuration = 15;
    try {
        int T = NoOfDays * DayDuration;
        if (SlotDuration == 0)
            throw std::overflow_error("Divide by zero exception");
        NoOfCloseContacts =(  int) ceil((double)(T * 60.0) / SlotDuration);
        CloseContacts.clear();
        CloseContacts.resize(NoOfCloseContacts, false);
        DayContacts.clear();
        DayContacts.resize(NoOfDays, false);
        CCFront = -1;
        CCRear = -1;
        DCFront = -1;
        DCRear = -1;
    }
    catch (std::overflow_error& e){
        std::cout << "Exception:" << e.what() << std::endl;
        std::exit(1);
    }
    catch (const std::out_of_range& e){
        std::cout << "Exception:" << e.what() << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc &e){
        std::cout << "Exception:" << e.what() << std::endl;
        std::exit(1);
    }
}

ContactVector::ContactVector(const int parNoOfDays, const int parDayDuration, 
    const int parSlotDuration){
    //It is a parametized constructor for creating a contact vector
    NoOfDays = parNoOfDays;
    DayDuration = parDayDuration;
    SlotDuration = parSlotDuration;
    int T;
    try {
        T = NoOfDays * DayDuration;
        if (SlotDuration == 0)
            throw std::overflow_error("Divide by zero exception");
        NoOfCloseContacts = (int)ceil((double)(T * 60.0) / SlotDuration);
        CloseContacts.clear();
        CloseContacts.resize(NoOfCloseContacts, false);
        DayContacts.clear();
        DayContacts.resize(NoOfDays, false);
        CCFront = -1;
        CCRear = -1;
        DCFront = -1;
        DCRear = -1;
    }
    catch (std::overflow_error& e){
        std::cout << "Exception:" << e.what() << std::endl;
        std::exit(1);
    }
    catch (const std::out_of_range& e){
        std::cout << "Exception:" << e.what() << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Exception:" << e.what() << std::endl;
        std::exit(1);
    }
}

int ContactVector::CVSize(){
    //Function to know the size of the contact vector
    return (NoOfCloseContacts + NoOfDays);
}

int ContactVector::CVSizeInBytes(){
    //Function to know the size of the contact vector
    int TotalNoOfBits;
    TotalNoOfBits = NoOfCloseContacts + NoOfDays;
    return (int)ceil((double)TotalNoOfBits / 8.0);
}

int ContactVector::CloseContactsSize(){
    return CloseContacts.size();
    //return NoOfCloseContacts;
}
int ContactVector::DayContactsSize(){
    return DayContacts.size();
    //return NoOfDays;
}

int ContactVector::GetNoOfDays(){
    //Function to know D.
    return NoOfDays;
}
int ContactVector::GetDayDuration(){
    //Function to know d.
    return DayDuration;
}
int ContactVector::GetSlotDuration(){
    //Function to know d.
    return SlotDuration;
}

std::vector<bool> ContactVector::GetDayContacts(){
    //Function to access the whole contact vector
    return DayContacts;
}

std::vector<bool> ContactVector::GetCloseContacts(){
    //Function to access the whole contact vector
    return CloseContacts;
}

bool ContactVector::SetADayContact(const int parPos, bool parVal){
    //Function for set / resetting the particular day based closed contact.
    if (parPos < 0 || parPos >= NoOfDays) return false;

    try{
        DayContacts[parPos] = parVal;
        return true;
    }
    catch(std::out_of_range &e){
        std::cout << "Exception in ContactVector::SetADayContact():" << e.what() << " not in index range" << std::endl;
        return false;
    }
}

bool ContactVector::SetACloseContact(const int parPos, bool parVal){
    //Function for set / resetting the particular minimum duration based closed contact.
    if (parPos < 0 || parPos >= NoOfCloseContacts) return false;

    try{
        CloseContacts[parPos] = parVal;
        return true;
    }
    catch (std::out_of_range& e){
        std::cout << "Exception in ContactVector::SetACloseContact():" << e.what() << " not in index range" << std::endl;
        return false;
    }
}

void ContactVector::ShowConfig(){
    std::cout << "NoOfDays=" << NoOfDays << std::endl;
    std::cout << "DayDuration=" << DayDuration << std::endl;
    std::cout << "SlotDuration=" << SlotDuration << std::endl;
    std::cout << "NoOfCloseContacts=" << NoOfCloseContacts << std::endl;
    std::cout << "CC Front=" << CCFront << std::endl;
    std::cout << "CC Rear=" << CCRear << std::endl;
    std::cout << "DC Front=" << DCFront << std::endl;
    std::cout << "DC Rear=" << DCRear << std::endl;
    std::cout << "C=(";
    std::copy(DayContacts.begin(), DayContacts.end(), std::ostream_iterator<bool>(std::cout, ""));
    std::cout << ")(" << DayContacts.size() << ")(";
    std::copy(CloseContacts.begin(), CloseContacts.end(), std::ostream_iterator<bool>(std::cout, ""));
    std::cout << ")(" << CloseContacts.size() << ")" << std::endl;
}


std::vector<bool> ContactVector::GetContactVector(){
    std::vector<bool> CV;
    try{
        CV.clear();
        CV = DayContacts;
        CV.insert(CV.end(), CloseContacts.begin(), CloseContacts.end());
        //Function to access the whole contact vector
        return CV;
    }
    catch (std::overflow_error& e){
        std::cout << "Exception:" << e.what() << std::endl;
        std::exit(1);
    }
    catch (const std::out_of_range& e){
        std::cout << "Exception:" << e.what() << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Exception:" << e.what() << std::endl;
        std::exit(1);
    }
}

std::vector<char> ContactVector::GetByteVector(){
    std::vector<char> ByteVec;
    std::vector<bool> BitVec;
    try {
        BitVec.clear();
        BitVec = DayContacts;
        BitVec.insert(BitVec.end(), CloseContacts.begin(), CloseContacts.end());
        ByteVec.clear();
        writeAsBytes(BitVec, ByteVec);
        return ByteVec;
    }
    catch (std::overflow_error& e){
        std::cout << "Exception:" << e.what() << std::endl;
        std::exit(1);
    }
    catch (const std::out_of_range& e){
        std::cout << "Exception:" << e.what() << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Exception:" << e.what() << std::endl;
        std::exit(1);
    }
}

void ContactVector::GetByteVector(std::vector<char>& parByteVec){
    std::vector<bool> BitVec;
    try {
        BitVec.clear();
        BitVec = DayContacts;
        BitVec.insert(BitVec.end(), CloseContacts.begin(), CloseContacts.end());
        parByteVec.clear();
        writeAsBytes(BitVec, parByteVec);
    }
    catch (std::overflow_error& e){
        std::cout << "Exception:" << e.what() << std::endl;
        std::exit(1);
    }
    catch (const std::out_of_range& e){
        std::cout << "Exception:" << e.what() << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Exception:" << e.what() << std::endl;
        std::exit(1);
    }
}

char* ContactVector::GetByteString(){
    //Function to get the whole contact vector in the form of string in 8-bits ASCII code
    std::vector<bool> V;
    try{
        V = DayContacts;
        V.insert(V.end(), CloseContacts.begin(), CloseContacts.end());
        int N;
        N = (int)ceil(V.size() / 8.0);
        char* Str = new char[N + 1];
        Str = writeAsBytes(V);
        //std::cout << "Str=" << Str << "Length=" << strlen(Str) << std::endl;
        return Str;
    }
    catch (std::overflow_error& e){
        std::cout << "Exception:" << e.what() << std::endl;
        std::exit(1);
    }
    catch (const std::out_of_range& e){
        std::cout << "ContactVector::GetByteString() " << e.what() <<" out of range" <<std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "ContactVector::GetByteString() " << e.what() << " bad allocation" <<std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "ContactVector::GetByteString() " << e.what() << " Exception occur" << std::endl;
        std::exit(1);
    }
}

bool ContactVector::SetUseByteVector(const std::vector<char>& parByteVec){
    //Function to set the whole contact vector with data from 8-bits ASCII code string form
    unsigned long NoOfBytes;
    int i=0,j=0,k=0;
    char Mask;
    bool BitVal;

    NoOfBytes = NoOfDays + NoOfCloseContacts;
    NoOfBytes = ceil(NoOfBytes / 8.0);

    //std::cout<<"In Function parByteVec.size()="<<parByteVec.size()<<std::endl;
    if (NoOfBytes != parByteVec.size()){
        std::cout << std::endl;
        std::cout << "Error in ContactVector::SetUseByteVector(): (" << parByteVec.size() << ") invalid vector size" << std::endl;
        return false;
    }
    try{        
        //for(  int ByteIndex=0;ByteIndex<NoOfBytes;ByteIndex++)
        for(auto Byte : parByteVec){
            Mask=0x80;
            if(Byte & Mask)
                BitVal = true;
            else
                BitVal = false;
            if(i<NoOfDays){
                DayContacts[j]=BitVal;
                j++;
            }
            else{
                CloseContacts[k]=BitVal;
                k++;
                if(k>=NoOfCloseContacts)return true;
            }
            i++;
            Mask = 0x40;
            while(Mask){
                //printf("\nMask=%X",Mask);
                if(Byte & Mask)
                    BitVal = true;
                else
                    BitVal = false;
                Mask = Mask>>1;
                if(i<NoOfDays){
                    DayContacts[j]=BitVal;
                    j++;
                }
                else{
                    CloseContacts[k]=BitVal;
                    k++;
                    if(k>=NoOfCloseContacts)return true;
                }
                i++;
            }
        }
        return true;
    }
    catch (std::overflow_error& e){
        std::cout << "ContactVector::SetUseByteVector(parByteVec) " << e.what() << " overflow" << std::endl;
        std::exit(1);
    }
    catch (const std::out_of_range& e){
        std::cout << "ContactVector::SetUseByteVector(parByteVec) " << e.what() << " out of range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "ContactVector::SetUseByteVector(parByteVec) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "ContactVector::SetUseByteVector(parByteVec) " << e.what() << " Exception occur" << std::endl;
        std::exit(1);
    }
}

ostream& operator<<(ostream& parOut, const ContactVector& parCV){
    //std::cout<<"\nIn ContactVector::operator<<() function"<<std::endl;
    //Function to display the current content of the contact vector
    parOut << "NoOfDays = " << parCV.NoOfDays << std::endl;
    parOut << "Day Duration(in Hrs) = " << parCV.DayDuration << std::endl;
    parOut << "Slot Duration(in Min) = " << parCV.SlotDuration << std::endl;
    parOut << "NoCloseContacts=" << parCV.NoOfCloseContacts << std::endl;
    parOut << "C=(";
    std::copy(parCV.DayContacts.begin(), parCV.DayContacts.end(), std::ostream_iterator<bool>(parOut, ""));
    parOut << ")(" << parCV.DayContacts.size() << ")(";
    std::copy(parCV.CloseContacts.begin(), parCV.CloseContacts.end(), std::ostream_iterator<bool>(parOut, ""));
    parOut << ")(" << parCV.CloseContacts.size() << ")" << std::endl;
    return parOut;
}

bool ContactVector::IsFalse(){
    /*  Day contact vector part is the summarization of the detail contact vector
    *   If none of the these Day contact is false then whole contact vector is false
    *   so this function returns true otherwise it returns false.
    */
    int i;
    try{
        for (i = 0; i < NoOfDays; i++)
            if (DayContacts[i]) return false;
        return true;
    }
    catch (std::overflow_error& e){
        std::cout << "ContactVector::IsFalse() " << e.what() << " overflow" << std::endl;
        std::exit(1);
    }
    catch (const std::out_of_range& e){
        std::cout << "ContactVector::IsFalse() " << e.what() << " out of range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "ContactVector::IsFalse() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "ContactVector::IsFalse() " << e.what() << " Exception occur" << std::endl;
        std::exit(1);
    }
}

int ContactVector::GetCCQueueFront(){   
    return CCFront;
}
int ContactVector::GetCCQueueRear(){
    return CCRear;
}
int ContactVector::GetDCQueueFront(){
    return DCFront;
}
int ContactVector::GetDCQueueRear(){
    return DCRear;
}
bool ContactVector::SetCCQueueFront(const int parCCFront){
    if (parCCFront < -1 || parCCFront >= (int)NoOfCloseContacts) return false;
    CCFront = parCCFront;
    return true;
}
bool ContactVector::SetCCQueueRear(const int parCCRear){
    if (parCCRear < -1 || parCCRear >= (int)NoOfCloseContacts) return false;
    CCRear = parCCRear;
    return true;
}
bool ContactVector::SetDCQueueFront(const int parDCFront){
    if (parDCFront < -1 || parDCFront >= (int)NoOfDays) return false;
    DCFront = parDCFront;
    return true;
}
bool ContactVector::SetDCQueueRear(const int parDCRear){
    if (parDCRear < -1 || parDCRear >= (int)NoOfDays) return false;
    DCRear = parDCRear;
    return true;
}

void ContactVector::InitializeQueue(){
    CCRear = -1; CCFront = -1;
    DCFront = -1; DCRear = -1;
    return;
}

void ContactVector::InitializeCCQueue(){
    CCRear = -1; CCFront = -1;
    return;
}
void ContactVector::InitializeDCQueue(){
    DCFront = -1; DCRear = -1;
    return;
}

bool ContactVector::IsQueueEmpty(){
    if (CCFront == -1 || DCFront == -1)
        return true;
    else
        return false;
}

bool ContactVector::IsQueueFull(){
    if (CCFront == -1 || DCFront == -1)
        return false;
    if (((int)((CCRear + 1) % NoOfCloseContacts) == CCFront) && ((int)((DCRear + 1) % NoOfDays) == DCFront))
        return true;
    else
        return false;
}

bool ContactVector::IsCCQueueEmpty(){
    if (CCFront == -1 || CCRear == -1)
        return true;
    else
        return false;
}
bool ContactVector::IsCCQueueFull(){
    if (CCFront == -1 || CCRear == -1)
        return false;
    if ((int)((CCRear + 1) % NoOfCloseContacts) == CCFront)
        return true;
    else
        return false;
}
bool ContactVector::IsDCQueueEmpty(){
    if (DCFront == -1 || DCRear == -1)
        return true;
    else
        return false;
}
bool ContactVector::IsDCQueueFull(){
    if (DCFront == -1 || DCRear == -1)
        return false;
    if ((int)((DCRear + 1) % NoOfDays) == DCFront)
        return true;
    else
        return false;
}

bool ContactVector::EnQueue(std::vector<bool>& parVector){
    //It is a daywise enqueue / insert operation
    unsigned long p,j;
    bool Flag;
    try{
        if (SlotDuration == 0)
            throw std::overflow_error("Divide by zero exception");

        p = ceil((double)(DayDuration * 60.0) / SlotDuration);
        if (parVector.size() != p){ //equal to number of close contacts in a day
            std::cout << std::endl;
            std::cout << "Error in ContactVector::EnQueue(parVector) - data size mismatch" << std::endl;
            return false;
        }
        Flag = false;
        if (CCFront == -1){
            CCFront = 0;
            CCRear = 0;
            if (parVector[0]) Flag = true;
            CloseContacts[CCRear] = parVector[0];
            for (j = 1; j < p; j++){
                if (parVector[j]) Flag = true;
                CCRear = (int)((CCRear + 1) % NoOfCloseContacts);
                CloseContacts[CCRear] = parVector[j];
            }
        }
        else{
            for (j = 0; j < p; j++){
                if (parVector[j]) Flag = true;
                CCRear = (int)((CCRear + 1) % NoOfCloseContacts);
                CloseContacts[CCRear] = parVector[j];
            }
        }
        if (DCFront == -1){
            DCFront = 0;
            DCRear = 0;
        }
        else DCRear = (int)((DCRear + 1) % NoOfDays);
        DayContacts[DCRear] = Flag;
        return true;
    }
    catch (std::overflow_error& e){
        std::cout << "ContactVector::EnQueue(parVector) " << e.what() << " overflow" << std::endl;
        std::exit(1);
    }
    catch (const std::out_of_range& e){
        std::cout << "ContactVector::EnQueue(parVector) " << e.what() << " out of range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "ContactVector::EnQueue(parVector) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "ContactVector::EnQueue(parVector) " << e.what() << " Exception occur" << std::endl;
        std::exit(1);
    }
}

int ContactVector::CCQueueSize(){
    if (CCFront == -1 || CCRear == -1) return 0;
    if ((int)((CCRear + 1) % NoOfCloseContacts) == CCFront)
        return NoOfCloseContacts;
    if (CCRear >= CCFront)
        return (int)(CCRear - CCFront + 1);
    return (int)(NoOfCloseContacts - (CCFront - CCRear));
}

bool ContactVector::DeQueue(std::vector<bool>& parVector){
    //It is a daywise dequeue or delete operation
    int p,i;
    try{
        parVector.clear();
        if (CCFront == -1){
            return false;
        }
        else{
            if (SlotDuration == 0)
                throw std::overflow_error("Divide by zero exception");
            p = ceil((double)(DayDuration * 60.0) / SlotDuration);
            for (i = 0; i < p; i++){
                parVector.push_back(CloseContacts[CCFront]);
                CloseContacts[CCFront] = false;
                CCFront = (int)((CCFront + 1) % NoOfCloseContacts);
            }
            DayContacts[DCFront] = false;
            DCFront = (int)((DCFront + 1) % NoOfDays);
        }
        return true;
    }
    catch (std::overflow_error& e){
        std::cout << "ContactVector::DeQueue(parVector) " << e.what() << " overflow" << std::endl;
        std::exit(1);
    }
    catch (const std::out_of_range& e){
        std::cout << "ContactVector::DeQueue(parVector) " << e.what() << " out of range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "ContactVector::DeQueue(parVector) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "ContactVector::DeQueue(parVector) " << e.what() << " Exception occur" << std::endl;
        std::exit(1);
    }
}

bool ContactVector::ClearNextDay(){
    if (CCFront == -1 || DCFront == -1) return false;
    int p,i;
    p = ceil((double)(DayDuration * 60.0) / SlotDuration);
    int Rear;
    Rear = CCRear;
    for (i = 0; i < p; i++){
        Rear = (int)((Rear + 1) % NoOfCloseContacts);
        CloseContacts[Rear] = false;
    }
    Rear = DCRear;
    Rear = (int)((Rear + 1) % NoOfDays);
    DayContacts[Rear] = false;
    return true;
}
//*********************************** End of Contact Vector Library **********************
