#include "Theta.h"

using namespace std;

//****************************************************************************************
//*********************************** Theta Library **************************************
//****************************************************************************************

Theta::Theta(){
    std::cout << "\nCreating Theta object[Theta::Theta()]....." << std::endl;

    std::fstream ThetaHeaderFPtr;
    ThetaHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream ThetaFPtr;
    ThetaFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream AvailCVIDListFPtr;
    AvailCVIDListFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    long long ReadRecCount,RecCount;
    int CVSize;
    ContactVector CV(NoOfDays, DayDuration, SlotDuration);
    
    try {
        ThetaHeaderFPtr.open(ThetaFileHeaderName, ios::in | ios::binary);
        if (ThetaHeaderFPtr.fail()) throw ThetaFileHeaderName;  // the exception being checked
        
        ThetaHeaderFPtr.read((char*)this, sizeof(Theta));
        ThetaHeaderFPtr.close();

        AvailCVIDListFPtr.open(AvailCVIDListFileName, ios::in | ios::binary);
        if (AvailCVIDListFPtr.fail()) throw AvailCVIDListFileName;  // the exception being checked

        AvailCVIDListFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        if (ReadRecCount != (NoOfContactVectors-NoOfUsedContactVectors)){
            std::cout << "Error in Theta::Theta() mismatch in the record count" << std::endl;
            std::exit(1);
        }
        AvailCVIDListFPtr.close();

        ThetaFPtr.open(ThetaFileName, ios::in | ios::binary);
        if (ThetaFPtr.fail()) throw ThetaFileName;  // the exception being checked

        ThetaFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        ThetaFPtr.seekg(0, ios::end);
        long long ThetaFlSize = ThetaFPtr.tellg();
        ThetaFPtr.close();

        ThetaFlSize = ThetaFlSize - sizeof(ReadRecCount);
        CVSize = CV.CVSizeInBytes();
        if (CVSize == 0)throw std::overflow_error("Divide by zero exception");
        RecCount = ceill((long double)ThetaFlSize / CVSize);
        if ((NoOfContactVectors != ReadRecCount) || (NoOfContactVectors != RecCount)){    
            std::cout << "Error in Theta::Theta() mismatch in the record count" << std::endl;
            std::exit(1);
        }
        std::cout << "\nExisting Theta object created successfully" << std::endl;
        return;
    }
    catch (std::fstream::failure& e){
        std::cerr << "Theta::Theta() " << e.what() << " - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Theta::Theta() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Theta::Theta() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Theta::Theta() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::overflow_error& e){
        std::cout << "Theta::Theta() " << e.what() << " Overflow" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Theta::Theta() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

Theta::Theta(long parN,long parq,int parNoOfDays,int parDayDuration,int parSlotDuration,int parDelta,float parf){
    std::cout << "\nCreating Theta object[Theta::Theta(parN, parq, parNoOfDays, parDayDuration, parSlotDuration, parDelta)]...." << std::endl;
    if (parN > 0)
        N = parN;
    if (parq > 0)
        q = parq;
    if (parNoOfDays > 0)
        NoOfDays = parNoOfDays;
    if (parDayDuration > 0)
        DayDuration = parDayDuration;
    if (parSlotDuration > 0)
        SlotDuration = parSlotDuration;
    if(parf>0)
        f = parf;
    if(parDelta>0)
        Delta = parDelta;

    long Q = ceil(f * q);
    int TotalObservationHours;

    float TotalObservationMin;
    std::fstream ThetaHeaderFPtr;
    ThetaHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream ThetaFPtr;
    ThetaFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream AvailCVIDListFPtr;
    AvailCVIDListFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    unsigned long n,i;
    long long CVID;
    ContactVector C(NoOfDays, DayDuration, SlotDuration);
    std::vector<char> ByteVec;
    
    try{
        TotalObservationHours = NoOfDays * DayDuration;
        TotalObservationMin = (float)(TotalObservationHours * 60.0);

        if (SlotDuration == 0)
            throw std::overflow_error("Divide by zero exception");

        NoOfCloseContactSlots = ceil((double)TotalObservationMin / SlotDuration);
        //NoOfContactVectors = (long long)(N * q);
        NoOfContactVectors = (long long)(N * Q);

        remove(ThetaFileHeaderName);
        ThetaHeaderFPtr.open(ThetaFileHeaderName, ios::out | ios::binary);
        if (ThetaHeaderFPtr.fail()) throw ThetaFileHeaderName;  // the exception being checked

        remove(AvailCVIDListFileName);
        AvailCVIDListFPtr.open(AvailCVIDListFileName, ios::out | ios::binary);
        if (AvailCVIDListFPtr.fail()) throw AvailCVIDListFileName;  // the exception being checked

        remove(ThetaFileName);
        ThetaFPtr.open(ThetaFileName, ios::out | ios::binary);
        if (ThetaFPtr.fail()) throw ThetaFileName;  // the exception being checked

        ThetaHeaderFPtr.seekp(0, ios::beg);
        ThetaHeaderFPtr.write((char*)this, sizeof(Theta));
        ThetaHeaderFPtr.close();

        AvailCVIDListFPtr.seekp(0, ios::beg);
        AvailCVIDListFPtr.write((char*)&NoOfContactVectors, sizeof(NoOfContactVectors));

        ThetaFPtr.seekp(0, ios::beg);
        ThetaFPtr.write((char*)&NoOfContactVectors, sizeof(NoOfContactVectors));
 
        C.GetByteVector(ByteVec);
        n = C.CVSizeInBytes();
        char* Str = (char *)malloc(n + 1);
        if(n != ByteVec.size()){
            std::cout<<"\nError in Theta::Theta(parN,parq, ...) - Vector Size Error"<<std::endl;
            std::exit(1);
        }
        for (i = 0; i < n; i++)
            *(Str + i) = ByteVec[i];// + 'A';
        std::cout << "Please Wait: Creating Records ......" << std::endl;
        ThetaFPtr.seekp(0, ios::end);
        AvailCVIDListFPtr.seekp(0, ios::end);
        
        for (CVID = 0; CVID < NoOfContactVectors; CVID++){
            ThetaFPtr.write(Str, n);
            AvailCVIDListFPtr.write((char*)&CVID, sizeof(long long));
            ThetaFPtr.seekp(0, ios::end);
            AvailCVIDListFPtr.seekp(0, ios::end);
        }
        ThetaFPtr.close();
        AvailCVIDListFPtr.close();
        std::cout << "\nNew Theta Object created successfully." << std::endl;
        return;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Theta::Theta(parN,parq, ...) " << e.what() << "- Exception opening/reading/closing file\n" << std::endl;
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Theta::Theta(parN,parq, ...) " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Theta::Theta(parN,parq, ...) " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Theta::Theta(parN,parq, ...) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Theta::Theta(parN,parq, ...) " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

ostream& operator<<(ostream& parOut, const Theta& parTH){
    parOut << "\nDescription of the Theta"<<std::endl;
    parOut << "Header File Name (ThetaFileHeaderName):" << parTH.ThetaFileHeaderName << std::endl;
    parOut << "File Name (ThetaFileName):" << parTH.ThetaFileName << std::endl;
    parOut << "File Name (AvailCVFileName):" << parTH.AvailCVIDListFileName << std::endl;
    parOut << "Number of Observation Days(D):" << parTH.NoOfDays << std::endl;
    parOut << "Minimum time duration for a day(in hours):" << parTH.DayDuration << std::endl;
    parOut << "Minimum time duration for a close contact(in Min):" << parTH.SlotDuration << std::endl;
    parOut << "Length of Contact Vector(c):" << parTH.NoOfCloseContactSlots << std::endl;
    parOut << "N:" << parTH.N << std::endl;
    parOut << "q:" << parTH.q << std::endl;
    parOut << "f:" << parTH.f << std::endl;
    parOut << "Delta:"<<parTH.Delta<<std::endl;
    parOut << "Number of Contact Vectors:" << parTH.NoOfContactVectors << "(Calculated)" << std::endl;
    std::fstream ThetaFPtr;
    ThetaFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream AvailCVIDListFPtr;
    AvailCVIDListFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    long long CVID=0,ThetaFlSize, ReadRecCount,CVIDRead;
    int i,Size;
    char* Str;
    std::vector<char> ByteVec;
    ContactVector C(parTH.NoOfDays, parTH.DayDuration, parTH.SlotDuration);
    try{
        ThetaFPtr.open(parTH.ThetaFileName, ios::in | ios::binary);
        if (ThetaFPtr.fail()) throw parTH.ThetaFileName;  // the exception being checked

        ThetaFPtr.seekg(0,ios::end);
        ThetaFlSize = ThetaFPtr.tellg();
        ThetaFPtr.seekg(0,ios::beg);

        ThetaFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        ThetaFlSize-=sizeof(ReadRecCount);
        parOut << "Number of Contact Vectors:" << ReadRecCount << "(Stored)" << std::endl;
        
        Size = C.CVSizeInBytes();
        parOut<<"File size of Theta:"<<ThetaFlSize<<"(calculated:";
        parOut<<parTH.NoOfContactVectors*Size<<")"<<std::endl;
        Str = (char *)malloc(Size+1);
        while(true){
            parOut << "Sl No:" << CVID << std::endl;   
            ByteVec.clear();
            ByteVec.reserve(Size);
            if(ThetaFPtr.good()){
                ThetaFPtr.read(Str, Size);
                for (i = 0; i < Size; i++){
                    ByteVec.push_back(*(Str + i));
                }
                if (!C.SetUseByteVector(ByteVec)){
                    parOut<<"\n Theta::operator<<() Unable to convert from byte to bits for Contact Vector"<<std::endl;
                    std::exit(1);
                }
                parOut << C << std::endl;
                //parOut<<"ThetaFlSize="<<ThetaFlSize<<std::endl;

                ThetaFlSize = ThetaFlSize - Size;
                
                if(ThetaFlSize==0){
                    ThetaFPtr.close();
                    break;
                }

                if(ThetaFPtr.eof()){
                    ThetaFPtr.close();
                    break;
                }
                //parOut<<"ThetaFlSize="<<ThetaFlSize<<std::endl;
            }
            else{
                ThetaFPtr.close();
                parOut<<"\nThetaFPtr not in good state"<<std::endl;
                std::exit(1);
            }
            CVID++;
        }    
        AvailCVIDListFPtr.open(parTH.AvailCVIDListFileName, ios::in | ios::binary);
        if (AvailCVIDListFPtr.fail()) throw parTH.AvailCVIDListFileName;  // the exception being checked

        AvailCVIDListFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        parOut << "No of records in AvailCVIDList=" << ReadRecCount << std::endl;
        
        for (CVID = 0; CVID < ReadRecCount; CVID++){
            AvailCVIDListFPtr.read((char*)&CVIDRead, sizeof(long long));
            parOut << "CVID=" << CVIDRead << std::endl;
        }
        AvailCVIDListFPtr.seekg(0,ios::end);
        parOut<<"File size of available CVID list"<<AvailCVIDListFPtr.tellg()<<"(Calculated:";
        parOut<<ReadRecCount*sizeof(long long)<<")"<<std::endl;
        AvailCVIDListFPtr.close();
        parOut<<"\nSuccessful file closing"<<std::endl;
        return parOut;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Theta::operator<<() " << e.what() << " - Exception opening/reading/closing file\n";   
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Theta::operator<<() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Theta::operator<<() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Theta::operator<<() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Theta::operator<<() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

int Theta::GetDelta(){
    return Delta;
}

void Theta::Show(){
    std::cout << "\nDescription of the Theta"<<std::endl;
    std::cout << "Header File Name (ThetaFileHeaderName):" << ThetaFileHeaderName << std::endl;
    std::cout << "File Name (ThetaFileName):" << ThetaFileName << std::endl;
    std::cout << "File Name (AvailCVFileName):" << AvailCVIDListFileName << std::endl;
    std::cout << "Number of Observation Days(D):" << NoOfDays << std::endl;
    std::cout << "Minimum time duration for a day(in hours):" << DayDuration << std::endl;
    std::cout << "Minimum time duration for a close contact(in Min):" << SlotDuration << std::endl;
    std::cout << "Length of Contact Vector(c):" << NoOfCloseContactSlots << std::endl;
    std::cout << "N:" << N << std::endl;
    std::cout << "q:" << q << std::endl;
    std::cout << "f:" << f << std::endl;
    std::cout << "Delta:"<<Delta<<std::endl;
    std::cout << "Number of Contact Vectors:" << NoOfContactVectors << "(Calculated)" << std::endl;
    std::fstream ThetaFPtr;
    ThetaFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream AvailCVIDListFPtr;
    AvailCVIDListFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    long long CVID = 0,ThetaFlSize,ReadRecCount,CVIDRead;
    ContactVector C(NoOfDays, DayDuration, SlotDuration);
    int Size,i;
    std::vector<char> ByteVec;
    char* Str;
    try{
        ThetaFPtr.open(ThetaFileName, ios::in | ios::binary);
        if (ThetaFPtr.fail()) throw ThetaFileName;  // the exception being checked
        ThetaFPtr.seekg(0,ios::end);
        ThetaFlSize = ThetaFPtr.tellg();
        ThetaFPtr.seekg(0,ios::beg);

        ThetaFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        ThetaFlSize-=sizeof(ReadRecCount);
        std::cout << "Number of Contact Vectors:" << ReadRecCount << "(Stored)" << std::endl;
        Size = C.CVSizeInBytes();
        Str = (char *)malloc(Size+1);
        while(true){
            std::cout << "Sl No:" << CVID << std::endl;   
            ByteVec.clear();
            ByteVec.reserve(Size);
            if(ThetaFPtr.good()){
                ThetaFPtr.read(Str, Size);
                for (i = 0; i < Size; i++)ByteVec.push_back(*(Str + i));
                if (!C.SetUseByteVector(ByteVec)){
                    ThetaFPtr.close();
                    std::cout<<"\n Theta::operator<<() Unable to convert from byte to bits for Contact Vector"<<std::endl;
                    std::exit(1);
                }
                std::cout << C << std::endl;
                std::cout<<"ThetaFlSize="<<ThetaFlSize<<std::endl;

                ThetaFlSize = ThetaFlSize - Size;
                
                if(ThetaFlSize==0){
                    ThetaFPtr.close();
                    break;
                }
                if(ThetaFPtr.eof()){
                    ThetaFPtr.close();
                    break;
                }
                std::cout<<"ThetaFlSize="<<ThetaFlSize<<std::endl;
            }
            else{
                ThetaFPtr.close();
                std::cout<<"\nThetaFPtr not in good state"<<std::endl;
                std::exit(1);
            }
            CVID++;
        }       

        AvailCVIDListFPtr.open(AvailCVIDListFileName, ios::in | ios::binary);
        if (AvailCVIDListFPtr.fail()) throw AvailCVIDListFileName;  // the exception being checked

        AvailCVIDListFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        std::cout << "No of records in AvailCVList=" << ReadRecCount << std::endl;
    
        for (CVID = 0; CVID < ReadRecCount; CVID++){
            AvailCVIDListFPtr.read((char*)&CVIDRead, sizeof(  long long));
            std::cout << "CVID=" << CVIDRead << std::endl;
        }        
        AvailCVIDListFPtr.close();

        std::cout<<"\nSuccessful file closing"<<std::endl;
        return;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Theta::operator<<() " << e.what() << " - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Theta::operator<<() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Theta::operator<<() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Theta::operator<<() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Theta::operator<<() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}


/*Contact Vector in byte string form is used because bool type vector is stored in byte string form, i.e. for each
true / false value one byte is stored. However, in byte string each byte stores eight number of bools, so it is a
compact and optimized contact vector in the disk storage, whereas in memory it is in bool form for quick access.
*/

bool Theta::GetCV(long long parCVID, ContactVector& parCV) {
    std::fstream ThetaFPtr;
    ThetaFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    long long CPos, ReadRecCount;    
    int n,i;    
    char* Str;
    vector<char> ByteVec;
    try{
        ThetaFPtr.open(ThetaFileName, ios::in | ios::binary);
        if (ThetaFPtr.fail()) throw ThetaFileName;  // the exception being checked

        ThetaFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        n = ceil((double)parCV.CVSize() / 8.0);
        CPos = sizeof(ReadRecCount) + parCVID * n; //Assuming 1st ContactVector has array index 0.   
        Str = (char *) malloc(n + 1);
        ThetaFPtr.seekg(CPos, ios::beg);
        //std::cout << "Next Vector's File Position(ThetaFPtr.tellg()):" << ThetaFPtr.tellg() << std::endl;
        ThetaFPtr.read(Str, n); //Fetch the contact vector in byte string form
        ThetaFPtr.close();

        for (i = 0; i < n; i++) ByteVec.push_back(Str[i]);
        parCV.SetUseByteVector(ByteVec);

        return true; //return the boolean contact vector 
    }
    catch (std::fstream::failure &e){
        std::cerr << "Theta::GetCV(parCVID,parCV) " << e.what() << "- Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Theta::GetCV(parCVID,parCV) " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Theta::GetCV(parCVID,parCV) " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Theta::GetCV(parCVID,parCV) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Theta::GetCV(parCVID,parCV) " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Theta::SetCV(long long parCVID, ContactVector& parCV) {
    //For current Theta the size of a contact vector in bytes
    int n,i;
    long long ReadRecCount, CPos;
    char* Str;
    vector<char> ByteVec;
    std::fstream ThetaFPtr;
    ThetaFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

    try{
        n = (NoOfDays * DayDuration);
        if (SlotDuration == 0)
            throw std::overflow_error("Divide by zero exception");
        n = ceil((double)(n * 60.0) / SlotDuration);
        n += NoOfDays;
        if (parCV.CVSize() != n){
            std::cout << "Error in Theta::SetCV() - Incorrect Contact Vector length" << std::endl;
            return false;
        }
        ThetaFPtr.open(ThetaFileName, ios::in | ios::out | ios::binary);
        if (ThetaFPtr.fail()) throw ThetaFileName;  // the exception being checked
        
        ThetaFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        n = ceil((double)parCV.CVSize() / 8.0);
        parCV.GetByteVector(ByteVec);
        CPos = sizeof(ReadRecCount) + parCVID * n; //Assuming 1st ContactVector has array index 0.   
        Str = (char *)malloc(n + 1);
        for (i = 0; i < n; i++)
            *(Str + i) = ByteVec[i];

        ThetaFPtr.seekp(CPos, ios::beg);
        //std::cout << "Next Vector's File Position(ThetaFPtr.tellg()):" << ThetaFPtr.tellg() << std::endl;
        ThetaFPtr.write(Str, n); //Fetch the contact vector in byte string form
        ThetaFPtr.close();
        return true; //return the boolean contact vector 
    }
    catch (std::fstream::failure &e){
        std::cerr << "Theta::SetCV(parCVID,parCV) " << e.what() << "- Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Theta::SetCV(parCVID,parCV) " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Theta::SetCV(parCVID,parCV) " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Theta::SetCV(parCVID,parCV) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Theta::SetCV(parCVID,parCV) " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

//Size is the multiple of largest data type among the different fields / attributes
int Theta::Size(){
    return sizeof(Theta);
}


void Theta::StorageSize(long long &parCalSize, long long &parObsrvSize){
    /* This size is the requirement of the secondary / disk storage for the Theta structure.
    * It is the sum of the space requirement for available contact vector list
    * and the actual contact vector storage.
    */
    long long ReadRecCount;
    long long ThetaFlCalSize = 0, ThetaFlObsrvSize = 0;
    long long ThetaHeaderFlCalSize = 0, ThetaHeaderFlObsrvSize = 0;
    long long ThetaAvlListFlCalSize = 0, ThetaAvlListFlObsrvSize = 0;

    ContactVector CV(NoOfDays, DayDuration, SlotDuration);

    ThetaFlCalSize = sizeof(NoOfContactVectors)+NoOfContactVectors * CV.CVSizeInBytes();
    
    ThetaAvlListFlCalSize = sizeof(NoOfContactVectors)+(NoOfContactVectors-NoOfUsedContactVectors)*sizeof(NoOfContactVectors);

    ThetaHeaderFlCalSize = sizeof(Theta);

    parCalSize = ThetaHeaderFlCalSize+ThetaAvlListFlCalSize+ThetaFlCalSize;
    
    std::fstream ThetaHeaderFPtr;
    ThetaHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream ThetaFPtr;
    ThetaFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream AvailCVIDListFPtr;
    AvailCVIDListFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    try {
        ThetaFPtr.open(ThetaFileName, ios::in | ios::binary);
        if (ThetaFPtr.fail()) throw ThetaFileName;  // the exception being checked
        ThetaFPtr.seekg(0, ios::end);
        ThetaFlObsrvSize = ThetaFPtr.tellg();
        ThetaFPtr.close();

        ThetaHeaderFPtr.open(ThetaFileHeaderName, ios::in | ios::binary);
        if (ThetaHeaderFPtr.fail()) throw ThetaFileHeaderName;  // the exception being checked
        ThetaHeaderFPtr.seekg(0, ios::end);
        ThetaHeaderFlObsrvSize = ThetaHeaderFPtr.tellg();
        ThetaHeaderFPtr.close();

        AvailCVIDListFPtr.open(AvailCVIDListFileName, ios::in | ios::binary);
        if (AvailCVIDListFPtr.fail()) throw AvailCVIDListFileName;  // the exception being checked        

        AvailCVIDListFPtr.read((char *)&ReadRecCount, sizeof(ReadRecCount));
        ThetaHeaderFlObsrvSize = ReadRecCount * sizeof(long long);
        /*
        Used CVIDs are not removed from this file. But, we can remove them. 
        Here we are not considering these for file size measurement. 
        If we like to use then above code will be replaced by
        //AvailCVIDListFPtr.seekg(0, ios::end);
        //ThetaFlSize += AvailCVIDListFPtr.tellg();
        */
        AvailCVIDListFPtr.close();
        parObsrvSize = ThetaHeaderFlObsrvSize+ThetaFlObsrvSize+ThetaAvlListFlObsrvSize;

        std::cout<<"ThetaHeaderFlSize(Cal,Obsrv)=("<<ThetaHeaderFlCalSize<<","<<ThetaHeaderFlObsrvSize<<")"<<std::endl;
        std::cout<<"ThetaFlSize(Cal,Obsrv)=("<<ThetaFlCalSize<<","<<ThetaFlObsrvSize<<")"<<std::endl;
        std::cout<<"ThetaAvlListFlSize(Cal,Obsrv)=("<<ThetaAvlListFlCalSize<<","<<ThetaAvlListFlObsrvSize<<")"<<std::endl;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Theta::StorageSize() " << e.what() << " - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Theta::StorageSize() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Theta::StorageSize() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Theta::StorageSize() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Theta::StorageSize() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Theta::GetEmptyCV(long long& parCVID){
    /* If Available Contact Vector list i.e. corresponding binary file is not empty, then
    * last available ID of this list is fetched as an empty Contact Vector.
    * The ID of this empty contact vector is passed as an argument of this function.
    */
    std::fstream AvailCVIDListFPtr;
    AvailCVIDListFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream ThetaHeaderFPtr;
    ThetaHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

    long long CVCount,Pos;
    try{
        ThetaHeaderFPtr.open(ThetaFileHeaderName, ios::in | ios::binary);
        if (ThetaHeaderFPtr.fail()) throw ThetaFileHeaderName;  // the exception being checked

        ThetaHeaderFPtr.read((char*)this, sizeof(Theta));
        ThetaHeaderFPtr.close();

        AvailCVIDListFPtr.open(AvailCVIDListFileName, ios::in | ios::binary);
        if (AvailCVIDListFPtr.fail()) throw AvailCVIDListFileName;  // the exception being checked
        AvailCVIDListFPtr.read((char*)&CVCount, sizeof(CVCount));
        AvailCVIDListFPtr.close();
        if (CVCount == 0){
            if(!AddCV(parCVID)){
                std::cout << "Error in Theta::GetEmptyCV() - No more empty / unused Contact Vector in the store" << std::endl;
                return false;
            }
            NoOfUsedContactVectors++;
            ThetaHeaderFPtr.open(ThetaFileHeaderName, ios::in|ios::out|ios::binary);
            if (ThetaHeaderFPtr.fail()) throw ThetaFileHeaderName;  // the exception being checked

            ThetaHeaderFPtr.write((char*)this, sizeof(Theta));
            ThetaHeaderFPtr.close();
            return true;            
        }

        AvailCVIDListFPtr.open(AvailCVIDListFileName, ios::in | ios::out | ios::binary);
        if (AvailCVIDListFPtr.fail()) throw AvailCVIDListFileName;  // the exception being checked

        //Take index of the last empty contact vector
        Pos = sizeof(CVCount) + (CVCount - 1) * sizeof(CVCount); //Assuming 1st ContactVector has array index 0.
        AvailCVIDListFPtr.seekg(Pos, ios::beg);
        AvailCVIDListFPtr.read((char*)&parCVID, sizeof(parCVID));
        AvailCVIDListFPtr.seekp(0, ios::beg);
        CVCount--;
        AvailCVIDListFPtr.write((char*)&CVCount, sizeof(CVCount));
        AvailCVIDListFPtr.close();

        NoOfUsedContactVectors++;

        ThetaHeaderFPtr.open(ThetaFileHeaderName, ios::in|ios::out|ios::binary);
        if (ThetaHeaderFPtr.fail()) throw ThetaFileHeaderName;  // the exception being checked

        ThetaHeaderFPtr.write((char*)this, sizeof(Theta));
        ThetaHeaderFPtr.close();
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Theta::GetEmptyCV(parCVID) " << e.what() << "- Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Theta::GetEmptyCV(parCVID) " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Theta::GetEmptyCV(parCVID) " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Theta::GetEmptyCV(parCVID) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Theta::GetEmptyCV(parCVID) " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Theta::FreeCV(long long parCVID){
    /* parCVID should hold a valid Contact Vector ID.
    * Both Day Contacts and detail close contacts of the vector must be set to false.
    * And finally this contact vector is added to the available list so that it can be
    * reused.
    */
    if (parCVID < 0 || parCVID > NoOfContactVectors){
        std::cout << "Error in Theta::FreeCV(parCVID) in valid Contact Vector ID" << std::endl;
        return false;
    }
    std::fstream ThetaHeaderFPtr;
    ThetaHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream ThetaFPtr;
    ThetaFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream AvailCVIDListFPtr;
    AvailCVIDListFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    long long CPos, CVCount;
    ContactVector CV(NoOfDays, DayDuration, SlotDuration);
    int n,i;
    std::vector<char> ByteVec;
    char* Str;
    try{
        ThetaHeaderFPtr.open(ThetaFileHeaderName, ios::in | ios::binary);
        if (ThetaHeaderFPtr.fail()) throw ThetaFileHeaderName;  // the exception being checked

        ThetaHeaderFPtr.read((char*)this, sizeof(Theta));
        ThetaHeaderFPtr.close();

        AvailCVIDListFPtr.open(AvailCVIDListFileName, ios::in | ios::binary);
        if (AvailCVIDListFPtr.fail()) throw AvailCVIDListFileName;  // the exception being checked
        AvailCVIDListFPtr.close();

        AvailCVIDListFPtr.open(AvailCVIDListFileName, ios::in | ios::out | ios::binary);
        if (AvailCVIDListFPtr.fail()) throw AvailCVIDListFileName;  // the exception being checked

        AvailCVIDListFPtr.read((char*)&CVCount, sizeof(CVCount));
        CVCount++;
        AvailCVIDListFPtr.seekp(0, ios::beg);
        AvailCVIDListFPtr.write((char*)&CVCount, sizeof(CVCount));
        AvailCVIDListFPtr.seekp(0, ios::end);
        AvailCVIDListFPtr.write((char*)&parCVID, sizeof(parCVID));
        AvailCVIDListFPtr.close();

        n = ceil((double)CV.CVSize() / 8.0);
        CV.GetByteVector(ByteVec);
        
        CPos = sizeof(long long) + parCVID * n; //Assuming 1st ContactVector has array index 0.   
        Str = (char *) malloc(n + 1);
        for (i = 0; i < n; i++)
            *(Str + i) = ByteVec[i];

        ThetaFPtr.open(ThetaFileName, ios::in | ios::binary);
        if (ThetaFPtr.fail()) throw ThetaFileName;  // the exception being checked
        ThetaFPtr.close();

        ThetaFPtr.open(ThetaFileName, ios::in | ios::out | ios::binary);
        if (ThetaFPtr.fail()) throw ThetaFileName;  // the exception being checked

        ThetaFPtr.seekp(CPos, ios::beg);
        ThetaFPtr.write(Str, n); //Fetch the contact vector in byte string form
        ThetaFPtr.close();

        NoOfUsedContactVectors--;

        ThetaHeaderFPtr.open(ThetaFileHeaderName, ios::in|ios::out|ios::binary);
        if (ThetaHeaderFPtr.fail()) throw ThetaFileHeaderName;  // the exception being checked

        ThetaHeaderFPtr.write((char*)this, sizeof(Theta));
        ThetaHeaderFPtr.close();

        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Theta::FreeCV(parCVID) " << e.what() << "- Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Theta::FreeCV(parCVID) " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Theta::FreeCV(parCVID) " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Theta::FreeCV(parCVID) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Theta::FreeCV(parCVID) " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

int Theta::GetNoOfCloseContactSlots(){
    return NoOfCloseContactSlots;
}
int Theta::GetNoOfDays(){
    return NoOfDays;
}

int Theta::GetSlotDuration(){
    return SlotDuration;
}

int Theta::GetDayDuration(){
    return DayDuration;
}

long long Theta::GetNoOfContactVectors(){
    return NoOfContactVectors;
}

long Theta::GetN(){
    return N;
}

long Theta::Getq(){
    return q;
}

bool Theta::AddCV(long long &parCVID){
    /* This function adds a number of empty contact vectors at the end of the binary data file
    * used to store all contact vectors in the system.
    * Added contact vectors are also added to the available list.
    * This updates the count of the number of contact vectors in the system.
    */
    //std::cout << "Theta::AddCV() operation ......";    
    std::fstream ThetaHeaderFPtr;
    ThetaHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream ThetaFPtr;
    ThetaFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    ContactVector C(NoOfDays, DayDuration, SlotDuration);
    int n;
    char* Str;
    std::vector<char> ByteVec;
    long long ReadRecCount, Pos;
    try {
        ThetaHeaderFPtr.open(ThetaFileHeaderName, ios::in | ios::binary);
        if (ThetaHeaderFPtr.fail()) throw ThetaFileHeaderName;  // the exception being checked

        ThetaHeaderFPtr.read((char*)this, sizeof(Theta));
        ThetaHeaderFPtr.close();

        ThetaFPtr.open(ThetaFileName, ios::in | ios::binary);
        if (ThetaFPtr.fail()) throw ThetaFileName;  // the exception being checked
        ThetaFPtr.read((char *)&ReadRecCount, sizeof(ReadRecCount));
        ThetaFPtr.close();
        if (ReadRecCount != NoOfContactVectors){
            std::cout<<"\nError:Theta::AddCV() mismatch in record count"<<std::endl;
            return false;
        }
        ThetaHeaderFPtr.open(ThetaFileHeaderName, ios::in | ios::out | ios::binary);
        if (ThetaHeaderFPtr.fail()) throw ThetaFileHeaderName;  // the exception being checked
        
        ThetaFPtr.open(ThetaFileName, ios::in | ios::out | ios::binary);
        if (ThetaFPtr.fail()) throw ThetaFileName;  // the exception being checked

        n = ceil((double)C.CVSize() / 8.0);
        Str = (char *)malloc(n + 1);

        //Check this sequential code for alternative
        C.GetByteVector(ByteVec);
        for (int i = 0; i < n; i++)
            *(Str + i) = ByteVec[i];
        
        Pos = sizeof(long long)+NoOfContactVectors*n;
        ThetaFPtr.seekg(Pos, ios::beg);
        ThetaFPtr.write(Str, n);
        if (ThetaFPtr.fail()){
            ThetaFPtr.close();
            ThetaHeaderFPtr.close();
            std::cout<<"\nError:Theta::AddCV() Unable add new CV record"<<std::endl;
            return false;
        }
        parCVID = NoOfContactVectors;

        NoOfContactVectors++;

        ThetaHeaderFPtr.seekg(0, ios::beg);
        ThetaHeaderFPtr.write((char*)this, sizeof(Theta));
        ThetaHeaderFPtr.close();

        ThetaFPtr.seekg(0, ios::beg);
        ThetaFPtr.write((char*)&NoOfContactVectors, sizeof(NoOfContactVectors));
        ThetaFPtr.close();
       
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Theta::AddCV() " << e.what() << "- Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Theta::AddCV() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Theta::AddCV() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Theta::AddCV() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Theta::AddCV() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Theta::AddCV(){
    /* This function adds a number of empty contact vectors at the end of the binary data file
    * used to store all contact vectors in the system.
    * Added contact vectors are also added to the available list.
    * This updates the count of the number of contact vectors in the system.
    */
    //std::cout << "Theta::AddCV() operation ......";
    
    std::fstream ThetaHeaderFPtr;
    ThetaHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream ThetaFPtr;
    ThetaFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream AvailCVIDListFPtr;
    AvailCVIDListFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    ContactVector C(NoOfDays, DayDuration, SlotDuration);
    int n;
    char* Str;
    std::vector<char> ByteVec;
    long long ReadRecCount, Pos;
    try {
        ThetaHeaderFPtr.open(ThetaFileHeaderName, ios::in | ios::binary);
        if (ThetaHeaderFPtr.fail()) throw ThetaFileHeaderName;  // the exception being checked

        ThetaHeaderFPtr.read((char*)this, sizeof(Theta));
        ThetaHeaderFPtr.close();

        AvailCVIDListFPtr.open(AvailCVIDListFileName, ios::in | ios::binary);
        if (AvailCVIDListFPtr.fail()) throw AvailCVIDListFileName;  // the exception being checked
        AvailCVIDListFPtr.close();

        ThetaFPtr.open(ThetaFileName, ios::in | ios::binary);
        if (ThetaFPtr.fail()) throw ThetaFileName;  // the exception being checked
        ThetaFPtr.read((char *)&ReadRecCount, sizeof(ReadRecCount));
        ThetaFPtr.close();
        if (ReadRecCount != NoOfContactVectors){
            std::cout<<"\nError:Theta::AddCV() mismatch in record count"<<std::endl;
            return false;
        }

        ThetaHeaderFPtr.open(ThetaFileHeaderName, ios::in | ios::out | ios::binary);
        if (ThetaHeaderFPtr.fail()) throw ThetaFileHeaderName;  // the exception being checked
        
        AvailCVIDListFPtr.open(AvailCVIDListFileName, ios::in | ios::out | ios::binary);
        if (AvailCVIDListFPtr.fail()) throw AvailCVIDListFileName;  // the exception being checked

        ThetaFPtr.open(ThetaFileName, ios::in | ios::out | ios::binary);
        if (ThetaFPtr.fail()) throw ThetaFileName;  // the exception being checked

        n = ceil((double)C.CVSize() / 8.0);
        Str = (char *)malloc(n + 1);
        
        C.GetByteVector(ByteVec);
        for (int i = 0; i < n; i++)
            *(Str + i) = ByteVec[i];

        Pos = sizeof(long long)+NoOfContactVectors*n;
        ThetaFPtr.seekg(Pos, ios::beg);
        ThetaFPtr.write(Str, n);
        if (ThetaFPtr.fail()){
            ThetaFPtr.close();
            AvailCVIDListFPtr.close();
            ThetaHeaderFPtr.close();
            std::cout<<"\nError:Theta::AddCV() Unable add new CV record"<<std::endl;
            return false;
        }
        AvailCVIDListFPtr.seekg(0, ios::beg);
        AvailCVIDListFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        Pos = sizeof(long long)+ReadRecCount*sizeof(long long);
        AvailCVIDListFPtr.seekg(Pos, ios::beg);
        /* Before add the value in NoOfContactVectors is the index or ID of the added
        contact vector.*/
        AvailCVIDListFPtr.write((char*)&NoOfContactVectors, sizeof(NoOfContactVectors));
        /* Increase count of the available CV by one and save it*/
        ReadRecCount++;
        AvailCVIDListFPtr.seekp(0, ios::beg);
        AvailCVIDListFPtr.write((char*)&ReadRecCount, sizeof(ReadRecCount));
        AvailCVIDListFPtr.close();

        /* After add NoOfContactVectors is increased by one*/
        NoOfContactVectors++;
        /* Save the changes of the Theta structure */
        ThetaHeaderFPtr.seekg(0, ios::beg);
        ThetaHeaderFPtr.write((char*)this, sizeof(Theta));
        ThetaHeaderFPtr.close();
        
        /* Value in NoOfContactVectors is also used to store the count of CV in Thete*/
        ThetaFPtr.seekg(0, ios::beg);
        ThetaFPtr.write((char*)&NoOfContactVectors, sizeof(NoOfContactVectors));
        ThetaFPtr.close();
 
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Theta::AddCV() " << e.what() << "- Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Theta::AddCV() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Theta::AddCV() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Theta::AddCV() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Theta::AddCV() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Theta::AddCV(int parNoOfCV){
    /* This function adds a number of empty contact vectors at the end of the binary data file
    * used to store all contact vectors in the system.
    * Added contact vectors are also added to the available list.
    * This updates the count of the number of contact vectors in the system.
    */
    std::cout << "Theta::AddCV(parNoOfCV) operation ......";
    if (parNoOfCV <= 0){
        std::cout << "Error in Theta::AddCV(parNoOfCV) - invalid number contact vector to add" << std::endl;
        return false;
    }
    std::fstream ThetaHeaderFPtr;
    ThetaHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream ThetaFPtr;
    ThetaFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream AvailCVIDListFPtr;
    AvailCVIDListFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    ContactVector C(NoOfDays, DayDuration, SlotDuration);
    int n;
    char* Str;
    std::vector<char> ByteVec;
    long long id,p,ReadRecCount,Pos;
    try {
        ThetaHeaderFPtr.open(ThetaFileHeaderName, ios::in | ios::binary);
        if (ThetaHeaderFPtr.fail()) throw ThetaFileHeaderName;  // the exception being checked

        ThetaHeaderFPtr.read((char*)this, sizeof(Theta));
        ThetaHeaderFPtr.close();

        AvailCVIDListFPtr.open(AvailCVIDListFileName, ios::in | ios::binary);
        if (AvailCVIDListFPtr.fail()) throw AvailCVIDListFileName;  // the exception being checked
        AvailCVIDListFPtr.close();

        ThetaFPtr.open(ThetaFileName, ios::in | ios::binary);
        if (ThetaFPtr.fail()) throw ThetaFileName;  // the exception being checked
        ThetaFPtr.read((char *)&ReadRecCount, sizeof(ReadRecCount));
        ThetaFPtr.close();
        if (ReadRecCount != NoOfContactVectors){
            std::cout<<"\nError:Theta::AddCV() mismatch in record count"<<std::endl;
            return false;
        }
   
        ThetaHeaderFPtr.open(ThetaFileHeaderName, ios::in | ios::out | ios::binary);
        if (ThetaHeaderFPtr.fail()) throw ThetaFileHeaderName;  // the exception being checked
        
        AvailCVIDListFPtr.open(AvailCVIDListFileName, ios::in | ios::out | ios::binary);
        if (AvailCVIDListFPtr.fail()) throw AvailCVIDListFileName;  // the exception being checked

        ThetaFPtr.open(ThetaFileName, ios::in | ios::out | ios::binary);
        if (ThetaFPtr.fail()) throw ThetaFileName;  // the exception being checked

        n = ceil((double)C.CVSize() / 8.0);
        Str = (char *)malloc(n + 1);
        
        C.GetByteVector(ByteVec);
        for (int i = 0; i < n; i++)
            *(Str + i) = ByteVec[i];
        
        AvailCVIDListFPtr.seekg(0, ios::beg);
        AvailCVIDListFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        Pos = sizeof(long long)+ReadRecCount*sizeof(long long);
        AvailCVIDListFPtr.seekg(Pos, ios::beg);

        Pos = sizeof(long long)+NoOfContactVectors*n;
        ThetaFPtr.seekg(Pos, ios::beg);

        for (p = 0; p < parNoOfCV; p++){
            id = NoOfContactVectors + p;
            ThetaFPtr.write(Str, n);
            AvailCVIDListFPtr.write((char*)&id, sizeof(id));
            if (ThetaFPtr.fail() || AvailCVIDListFPtr.fail()){
                ThetaFPtr.close();
                AvailCVIDListFPtr.close();
                ThetaHeaderFPtr.close();
                std::cout<<"\nError:Theta::AddCV() Unable add new CV record"<<std::endl;
                return false;
            }
        }   
        ReadRecCount += parNoOfCV;
        AvailCVIDListFPtr.seekg(0, ios::beg);
        AvailCVIDListFPtr.write((char*)&ReadRecCount, sizeof(ReadRecCount));
        AvailCVIDListFPtr.close();

        NoOfContactVectors += parNoOfCV;
        ThetaHeaderFPtr.seekg(0, ios::beg);
        ThetaHeaderFPtr.write((char*)this, sizeof(Theta));
        ThetaHeaderFPtr.close();

        ThetaFPtr.seekg(0, ios::beg);
        ThetaFPtr.write((char*)&NoOfContactVectors, sizeof(NoOfContactVectors));
        ThetaFPtr.close();

        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Theta::AddCV(parNoOfCV) " << e.what() << "- Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Theta::AddCV(parNoOfCV) " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Theta::AddCV(parNoOfCV) " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Theta::AddCV(parNoOfCV) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Theta::AddCV(parNoOfCV) " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Theta::RemoveCV(int parNoOfCV){
    /* This is a maintance work. It will be used when there are large number CVs are in the available list.
    * This function will remove a given number of contact vectors from the end of the binary data file for the Theta
    * storage structure. For this different processings are needed. It is possible to know the ID of all such CVs.
    * Now we have find all these CVs. Note that these are either under available list or in the used list.
    * First available list will be checked and all such CVs are shifted towards the end of the list by swapping.
    * If required number of CVs appear in the end of the list then we can remove them easily. Otherwise, we have
    * to get these CVs from the used CVs. For any such CV we have to trace the Phi structure. If found then
    * copy the corresponding CV to an empty CV in the available list and not in the remove list. In this way
    * required CV will be made free and hence we can remove it easily.
    */
    std::cout << "Theta::RemoveCV() operation ......";
    if (parNoOfCV <= 0){
        std::cout << "Error in Theta::RemoveCV() - invalid number contact vector to add" << std::endl;
        return false;
    }
    std::fstream ThetaHeaderFPtr;
    ThetaHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream ThetaFPtr;
    ThetaFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream AvailCVIDListFPtr;
    AvailCVIDListFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

    try {
        ThetaHeaderFPtr.open(ThetaFileHeaderName, ios::in | ios::binary);
        if (ThetaHeaderFPtr.fail()) throw ThetaFileHeaderName;  // the exception being checked
        ThetaHeaderFPtr.close();

        AvailCVIDListFPtr.open(AvailCVIDListFileName, ios::in | ios::binary);
        if (AvailCVIDListFPtr.fail()) throw AvailCVIDListFileName;  // the exception being checked
        AvailCVIDListFPtr.close();

        ThetaFPtr.open(ThetaFileName, ios::in | ios::binary);
        if (ThetaFPtr.fail()) throw ThetaFileName;  // the exception being checked
        ThetaFPtr.close();
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Theta::RemoveCV(parNoOfCV) " << e.what() << "- Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Theta::RemoveCV(parNoOfCV) " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Theta::RemoveCV(parNoOfCV) " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Theta::RemoveCV(parNoOfCV) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Theta::RemoveCV(parNoOfCV) " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

//************************************ End of Theta Library ******************************
