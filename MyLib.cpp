#include "MyLib.h"

using namespace std;

//***************************************************************************************
//*********************************** Phi Library ***************************************
//***************************************************************************************

Phi::Phi(){
    std::cout << "\nCreating Phi object [Phi::Phi()]....." << std::endl;
    std::fstream PhiHeaderFPtr;
    PhiHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream AvailListPhiOvFPtr;
    AvailListPhiOvFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

    long long ReadRecCount;
    try {
        PhiHeaderFPtr.open(PhiFileHeaderName, ios::in | ios::binary);
        if (PhiHeaderFPtr.fail()) throw PhiFileHeaderName;  // the exception being checked

        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiHeaderFPtr.read((char*)this, sizeof(Phi));
        //std::cout << "Object Size[sizeof(Phi)]=" << sizeof(Phi) << std::endl;
        //std::cout << "File Size[PhiHeaderFPtr.tellg()]=" << PhiHeaderFPtr.tellg() << "(" << PhiFileHeaderName << ")" << std::endl;
        PhiHeaderFPtr.close();
        
        PhiFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        if (NoOfPhiRec != ReadRecCount){
            PhiFPtr.close();
            std::cout << "Error in Phi::Phi() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }
        //std::cout << "Object Size[sizeof(R)]=" << sizeof(R) << " NoOfPhiRec=" << NoOfPhiRec << std::endl;
        //std::cout << "File Size[PhiFPtr.tellg()]=" << PhiFPtr.tellg() << "(" << PhiFileName << ")" << std::endl;
        PhiFPtr.close();

        AvailListPhiOvFPtr.open(PhiOvAvailListFileName, ios::in | ios::binary);
        if (AvailListPhiOvFPtr.fail()) throw PhiOvAvailListFileName;  // the exception being checked

        AvailListPhiOvFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        if (NoOfPhiAvailOvRec != ReadRecCount){
            AvailListPhiOvFPtr.close();
            std::cout << "Error in Phi::Phi() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }
        AvailListPhiOvFPtr.close();
        std::cout<<*this<<std::endl;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::Phi() opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::Phi() " << e << " error in opening" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::Phi() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::Phi() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::Phi() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
    std::cout << "\nExisting Phi Object created successfully." << std::endl;
}


Phi::Phi(long parN,long parq,float parf){
    std::cout << "\nCreating Phi Object[Phi::Phi(parN, parq)]....." << std::endl;
    N = parN;
    q = parq;
    f = parf;
    long Q = ceil(f * q);
    NoOfPhiRec = (N * Q); // 8 bytes - as (u,v) and (v,u) both are stored

    std::fstream PhiHeaderFPtr;
    PhiHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream AvailListPhiOvFPtr;
    AvailListPhiOvFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

    PhiRec R;
    int RSize = R.Size();
    R.SetDelFlag(true);
    R.SetUpdateFlag(false);
    R.SetCVID(-1L);
    R.SetNeighborID(-1L);
    long PersonID, Degree=0, i;
    long long OvStart= -1L;
    
    try {
        PhiHeaderFPtr.open(PhiFileHeaderName, ios::out | ios::binary);
        if (PhiHeaderFPtr.fail()) throw PhiFileHeaderName;  // the exception being checked

        PhiHeaderFPtr.write((char*)this, sizeof(Phi));
        PhiHeaderFPtr.close();

        PhiFPtr.open(PhiFileName, ios::out | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.write((char*)&NoOfPhiRec, sizeof(NoOfPhiRec));
        std::cout << "Please Wait: Creating Records ......" << std::endl;
        
        for (PersonID = 0; PersonID < N; PersonID++)
        {   
            //Number of adjacent nodes or degree, initially it is zero.
            PhiFPtr.write((char *)&Degree, sizeof(Degree));
            for(i=0;i<Q;i++)PhiFPtr.write((char*)&R, RSize);
            //Starting pointer index for the overflow records
            PhiFPtr.write((char *)&OvStart, sizeof(OvStart));
        }
        PhiFPtr.close();

        AvailListPhiOvFPtr.open(PhiOvAvailListFileName, ios::out | ios::binary);
        if (AvailListPhiOvFPtr.fail()) throw PhiOvAvailListFileName;  // the exception being checked
        
        AvailListPhiOvFPtr.write((char *)&NoOfPhiAvailOvRec, sizeof(NoOfPhiAvailOvRec));
        AvailListPhiOvFPtr.close();

        std::cout<<*this<<std::endl;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::Phi(parN, parq, parf) - opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::Phi(parN, parq, parf) " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::Phi(parN, parq, parf) " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::Phi(parN, parq, parf) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::Phi(parN, parq, parf) " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
    std::cout << "\nNew Phi Object created successfully." << std::endl;
}

ostream& operator<<(ostream& out, const Phi& parPhi){
    long i, Q = ceil(parPhi.f * parPhi.q);
    out << std::endl;
    out << "File Name for Phi(PhiFileName):" << parPhi.PhiFileName << std::endl;
    out << "Size of the Population(N):" << parPhi.N << std::endl;
    out << "Average size of association(q):" << parPhi.q << std::endl;
    out << "Factor of Average size of association(f):" << parPhi.f << std::endl;
    out << "Q(=q * f):"<< Q <<std::endl;
    out << "No of Phi Records(NoOfPhiRec):" << parPhi.NoOfPhiRec << std::endl;
    out << "No of Overflow Records(Now)(NoOfPhiOvRec):"<<parPhi.NoOfPhiOvRec<<std::endl;
    out << "No of Overflow Records Available(Now)(NoOfPhiAvailOvRec):"<<parPhi.NoOfPhiAvailOvRec<<std::endl;
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream AvailListPhiOvFPtr;
    AvailListPhiOvFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

    PhiRec R;
    int RSize = R.Size();
    PhiRecOverflow R1;
    int R1Size = R1.Size();
    long long ReadRecCount, Pos;
    long UID, Count=0;
    long long OvStart=-1L;
    out<<"Size of Phi Records(RSize):"<<RSize<<"(in Bytes)"<<std::endl;
    out<<"Size of Overflow Phi Records(R1Size):"<<R1Size<<"(in Bytes)"<<std::endl;

    try{
        PhiFPtr.open(parPhi.PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw parPhi.PhiFileName;  // the exception being checked
        
        PhiFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        out<<"\nNo of Phi Records (from stored)(ReadRecCount):"<<ReadRecCount<<std::endl;
        
        if (ReadRecCount != parPhi.NoOfPhiRec){
            PhiFPtr.close();
            std::cout << "Error in Phi::operator<<() - mismatch in number of records" << std::endl;
            std::exit(1);
        }        
        for (UID = 0; UID < parPhi.N; UID++)
        {   
            //Number of adjacent nodes or degree, initially it is zero.
            PhiFPtr.read((char *)&Count, sizeof(Count));
            out << "Degree:" << Count << std::endl;
            for(i = 0;i < Q;i++){
                PhiFPtr.read((char*)&R, RSize);
                out <<"R:"<< R << std::endl;
            }
            PhiFPtr.read((char *)&OvStart, sizeof(OvStart));
            out << "Start index for overflow area(OvStart):"<<OvStart<<std::endl;
        }
        Pos = sizeof(long long);
        //Pos += (sizeof(long)+Q*RSize+sizeof(long long))*parPhi.N;
        //Pos += (sizeof(long)+sizeof(long long))*parPhi.N+parPhi.NoOfPhiRec*RSize;
        Pos += (sizeof(long)+Q*RSize+sizeof(long long))*parPhi.N;

        PhiFPtr.seekg(Pos, ios::beg);
        out << "\nRecords in Overflow Area"<<std::endl;
        for (long long i = 0; i < parPhi.NoOfPhiOvRec; i++){
            PhiFPtr.read((char*)&R1, R1Size);
            out <<"R1:"<< R1 << std::endl;
        }
        //out << "Size of Phi Record(normal)="<<RSize<<std::endl;
        //out << "Size of Phi Record(overflow)="<<R1Size<<std::endl;
        PhiFPtr.seekg(0,ios::end);
        out<<"File Size for Phi(PhiFPtr.tellg()):"<<PhiFPtr.tellg()<<"(observed)(Pos:";
        Pos =sizeof(long long)+(sizeof(long)+Q*RSize+sizeof(long long))*parPhi.N; 
        Pos +=parPhi.NoOfPhiOvRec*R1Size;
        out<<Pos<<")"<<std::endl;
        PhiFPtr.close();

        AvailListPhiOvFPtr.open(parPhi.PhiOvAvailListFileName, ios::in | ios::binary);
        if (AvailListPhiOvFPtr.fail()) throw parPhi.PhiOvAvailListFileName;  // the exception being checked
        
        AvailListPhiOvFPtr.read((char *)&ReadRecCount, sizeof(ReadRecCount));
        
        out<<"\nNo of Available Phi Records (from stored)(ReadRecCount):"<<ReadRecCount<<std::endl;
        if (ReadRecCount != parPhi.NoOfPhiAvailOvRec){
            AvailListPhiOvFPtr.close();
            std::cout << "Error in Phi::operator<<() - mismatch in number of records" << std::endl;
            std::exit(1);
        }        
        out << "\nList of Available Records:"<<std::endl;
        long long OvRecIndex;
        for (long long i = 0; i < parPhi.NoOfPhiAvailOvRec; i++){
            PhiFPtr.read((char*)&OvRecIndex, sizeof(OvRecIndex));
            out <<"OvRecIndex:"<< OvRecIndex << " ";
        }
        AvailListPhiOvFPtr.seekg(0,ios::end);
        out<<"File size of Available overflow record list(AvailListPhiOvFPtr.tellg()):"<<AvailListPhiOvFPtr.tellg()<<"(Observed)(Pos:";
        Pos = sizeof(long long)+parPhi.NoOfPhiAvailOvRec*R1Size;
        out<<Pos<<")"<<std::endl;
        out << "\nEnd of Phi Data"<<std::endl;
        AvailListPhiOvFPtr.close();
        return out;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::operator<<() - opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::operator<<() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::operator<<() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::operator<<() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::operator<<() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

/*
Search for the existing contact. If not found it returns false, otherwise
it returns true. Along with the area flag and the record index at which 
it is available.
*/
bool Phi::GetPhiRecIndex(long parSID, long parDID, bool &parOvFlag,long long &parPhiRecIndex){
//Hash function may be used
    if ((parSID < 0L || parDID < 0L) || (parSID >= N || parDID >= N)){
        std::cout<<"Error in Phi::GetPhiRecIndex():-Invalid user ID(s)"<<std::endl;
        std::exit(1);
    }
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

    PhiRec R;
    int RSize=R.Size();
    PhiRecOverflow R1;
    int R1Size=R1.Size();
    long long Pos=0,ReadRecCount=0,OvStart=-1L;
    long Degree=0,i,Q = ceil(f * q);
    try{
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        if (ReadRecCount != NoOfPhiRec){
            PhiFPtr.close();
            std::cout << "Error in Phi::GetPhiRecIndex():- mismatch in number of records" << std::endl;
            std::exit(1);
        }
        Pos = sizeof(long long);
        Pos += (sizeof(long)+Q*RSize+sizeof(long long))*parSID;
        PhiFPtr.seekg(Pos, ios::beg);
        PhiFPtr.read((char*)&Degree, sizeof(long));
        if (Degree == 0){
            PhiFPtr.close();
            //std::cout << "Phi::GetPhiRecIndex():-zero degree node" << std::endl;
            return false;
        }
        if (Degree <= Q){
            for(i = 0; i < Degree; i++){
                PhiFPtr.read((char*)&R, RSize);
                //std::cout<<"R="<<R<<std::endl;
                if (R.GetNeighborID() == parDID) break;    
            }
            PhiFPtr.close();
            if(i>=Degree){
                //std::cout << "Phi::GetPhiRecIndex():- no record in the regular area" << std::endl;
                return false;
            }
            parPhiRecIndex = (long long)i;
            parOvFlag = false;
            return true;
        }
        for(i = 0; i < Q; i++){
            PhiFPtr.read((char*)&R, RSize);
            //std::cout<<"R="<<R<<std::endl;
            if (R.GetNeighborID() == parDID) break;    
        }
        if (i<Q){
            parPhiRecIndex = (long long)i;
            parOvFlag = false;
            return true;
        }
   
        Pos = sizeof(long long);
        Pos += (sizeof(long)+Q*RSize+sizeof(long long))*(parSID+1);
        Pos -= sizeof(long long);
        PhiFPtr.read((char*)&OvStart, sizeof(OvStart));
        //std::cout<<"OvStart="<<OvStart<<"("<<parSID<<","<< parDID << ")"<<std::endl;
        if (OvStart < 0L){
            PhiFPtr.close();
            //std::cout << "Phi::GetPhiRecIndex():- no record in the regular area" << std::endl;
            return false;                
        }
        parPhiRecIndex = OvStart;
        parOvFlag = true;

        Pos = sizeof(long long);
        //Pos += (sizeof(long)+Q*RSize+sizeof(long long))* NoOfPhiRec;
        Pos += (sizeof(long)+Q*RSize+sizeof(long long))* N;
        //Pos += (sizeof(long)+sizeof(long long))*N+(RSize*NoOfPhiRec);
        //std::cout<<"Pos="<<Pos<<std::endl;

        while(true){
            PhiFPtr.seekg(Pos + parPhiRecIndex * R1Size, ios::beg);
            PhiFPtr.read((char *)&R1, R1Size);
            //std::cout<<"R1="<<R1<<std::endl;
            if (R1.GetNeighborID() == parDID && R1.GetHostID() == parSID){
                PhiFPtr.close();
                return true;
            }
            parPhiRecIndex = R1.GetNext();
            if (parPhiRecIndex < 0L){
                PhiFPtr.close();
                //std::cout << "Phi::GetPhiRecIndex():- no record in the overflow area" << std::endl;
                return false;                
            }
        }
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::GetPhiRecIndex() - opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::GetPhiRecIndex() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::GetPhiRecIndex() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::GetPhiRecIndex() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::GetPhiRecIndex() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }    
}

bool Phi::GetCVID(long parSID,bool parOvFlag,long long parPhiRecIndex,long long& parCVID){
    if ((parSID<0L||parSID>=N)){
        std::cout<<"Error in Phi::GetCVID():-Invalid user ID(s)"<<std::endl;
        return false;
    }
    if ((!parOvFlag)&&(parPhiRecIndex<0L||parPhiRecIndex>=NoOfPhiRec)){
        std::cout<<"\nPhi::GetCVID():-Invalid record index("<<parPhiRecIndex<<")"<<std::endl;
        return false;
    }
    if (parOvFlag&&(parPhiRecIndex<0L||parPhiRecIndex>=NoOfPhiOvRec)){
        std::cout<<"\nPhi::GetCVID():-Invalid record index("<<parPhiRecIndex<<")"<<std::endl;
        return false;
    }
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    PhiRec R;
    PhiRecOverflow R1;
    int RSize = R.Size(), R1Size = R1.Size();
    long long ReadRecCount,Pos;
    long Degree, Q = ceil(f * q);
    try {
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        if (ReadRecCount != NoOfPhiRec){
            PhiFPtr.close();
            std::cout << "Error in Phi::GetCVID() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }
        if(!parOvFlag){
            Pos = sizeof(long long);
            Pos += (sizeof(long)+Q*RSize+sizeof(long long))*parSID;
            PhiFPtr.seekg(Pos,ios::beg);
            PhiFPtr.read((char *)&Degree, sizeof(Degree));
            if(parPhiRecIndex >= Degree){
                PhiFPtr.close();
                std::cout<<"\nPhi::GetCVID():-Invalid record index("<<parPhiRecIndex<<")"<<std::endl;
                return false;
            }
            PhiFPtr.seekg(Pos+sizeof(Degree)+parPhiRecIndex*RSize,ios::beg);
            PhiFPtr.read((char *)&R,RSize);
            if(R.GetCVID()<0L || R.GetDelFlag() || R.GetUpdateFlag()){
                PhiFPtr.close();
                std::cout << "Error in Phi::GetCVID()-inconsistent stored data" << std::endl;
                std::exit(1);
            }
            parCVID = R.GetCVID();
            PhiFPtr.close();
            return true;
        }
        Pos = sizeof(long long);
        //Pos += (sizeof(long)+Q*RSize+sizeof(long long))*NoOfPhiRec;
        Pos += (sizeof(long)+Q*RSize+sizeof(long long))*N;
        //Pos += (sizeof(long)+sizeof(long long))*N+(RSize*NoOfPhiRec);
        //std::cout<<"(Pos,PhiRecIndex)=("<<Pos<<","<<parPhiRecIndex<<")"<<std::endl;

        PhiFPtr.seekg(Pos+parPhiRecIndex*R1Size,ios::beg);
        PhiFPtr.read((char *)&R1,R1Size);
        if(R1.GetHostID()!=parSID||R1.GetCVID()<0L||R1.GetDelFlag()||R1.GetUpdateFlag()){
            PhiFPtr.close();
            std::cout << "Error in Phi::GetCVID()-inconsistent stored data" << std::endl;
            std::exit(1);
        }
        parCVID = R1.GetCVID();
        PhiFPtr.close();
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::GetCVID()- Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::GetCVID() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::GetCVID() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::GetCVID() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::GetCVID() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
    return true;
}

/*
This function checks in Phi that the user with userID in parDID (destination) parameter
is in contact with the user with userID in parSID (source) parameter. If found then
it returns corresponding index of the Contact Vector.
*/
bool Phi::GetCVID(long parSID, long parDID, long long& parCVID, bool& parOvFlag, long long& parPhiRecIndex){
    if ((parSID < 0L || parDID < 0L) || (parSID >= N || parDID >= N)){
        std::cout<<"Error in Phi::GetCVID():-Invalid user ID(s)"<<std::endl;
        return false;
    }
    parCVID = -1L;
    bool OvFlag=false;
    long long PhiRecIndex=-1L;
    if (!GetPhiRecIndex(parSID,parDID,OvFlag,PhiRecIndex)){
        //std::cout << "Phi::GetCVID():-No Contact Exists" << std::endl;
        return false;
    }

    parOvFlag = OvFlag;
    parPhiRecIndex = PhiRecIndex;
    
    long long ReadRecCount=0,Pos=0;
    long Q = ceil(f * q);
    PhiRec R;
    int RSize = R.Size();
    PhiRecOverflow R1;
    int R1Size=R1.Size();
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

    try{
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        if (ReadRecCount != NoOfPhiRec){
            PhiFPtr.close();
            std::cout << "Error in Phi::GetCVID():- mismatch in number of records" << std::endl;
            std::exit(1);
        }
        if (OvFlag == false){
            Pos = sizeof(long long);
            Pos += (sizeof(long)+Q*RSize+sizeof(long long))*parSID;
            PhiFPtr.seekg(Pos+sizeof(long)+PhiRecIndex * RSize, ios::beg);
            PhiFPtr.read((char *)&R, RSize);
            //std::cout<<"R(in GetCVID())="<<R<<std::endl;
            parCVID = R.GetCVID();
        }
        else {
            Pos = sizeof(long long);
            //Pos += (sizeof(long)+Q*RSize+sizeof(long long))*NoOfPhiRec;
            Pos += (sizeof(long)+Q*RSize+sizeof(long long))*N;
            //Pos += (sizeof(long)+sizeof(long long))*N+(RSize*NoOfPhiRec);
            //std::cout<<"(Pos,PhiRecIndex)=("<<Pos<<","<<PhiRecIndex<<")"<<std::endl;

            PhiFPtr.seekg(Pos + PhiRecIndex * R1Size, ios::beg);
            PhiFPtr.read((char *)&R1, R1Size);
            //std::cout<<"R1(in GetCVID())="<<R1<<std::endl;
            parCVID = R1.GetCVID();
        }
        PhiFPtr.close();
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::GetPhiRecIndex() - opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::GetPhiRecIndex() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::GetPhiRecIndex() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::GetPhiRecIndex() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::GetPhiRecIndex() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }    
}


void Phi::GetContactList(long parSID, vector<CONTACT>& parCL){
    /*
    This function returns the whole list of all contacts of an user with userID in parSID passed
    as input parameter of the function. Contact list is a sequential list of contact record. Each
    contact record has two fields - contact userID and the index of the corresponding Contact Vector.
    */
    if (parSID <0L || parSID >= N){
        std::cout<<"Error in Phi::GetContactList()- Invalid user ID"<<std::endl;
        std::exit(1);
    }
    vector<CONTACT> L;
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    PhiRec R;
      long long Pos,NoOfPhiRecRead;
    try{
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&NoOfPhiRecRead, sizeof(NoOfPhiRecRead));
        if (NoOfPhiRecRead != NoOfPhiRec){
            PhiFPtr.close();
            std::cout << "Error in Phi::GetContactList() - miss match in number of records" << std::endl;
            std::exit(1);
        }

        Pos = (long long)(parSID * q * 2 * R.Size());
        PhiFPtr.seekg(Pos, ios::beg);
        for (  int i = 0; i < ((2 * q) -1); i++){
            PhiFPtr.read((char*)&R, R.Size());
            if (R.GetDelFlag() == true){
                PhiFPtr.close();
                return;
            }
            CONTACT T(R.GetNeighborID(), R.GetCVID());
            L.push_back(T);
        }
        PhiFPtr.close();
        return;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::GetContactList(parSID,parCL) - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::GetContactList(parSID,parCL) " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::GetContactList(parSID,parCL) " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::GetContactList(parSID,parCL) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::GetContactList(parSID,parCL) " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

/*
This is equivalent to GetNode() function in case of linked list.
This function is used to supply the index of one available Phi Overflow Record.
If there is any free record which is in the available list it is supplied.
If multiple such records are available then last one of the list is supplied.
Otherwise, one new overflow record is added at the end of the Phi records, and
index of this record is supplied. This will increase the count of Phi Overflow 
records by one in case of adding the new record, otherwise it decrease the count
of nubmer of available Phi Overflow Record. All these must be noted in the Phi 
object structure both in memory and file store.  
*/
bool Phi::GetNode(long long &parPhiOvRecIndex){
    std::fstream PhiHeaderFPtr;
    PhiHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream AvailListPhiOvFPtr;
    AvailListPhiOvFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

    long Q = ceil(f * q);
    PhiRec R;
    int RSize = R.Size();
    PhiRecOverflow R1;
    int R1Size = R1.Size();
    R1.SetCVID(0L);
    R1.SetDelFlag(true);
    R1.SetHostID(-1L);
    R1.SetNeighborID(-1L);
    R1.SetNext(-1L);
    R1.SetPrevious(-1L);
    R1.SetUpdateFlag(false);
    long long ReadRecCount,Pos;
    try
    {
        PhiHeaderFPtr.open(PhiFileHeaderName, ios::in | ios::binary);
        if (PhiHeaderFPtr.fail()) throw PhiFileHeaderName;  // the exception being checked
        PhiHeaderFPtr.close();

        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        PhiFPtr.close();
        if (ReadRecCount != NoOfPhiRec){
            std::cout << "Error in Phi::GetNode() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }
        AvailListPhiOvFPtr.open(PhiOvAvailListFileName, ios::in | ios::binary);
        if (AvailListPhiOvFPtr.fail()) throw PhiOvAvailListFileName;  // the exception being checked

        AvailListPhiOvFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        AvailListPhiOvFPtr.close();
        if (NoOfPhiAvailOvRec != ReadRecCount){
            std::cout << "Error in Phi::GetNode() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }        
        /*One or more Phi Overflow records have been added and one or more such records
        are free to use.
        */
       //Add an new Phi Overflow Record
        if(NoOfPhiAvailOvRec == 0){
            PhiFPtr.open(PhiFileName, ios::in | ios::out | ios::binary);
            if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

            Pos = sizeof(long long);
            //Pos += (sizeof(long)+Q*RSize+sizeof(long long))*NoOfPhiRec;
            Pos += (sizeof(long)+Q*RSize+sizeof(long long))*N;
            //Pos += (sizeof(long)+sizeof(long long))*N+(RSize*NoOfPhiRec);
            //std::cout<<"(Pos,NoOfPhiOvRec)=("<<Pos<<","<<NoOfPhiOvRec<<")"<<std::endl;

            Pos += NoOfPhiOvRec*R1Size;
            PhiFPtr.seekg(Pos, ios::beg);
            PhiFPtr.write((char *)&R1, R1.Size());
            if (PhiFPtr.fail()){
                PhiFPtr.close();
                AvailListPhiOvFPtr.close();    
                std::cout << "Error in Phi::GetNode() " << " no space for the new overflow record" << std::endl;
                return false;
            }
            //Earlier Phi Overflow Record count is the index of this new record
            //as this record index starts from zero.
            parPhiOvRecIndex = NoOfPhiOvRec;
            NoOfPhiOvRec++;
            PhiFPtr.close();
        }
        else{
            AvailListPhiOvFPtr.open(PhiOvAvailListFileName, ios::in | ios::out | ios::binary);
            if (AvailListPhiOvFPtr.fail()) throw PhiOvAvailListFileName;  // the exception being checked

            //Index of records in the overflow area are the records for Avail ... file. 
            NoOfPhiAvailOvRec--;
            Pos = sizeof(long long)+NoOfPhiAvailOvRec*sizeof(long long);
            AvailListPhiOvFPtr.seekg(Pos, ios::beg);
            AvailListPhiOvFPtr.read((char *)&parPhiOvRecIndex, sizeof(parPhiOvRecIndex));
            /*Note there we are not deleting the entries from this file, for this reason
            appending at the end is not possible.
            How to discard the unused part from the file by updating file size in the 
            File Allocation Table is to know so that proper deletion of unused part
            can be controlled. */
            AvailListPhiOvFPtr.seekg(0, ios::beg);
            AvailListPhiOvFPtr.write((char*)&NoOfPhiAvailOvRec, sizeof(NoOfPhiAvailOvRec));
            AvailListPhiOvFPtr.close();
        }
        //Store all these changes in the Phi Object Storage Structure
        PhiHeaderFPtr.open(PhiFileHeaderName, ios::in | ios::out | ios::binary);
        if (PhiHeaderFPtr.fail()) throw PhiFileHeaderName;  // the exception being checked

        PhiHeaderFPtr.write((char*)this, sizeof(Phi));
        PhiHeaderFPtr.close();
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::GetNode()  - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::GetNode() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::GetNode() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::GetNode() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::GetNode() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Phi::FreeNode(long long parPhiOvRecIndex){
    if (parPhiOvRecIndex < 0L || parPhiOvRecIndex >= NoOfPhiOvRec){
        std::cout<<"Error in Phi::FreeNode()- invalid number Phi Overflow record index"<<std::endl;
        return false;
    }
    std::fstream PhiHeaderFPtr;
    PhiHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream AvailListPhiOvFPtr;
    AvailListPhiOvFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

    long long ReadRecCount,Pos;
    long Q = ceil(f * q);
    PhiRec R;
    int RSize = R.Size();
    PhiRecOverflow R1;
    int R1Size = R1.Size();
    R1.SetCVID(0L);
    R1.SetDelFlag(true);
    R1.SetHostID(-1L);
    R1.SetNeighborID(-1L);
    R1.SetNext(-1L);
    R1.SetPrevious(-1L);
    R1.SetUpdateFlag(false);
    try
    {
        PhiHeaderFPtr.open(PhiFileHeaderName, ios::in | ios::binary);
        if (PhiHeaderFPtr.fail()) throw PhiFileHeaderName;  // the exception being checked

        PhiHeaderFPtr.close();

        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        PhiFPtr.close();
        if (ReadRecCount != NoOfPhiRec){
            std::cout << "Error in Phi::FreeNode() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }

        AvailListPhiOvFPtr.open(PhiOvAvailListFileName, ios::in | ios::binary);
        if (AvailListPhiOvFPtr.fail()) throw PhiOvAvailListFileName;  // the exception being checked

        AvailListPhiOvFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        AvailListPhiOvFPtr.close();
        if (NoOfPhiAvailOvRec != ReadRecCount){
            std::cout << "Error in Phi::FreeNode() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }
        AvailListPhiOvFPtr.open(PhiOvAvailListFileName, ios::in | ios::out | ios::binary);
        if (AvailListPhiOvFPtr.fail()) throw PhiOvAvailListFileName;  // the exception being checked

        /*
        At present deleted records from this Available List is not removed so appending
        of free record is not possible. But, it will be added as last record at the position
        pointed by the No of Available Overflow Record Count.
        */
        Pos = sizeof(long long)+ NoOfPhiAvailOvRec * sizeof(long long);
        AvailListPhiOvFPtr.seekg(Pos, ios::beg);
        AvailListPhiOvFPtr.write((char *)&parPhiOvRecIndex, sizeof(parPhiOvRecIndex));
        if (AvailListPhiOvFPtr.fail()){
            AvailListPhiOvFPtr.close();
            std::cout <<"\nError in Phi::FreeNode() :- No space to store the index of the free overflow record"<<std::endl;
            return false;
        }
        NoOfPhiAvailOvRec++;
        AvailListPhiOvFPtr.seekg(0, ios::beg);
        AvailListPhiOvFPtr.write((char*)&NoOfPhiAvailOvRec, sizeof(NoOfPhiAvailOvRec));
        AvailListPhiOvFPtr.close();

        PhiFPtr.open(PhiFileName, ios::in | ios::out | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        Pos = sizeof(long long);
        //Pos += (sizeof(long)+Q*RSize+sizeof(long long))*NoOfPhiRec;
        Pos += (sizeof(long)+Q*RSize+sizeof(long long))*N;

        PhiFPtr.seekg(Pos+parPhiOvRecIndex*R1Size, ios::beg);
        PhiFPtr.write((char *)&R1,R1Size);
        if (PhiFPtr.fail()){
            PhiFPtr.close();
            std::cout <<"\nError in Phi::FreeNode() :- cannot write the record"<<std::endl;
            return false;
        }
        PhiFPtr.close();
        //Store all these changes in the Phi Object Storage Structure
        PhiHeaderFPtr.open(PhiFileHeaderName, ios::in | ios::out | ios::binary);
        if (PhiHeaderFPtr.fail()) throw PhiFileHeaderName;  // the exception being checked

        PhiHeaderFPtr.write((char*)this, sizeof(Phi));
        PhiHeaderFPtr.close();
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::FreeNode()- Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::FreeNode() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::FreeNode() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::FreeNode() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::FreeNode() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Phi::Insert(long parSID,long parDID,long long parCVID,bool& parOvFlag,long long& parPhiRecIndex){
    if ((parSID <0L || parDID <0L) || (parSID >= N || parDID >= N)){
        std::cout<<"Error in Phi::Insert()- Invalid user ID(s)"<<std::endl;
        return false;
    }
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    PhiRec R;
    int RSize = R.Size();
    PhiRecOverflow R1;
    int R1Size = R1.Size();
    long Q = ceil(f * q);
    long long Pos,ReadRecCount;
    long long OvStart,NewPhiRecIndex;
    long Degree;
    try
    {
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked
        PhiFPtr.close();

        PhiFPtr.open(PhiFileName, ios::in | ios::out | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        //std::cout<<"Phi::Insert()-No of Phi Rec Read="<<ReadRecCount<<std::endl;

        if (ReadRecCount != NoOfPhiRec){
            PhiFPtr.close();
            std::cout << "Error in Phi::Insert():- mismatch in number of records" << std::endl;
            std::exit(1);
        }
        Pos = sizeof(long long);
        Pos += (sizeof(long)+Q*RSize+sizeof(long long))*parSID;
        PhiFPtr.seekg(Pos, ios::beg);
        PhiFPtr.read((char*)&Degree, sizeof(long));
        if (Degree < Q)
        {
            R.SetCVID(parCVID);
            R.SetNeighborID(parDID);
            R.SetDelFlag(false);
            R.SetUpdateFlag(false);

            PhiFPtr.seekg(Pos+sizeof(long)+Degree*RSize, ios::beg);
            PhiFPtr.write((char*)&R, RSize);
            parPhiRecIndex = Degree;
            parOvFlag = false;
            Degree++;
            PhiFPtr.seekg(Pos, ios::beg);
            PhiFPtr.write((char*)&Degree, sizeof(long));
            PhiFPtr.close();
            return true;
        }
        //Get a node i.e. index of the free overflow record
        //If not available then create it. Use the insert at the starting position technique.
        //Here this last node working as starting pointer.
        //Here two-ways linked representation is used.
        //GetNode() increase the number of overflow record count
        //As the node is used for insertion so number of available overflow node remain unchanged
        if (!GetNode(NewPhiRecIndex)){
            PhiFPtr.close();
            //std::cout << "Error in Phi::Insert()-Free Phi Overflow Record is not available"<<std::endl;
            return false;
        }
        parOvFlag = true;
        parPhiRecIndex = NewPhiRecIndex;
        //For Overflow Area
        Degree++;
        PhiFPtr.seekg(Pos, ios::beg);
        PhiFPtr.write((char*)&Degree, sizeof(long));

        //Read the Start Pointer of the link list
        Pos = sizeof(long long);
        Pos += (sizeof(long)+Q*RSize+sizeof(long long))*(parSID+1);
        Pos -=sizeof(long long);
        PhiFPtr.seekg(Pos, ios::beg);
        PhiFPtr.read((char*)&OvStart, sizeof(long long));
        PhiFPtr.seekg(Pos, ios::beg);
        PhiFPtr.write((char*)&NewPhiRecIndex, sizeof(long long));

        //Set base for overflow area
        Pos = sizeof(long long);
        //Pos += (sizeof(long)+Q*RSize+sizeof(long long))*NoOfPhiRec;
        Pos += (sizeof(long)+Q*RSize+sizeof(long long))*N;
        //Free Phi Overflow Record is available, now read it for all updates        
        //PhiFPtr.seekg(Pos+NewPhiRecIndex*R1Size, ios::beg);
        //PhiFPtr.read((char*)&R1, R1Size);
        R1.SetPrevious(-1L);//BLink of starting node is NULL 
        R1.SetCVID(parCVID);
        R1.SetNeighborID(parDID);
        R1.SetHostID(parSID);
        R1.SetDelFlag(false);
        R1.SetUpdateFlag(false);
        R1.SetNext(OvStart);
        //Store the inserted node
        PhiFPtr.seekg(Pos+NewPhiRecIndex*R1Size, ios::beg);
        PhiFPtr.write((char*)&R1, R1Size);
        //update is required for the previous start node
        if (OvStart >= 0L){
            PhiFPtr.seekg(Pos+OvStart*R1Size, ios::beg);
            PhiFPtr.read((char*)&R1, R1Size);
            R1.SetPrevious(NewPhiRecIndex);
            PhiFPtr.seekg(Pos+OvStart*R1Size, ios::beg);
            PhiFPtr.write((char*)&R1, R1Size);
        }
        PhiFPtr.close();
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::Insert()  - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::Insert() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::Insert() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::Insert() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::Insert() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Phi::Delete(long parSID, bool parOvFlag, long long parPhiRecIndex){
    /*
    By this delete function last record in the non-overflow area is placed at the deleted
    space, so the record shifting is not required. And only the last record is changing its
    current index location.
    */
    if (parSID < 0L || parSID >= N){
        std::cout<<"Error in Phi::Delete()- Invalid user ID(s)"<<std::endl;
        return false;
    }
    if ((!parOvFlag) && (parPhiRecIndex < 0L || parPhiRecIndex >= NoOfPhiRec)){
        std::cout<<"\nPhi::Delete():-Invalid record index("<<parPhiRecIndex<<")"<<std::endl;
        return false;
    }
    if (parOvFlag && (parPhiRecIndex < 0L || parPhiRecIndex >= NoOfPhiOvRec)){
        std::cout<<"\nPhi::Delete():-Invalid record index("<<parPhiRecIndex<<")"<<std::endl;
        return false;
    }
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    PhiRec R;
    int RSize = R.Size();
    long long Pos;
    long Degree,Q = ceil(f * q);
    PhiRecOverflow R1;
    int R1Size = R1.Size();
    long long ReadRecCount;
    try{
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        PhiFPtr.close();

        if (ReadRecCount != NoOfPhiRec){
            std::cout << "Error in Phi::Delete() - mismatch in number of records" << std::endl;
            std::exit(1);
        }

        PhiFPtr.open(PhiFileName, ios::in | ios::out | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        Pos = sizeof(long long);
        Pos += (sizeof(long)+Q*RSize+sizeof(long long))*parSID;

        PhiFPtr.seekg(Pos, ios::beg);
        PhiFPtr.read((char*)&Degree, sizeof(Degree));

        if(Degree <= 0L){
            PhiFPtr.close();
            std::cout << "Error in Phi::Delete()- no record to delete" << std::endl;
            return false;
        }

        if (parOvFlag == false){
            if (parPhiRecIndex >= Degree){
                PhiFPtr.close();
                std::cout << "Error in Phi::Delete()- no record to delete" << std::endl;
                return false;
            }
            Degree--;
            PhiFPtr.seekg(Pos, ios::beg);
            PhiFPtr.write((char*)&Degree, sizeof(Degree));
            if (Degree==0L){
                R.SetCVID(-1L);
                R.SetDelFlag(true);
                R.SetNeighborID(-1L);
                R.SetUpdateFlag(false);
                PhiFPtr.seekg(Pos+sizeof(Degree), ios::beg);
                PhiFPtr.write((char*)&R, RSize);
                PhiFPtr.close();
                std::cout << "Phi::Delete()- successful deletion" << std::endl;
                return true; 
            }
            PhiFPtr.seekg(Pos+sizeof(long)+Degree*RSize, ios::beg);
            PhiFPtr.read((char*)&R, RSize);
            PhiFPtr.seekg(Pos+sizeof(long)+parPhiRecIndex*RSize, ios::beg);
            PhiFPtr.write((char*)&R, RSize);
            R.SetCVID(-1L);
            R.SetDelFlag(true);
            R.SetNeighborID(-1L);
            R.SetUpdateFlag(false);
            PhiFPtr.seekg(Pos+sizeof(long)+Degree*RSize, ios::beg);
            PhiFPtr.write((char*)&R, RSize);
            PhiFPtr.close();
            std::cout << "Phi::Delete()- successful deletion" << std::endl;
            return true;
        }
        Degree--;
        PhiFPtr.seekg(Pos, ios::beg);
        PhiFPtr.write((char*)&Degree, sizeof(Degree));
        
        Pos = sizeof(long long);
        //Pos += (sizeof(long)+Q*RSize+sizeof(long long))*NoOfPhiRec;
        Pos += (sizeof(long)+Q*RSize+sizeof(long long))*N;

        PhiFPtr.seekg(Pos+parPhiRecIndex*R1Size, ios::beg);
        PhiFPtr.read((char*)&R1, R1Size);

        long long PhiRecIndexPred, PhiRecIndexSucc;
        PhiRecIndexPred = R1.GetPrevious();
        PhiRecIndexSucc = R1.GetNext();

        //Deleted record in the overflow area is now freed so that it can be reused.
        //We use here the Free function for this deletion.

        if (PhiRecIndexSucc>=0L){
            PhiFPtr.seekg(Pos+PhiRecIndexSucc*R1Size, ios::beg);
            PhiFPtr.read((char*)&R1, R1Size);
            R1.SetPrevious(PhiRecIndexPred);
            PhiFPtr.seekg(Pos+PhiRecIndexSucc*R1Size, ios::beg);
            PhiFPtr.write((char*)&R1, R1Size);
        }
        if(PhiRecIndexPred>=0L){
            PhiFPtr.seekg(Pos+PhiRecIndexPred*R1Size, ios::beg);
            PhiFPtr.read((char*)&R1, R1Size);
            R1.SetNext(PhiRecIndexSucc);
            PhiFPtr.seekg(Pos+PhiRecIndexPred*R1Size, ios::beg);
            PhiFPtr.write((char*)&R1, R1Size);
        }
        if(PhiRecIndexPred<0L){
            Pos -= sizeof(long long);
            PhiFPtr.seekg(Pos, ios::beg);
            PhiFPtr.write((char*)&PhiRecIndexSucc, sizeof(PhiRecIndexSucc));
        }
        PhiFPtr.close(); 
        if(!FreeNode(parPhiRecIndex)){
            std::cout << "Error in Phi::Delete():- not possible to make the deleted node as free" << std::endl;
            std::exit(1);
        }
        std::cout << "Phi::Delete()- successful deletion" << std::endl;
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::Delete()  - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::Delete() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::Delete() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::Delete() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::Delete() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }

}

bool Phi::Delete(long parSID, long parDID){
    if ((parSID < 0L || parDID < 0L) || (parSID >= N || parDID >= N)){
        std::cout<<"Error in Phi::Delete()- Invalid user ID(s)"<<std::endl;
        return false;
    }
    long long PhiRecIndex;
    bool OvFlag;
    if(!GetPhiRecIndex(parSID,parDID,OvFlag,PhiRecIndex)){
        std::cout << "Error in Phi::Delete()- no record to delete" << std::endl;
        return false;
    }
    return Delete(parSID,OvFlag,PhiRecIndex);
}

inline   int Phi::Size(){
    return sizeof(Phi);
}

void Phi::StorageSize(long long &parCalSize, long long &parObsrvSize){
    long long PhiFlCalSize = 0, PhiFlObsrvSize = 0;
    long long PhiHeaderFlCalSize = 0, PhiHeaderFlObsrvSize = 0;
    long long PhiAvlListFlCalSize = 0, PhiAvlListFlObsrvSize = 0;
    long long ReadRecCount;
    PhiRec R;
    PhiRecOverflow R1;
    PhiFlCalSize = sizeof(long long)+NoOfPhiRec*R.Size()+N*(sizeof(long)+sizeof(long long));
    PhiFlCalSize += NoOfPhiOvRec*R1.Size();

    PhiAvlListFlCalSize = (NoOfPhiAvailOvRec+1)*sizeof(long long);

    PhiHeaderFlCalSize = sizeof(Phi);

    parCalSize = PhiFlCalSize + PhiAvlListFlCalSize + PhiHeaderFlCalSize;
    //std::cout<<"Storage requirements for storing Phi Structure="<<PhiFlSize<<std::endl;
    std::fstream PhiHeaderFPtr;
    PhiHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream AvailListPhiOvFPtr;
    AvailListPhiOvFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    try {
        PhiHeaderFPtr.open(PhiFileHeaderName, ios::in | ios::binary);
        if (PhiHeaderFPtr.fail()) throw PhiFileHeaderName;  // the exception being checked
        
        PhiHeaderFPtr.seekg(0, ios::end);
        PhiHeaderFlObsrvSize = PhiHeaderFPtr.tellg();
        PhiHeaderFPtr.close();

        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked
        PhiFPtr.seekg(0, ios::end);
        PhiFlObsrvSize = PhiFPtr.tellg();
        PhiFPtr.close();
        
        AvailListPhiOvFPtr.open(PhiOvAvailListFileName,ios::in | ios::binary);
        if (AvailListPhiOvFPtr.fail()) throw PhiOvAvailListFileName;  // the exception being checked
        AvailListPhiOvFPtr.seekg(0, ios::beg);
        AvailListPhiOvFPtr.read((char *)&ReadRecCount, sizeof(ReadRecCount));
        AvailListPhiOvFPtr.close();
        
        PhiAvlListFlObsrvSize = (ReadRecCount+1)*sizeof(long long);
        parObsrvSize = PhiHeaderFlObsrvSize+PhiFlObsrvSize+PhiAvlListFlObsrvSize;
        std::cout<<"PhiHeaderSize(Cal,Obsrv)=("<<PhiHeaderFlCalSize<<","<<PhiHeaderFlObsrvSize<<")"<<std::endl;
        std::cout<<"PhiSize(Cal,Obsrv)=("<<PhiFlCalSize<<","<<PhiFlObsrvSize<<")"<<std::endl;
        std::cout<<"PhiAvlListSize(Cal,Obsrv)=("<<PhiAvlListFlCalSize<<","<<PhiAvlListFlObsrvSize<<")"<<std::endl;
        return;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::StorageSize() - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::StorageSize() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::StorageSize() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::StorageSize() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::StorageSize() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Phi::GetPhiRec(const long long parRecNo, PhiRec& R){
    if (parRecNo<0L||parRecNo>(NoOfPhiRec+NoOfPhiOvRec)){
            std::cout << "Error in Phi::GetPhiRec(): invalid record number" << std::endl;
            return false;
    }
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    int RSize = R.Size();
    long long Pos;
    long UID,PhiRecIndex,Q = ceil(f * q);
    PhiRecOverflow R1;
    int R1Size = R1.Size();
    long long RecCount=0;
    try {
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&RecCount, sizeof(RecCount));
        if (RecCount != NoOfPhiRec){
            PhiFPtr.close();
            std::cout << "Error in Phi::GetPhiRec(): mismatch in record counts" << std::endl;
            exit(1);
        }
        if (parRecNo < NoOfPhiRec){
            UID = trunc(parRecNo / Q);
            PhiRecIndex = (parRecNo % Q);
            Pos = sizeof(long long)+(sizeof(long)+Q*RSize+sizeof(long long))*UID;
            PhiFPtr.seekg(Pos+sizeof(long)+PhiRecIndex*RSize, ios::beg);
            PhiFPtr.read((char*)&R, RSize);
        }
        else {
            PhiRecIndex = parRecNo - NoOfPhiRec;
            //Pos = sizeof(long long)+(sizeof(long)+Q*RSize+sizeof(long long))*NoOfPhiRec;
            Pos = sizeof(long long)+(sizeof(long)+Q*RSize+sizeof(long long))*N;
            PhiFPtr.seekg(Pos+PhiRecIndex*R1Size, ios::beg);
            PhiFPtr.read((char*)&R1, R1Size);
            R.SetCVID(R1.GetCVID());
            R.SetNeighborID(R1.GetNeighborID());
            R.SetDelFlag(R1.GetDelFlag());
            R.SetUpdateFlag(R1.GetUpdateFlag());
        }
        PhiFPtr.close();
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::GetPhiRec()-Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::GetPhiRec() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::GetPhiRec() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::GetPhiRec() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::GetPhiRec() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Phi::SetPhiRec(const long long parRecNo, PhiRec& R){
    /*
    By this setting contact data may be removed as there is no close contacts. 
    For this subsequenct contact record for the current user should be shifted.
    For the overflow records proper case must be taken for this.
    */
    if (parRecNo<0L||parRecNo>(NoOfPhiRec+NoOfPhiOvRec)){
            std::cout << "Error in Phi::SetPhiRec(): invalid record number" << std::endl;
            return false;
    }
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    int RSize = R.Size();
    long long Pos;
    long UID,PhiRecIndex,Q = ceil(f * q);
    PhiRecOverflow R1;
    int R1Size = R1.Size();
    long long RecCount=0;
    try {
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&RecCount, sizeof(RecCount));
        if (RecCount != NoOfPhiRec){
            PhiFPtr.close();
            std::cout << "Error in Phi::SetPhiRec(): mismatch in record counts" << std::endl;
            exit(1);
        }
        
        PhiFPtr.open(PhiFileName, ios::in | ios:: out | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        if (parRecNo < NoOfPhiRec){
            UID = trunc(parRecNo / Q);
            PhiRecIndex = (parRecNo % Q);
            Pos = sizeof(long long)+(sizeof(long)+Q*RSize+sizeof(long long))*UID;
            PhiFPtr.seekg(Pos+sizeof(long)+PhiRecIndex*RSize, ios::beg);
            PhiFPtr.write((char*)&R, RSize);
        }
        else {
            PhiRecIndex = parRecNo - NoOfPhiRec;
            //Pos = sizeof(long long)+(sizeof(long)+Q*RSize+sizeof(long long))*NoOfPhiRec;
            Pos = sizeof(long long)+(sizeof(long)+Q*RSize+sizeof(long long))*N;
            PhiFPtr.seekg(Pos+PhiRecIndex*R1Size, ios::beg);
            PhiFPtr.read((char *)&R1, R1Size);
            R1.SetCVID(R.GetCVID());
            R1.SetNeighborID(R.GetNeighborID());
            R1.SetDelFlag(R.GetDelFlag());
            R1.SetUpdateFlag(R.GetUpdateFlag());
            PhiFPtr.seekg(Pos+PhiRecIndex*R1Size, ios::beg);
            PhiFPtr.write((char*)&R1, R1Size);
        }
        PhiFPtr.close();
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::SetPhiRec()-Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::SetPhiRec() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::SetPhiRec() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::SetPhiRec() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::SetPhiRec() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

Phi& Phi::operator=(const Phi& parPHI){
    strcpy(PhiFileName, parPHI.PhiFileName);
    strcpy(PhiFileHeaderName, parPHI.PhiFileHeaderName);
    N = parPHI.N; //Size of the population and hence the records
    q = parPHI.q; //Average number adjacent nodes
    NoOfPhiRec = parPHI.NoOfPhiRec; // 8 bytes
    return *this;
}

long long Phi::GetPhiRecCount(){
    long long RecCount, Pos;
    long Degree,Q = ceil(f * q);
    PhiRec R;
    int RSize = R.Size();

    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    try{
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        Pos = sizeof(long long);
        for(long id=0;id<N;id++){
            PhiFPtr.seekg(Pos+(sizeof(long)+Q*RSize+sizeof(long long))*id,ios::beg);
            PhiFPtr.read((char *)&Degree, sizeof(Degree));
            RecCount += Degree;
        }
        PhiFPtr.close();
        return RecCount;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::GetPhiRecCount()  - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::GetPhiRecCount()  " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::GetPhiRecCount()  " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::GetPhiRecCount() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::GetPhiRecCount() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Phi::GetUpdateFlag(long parSID,bool parOvFlag,long long parPhiRecIndex){
    if ((parSID<0L||parSID>=N)){
        std::cout<<"Error in Phi::GetUpdateFlag()-Invalid user ID(s)"<<std::endl;
        return false;
    }
    if ((!parOvFlag)&&(parPhiRecIndex<0L||parPhiRecIndex>=NoOfPhiRec)){
        std::cout<<"\nPhi::GetUpdateFlag():-Invalid record index"<<std::endl;
        return false;
    }
    if (parOvFlag&&(parPhiRecIndex<0L||parPhiRecIndex>=NoOfPhiOvRec)){
        std::cout<<"\nPhi::GetUpdateFlag():-Invalid record index"<<std::endl;
        return false;
    }
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    PhiRec R;
    PhiRecOverflow R1;
    int RSize = R.Size(), R1Size = R1.Size();
    long long ReadRecCount,Pos;
    long Degree, Q = ceil(f * q);
    try {
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        if (ReadRecCount != NoOfPhiRec){
            PhiFPtr.close();
            std::cout << "Error in Phi::GetUpdateFlag()- mismatch in the record count" << std::endl;
            std::exit(1);
        }

        /*  */
        if(!parOvFlag){
            Pos = sizeof(long long);
            Pos += (sizeof(long)+Q*RSize+sizeof(long long))*parSID;
            PhiFPtr.seekg(Pos,ios::beg);
            PhiFPtr.read((char *)&Degree, sizeof(Degree));
            //std::cout<<"OvFlag="<<parOvFlag;
            //std::cout<<" SID="<<parSID<<" Degree="<<Degree<<" PhiRecIndex="<<parPhiRecIndex<<std::endl;
            if(parPhiRecIndex >= Degree){
                PhiFPtr.close();
                std::cout<<"\nPhi::GetUpdateFlag():-Invalid record index("<<parPhiRecIndex<<")"<<std::endl;
                return false;
            }
            PhiFPtr.seekg(Pos+sizeof(Degree)+parPhiRecIndex*RSize,ios::beg);
            PhiFPtr.read((char *)&R,RSize);
            PhiFPtr.close();
            if(R.GetCVID()<0L||R.GetDelFlag()){
                std::cout << "Error in Phi::GetUpdateFlag()-inconsistent stored data" << std::endl;
                std::exit(1);
            }            
            return R.GetUpdateFlag();
        }
        Pos = sizeof(long long);
        //Pos += (sizeof(long)+Q*RSize+sizeof(long long))*NoOfPhiRec;
        Pos += (sizeof(long)+Q*RSize+sizeof(long long))*N;

        PhiFPtr.seekg(Pos+parPhiRecIndex*R1Size,ios::beg);
        PhiFPtr.read((char *)&R1,R1Size);
        PhiFPtr.close();
        if(R1.GetHostID()!=parSID||R1.GetCVID()<0L||R1.GetDelFlag()){
            std::cout << "Error in Phi::GetUpdateFlag()-inconsistent stored data" << std::endl;
            std::exit(1);
        }
        return R1.GetUpdateFlag();
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::GetUpdateFlag()- Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::GetUpdateFlag() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::GetUpdateFlag() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::GetUpdateFlag() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::GetUpdateFlag() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Phi::SetUpdateFlag(long parSID,bool parOvFlag,long long parPhiRecIndex){
if (parSID<0L||parSID>=N){
        std::cout<<"Error in Phi::SetUpdateFlag()- Invalid user ID(s)"<<std::endl;
        return false;
    }
    if ((!parOvFlag)&&(parPhiRecIndex<0L||parPhiRecIndex>=NoOfPhiRec)){
        std::cout<<"\nPhi::SetUpdateFlag():-Invalid record index("<<parPhiRecIndex<<")"<<std::endl;
        return false;
    }
    if (parOvFlag&&(parPhiRecIndex<0L||parPhiRecIndex>=NoOfPhiOvRec)){
        std::cout<<"\nPhi::SetUpdateFlag():-Invalid record index("<<parPhiRecIndex<<")"<<std::endl;
        return false;
    }
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    PhiRec R;
    PhiRecOverflow R1;
    int RSize = R.Size(), R1Size = R1.Size();
    long long ReadRecCount,Pos;
    long Degree, Q = ceil(f * q);
    try {
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        PhiFPtr.close();
        if (ReadRecCount != NoOfPhiRec){    
            std::cout << "Error in Phi::SetUpdateFlag() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }
        
        PhiFPtr.open(PhiFileName, ios::in | ios::out | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        if(!parOvFlag){
            Pos = sizeof(long long);
            Pos += (sizeof(long)+Q*RSize+sizeof(long long))*parSID;
            PhiFPtr.seekg(Pos,ios::beg);
            PhiFPtr.read((char *)&Degree, sizeof(Degree));
            if(parPhiRecIndex >= Degree){
                PhiFPtr.close();
                std::cout<<"\nPhi::SetUpdateFlag():-Invalid record index("<<parPhiRecIndex<<")"<<std::endl;
                return false;
            }
            PhiFPtr.seekg(Pos+sizeof(Degree)+parPhiRecIndex*RSize,ios::beg);
            PhiFPtr.read((char *)&R,RSize);
            if(R.GetCVID()<0L || R.GetDelFlag()){
                PhiFPtr.close();
                std::cout << "Error in Phi::SetUpdateFlag()-inconsistent stored data" << std::endl;
                std::exit(1);
            }
            R.SetUpdateFlag(true);
            PhiFPtr.seekg(Pos+sizeof(Degree)+parPhiRecIndex*RSize,ios::beg);
            PhiFPtr.write((char *)&R,RSize);
            PhiFPtr.close();
            return true;
        }
        Pos = sizeof(long long);
        //Pos += (sizeof(long)+Q*RSize+sizeof(long long))*NoOfPhiRec;
        Pos += (sizeof(long)+Q*RSize+sizeof(long long))*N;

        PhiFPtr.seekg(Pos+parPhiRecIndex*R1Size,ios::beg);
        PhiFPtr.read((char *)&R1,R1Size);
        if(R1.GetHostID()!=parSID||R1.GetCVID()<0L||R1.GetDelFlag()){
            PhiFPtr.close();
            std::cout << "Error in Phi::SetUpdateFlag()-inconsistent stored data" << std::endl;
            std::exit(1);
        }
        R1.SetUpdateFlag(true);
        PhiFPtr.seekg(Pos+parPhiRecIndex*R1Size,ios::beg);
        PhiFPtr.write((char *)&R1,R1Size);

        PhiFPtr.close();
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::SetUpdateFlag()- Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::SetUpdateFlag() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::SetUpdateFlag() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::SetUpdateFlag() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::SetUpdateFlag() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Phi::SetUpdateFlag(long parSID,long parDID,bool parOvFlag,long long parPhiRecIndex){
    if ((parSID<0L||parDID<0L)||(parSID>=N||parDID>=N)){
        std::cout<<"Error in Phi::SetUpdateFlag()- Invalid user ID(s)"<<std::endl;
        return false;
    }
    if ((!parOvFlag)&&(parPhiRecIndex<0L||parPhiRecIndex>=NoOfPhiRec)){
        std::cout<<"\nPhi::SetUpdateFlag():-Invalid record index("<<parPhiRecIndex<<")"<<std::endl;
        return false;
    }
    if (parOvFlag&&(parPhiRecIndex<0L||parPhiRecIndex>=NoOfPhiOvRec)){
        std::cout<<"\nPhi::SetUpdateFlag():-Invalid record index("<<parPhiRecIndex<<")"<<std::endl;
        return false;
    }
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    PhiRec R;
    PhiRecOverflow R1;
    int RSize = R.Size(), R1Size = R1.Size();
    long long ReadRecCount,Pos;
    long Degree, Q = ceil(f * q);
    try {
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        PhiFPtr.close();
        
        if (ReadRecCount != NoOfPhiRec){
            std::cout << "Error in Phi::SetUpdateFlag() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }
        PhiFPtr.open(PhiFileName, ios::in | ios::out | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        if(!parOvFlag){
            Pos = sizeof(long long);
            Pos += (sizeof(long)+Q*RSize+sizeof(long long))*parSID;
            PhiFPtr.seekg(Pos,ios::beg);
            PhiFPtr.read((char *)&Degree, sizeof(Degree));
            if(parPhiRecIndex >= Degree){
                PhiFPtr.close();
                std::cout<<"\nPhi::SetUpdateFlag():-Invalid record index("<<parPhiRecIndex<<")"<<std::endl;
                return false;
            }
            PhiFPtr.seekg(Pos+sizeof(Degree)+parPhiRecIndex*RSize,ios::beg);
            PhiFPtr.read((char *)&R,RSize);
            if(R.GetNeighborID()!=parDID || R.GetCVID()<0L || R.GetDelFlag()){
                PhiFPtr.close();
                std::cout << "Error in Phi::SetUpdateFlag()-inconsistent stored data" << std::endl;
                std::exit(1);
            }
            R.SetUpdateFlag(true);
            PhiFPtr.seekg(Pos+sizeof(Degree)+parPhiRecIndex*RSize,ios::beg);
            PhiFPtr.write((char *)&R,RSize);
            PhiFPtr.close();
            return true;
        }
        Pos = sizeof(long long);
        //Pos += (sizeof(long)+Q*RSize+sizeof(long long))*NoOfPhiRec;
        Pos += (sizeof(long)+Q*RSize+sizeof(long long))*N;

        PhiFPtr.seekg(Pos+parPhiRecIndex*R1Size,ios::beg);
        PhiFPtr.read((char *)&R1,R1Size);
        if(R1.GetNeighborID()!=parDID||R1.GetHostID()!=parSID||R1.GetCVID()<0L||R1.GetDelFlag()){
            PhiFPtr.close();
            std::cout << "Error in Phi::SetUpdateFlag()-inconsistent stored data" << std::endl;
            std::exit(1);
        }
        R1.SetUpdateFlag(true);
        PhiFPtr.seekg(Pos+parPhiRecIndex*R1Size,ios::beg);
        PhiFPtr.write((char *)&R1,R1Size);

        PhiFPtr.close();
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::SetUpdateFlag()- Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::SetUpdateFlag() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::SetUpdateFlag() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::SetUpdateFlag() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::SetUpdateFlag() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Phi::SetResetUpdateFlag(long parSID, long parDID, bool parUpdateFlag){
    if (((parSID<0L||parDID<0L)||(parSID>=N||parDID>=N))){
        std::cout<<"Error in Phi::SetResetUpdateFlag()- Invalid user ID(s)"<<std::endl;
        return false;
    }
    long long PhiRecIndex;
    bool OvFlag;
    if(!GetPhiRecIndex(parSID,parDID,OvFlag,PhiRecIndex)){
        std::cout << "Error in Phi::SetResetUpdateFlag()- no record to delete" << std::endl;
        return false;
    }
    if ((!OvFlag) && (PhiRecIndex < 0L || PhiRecIndex >= NoOfPhiRec)){
        std::cout<<"\nPhi::SetResetUpdateFlag():-Invalid record index("<<PhiRecIndex<<")"<<std::endl;
        return false;
    }
    if (OvFlag && (PhiRecIndex < 0L || PhiRecIndex >= NoOfPhiOvRec)){
        std::cout<<"\nPhi::SetResetUpdateFlag():-Invalid record index("<<PhiRecIndex<<")"<<std::endl;
        return false;
    }
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    PhiRec R;
    PhiRecOverflow R1;
    int RSize = R.Size(), R1Size = R1.Size();
    long long ReadRecCount,Pos;
    long Degree, Q = ceil(f * q);
    try {
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        if (ReadRecCount != NoOfPhiRec){
            PhiFPtr.close();
            std::cout << "Error in Phi::SetResetUpdateFlag() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }
        PhiFPtr.open(PhiFileName, ios::in | ios::out | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        if(!OvFlag){
            Pos = sizeof(long long);
            Pos += (sizeof(long)+Q*RSize+sizeof(long long))*parSID;
            PhiFPtr.seekg(Pos,ios::beg);
            PhiFPtr.read((char *)&Degree, sizeof(Degree));
            if(PhiRecIndex >= Degree){
                PhiFPtr.close();
                std::cout<<"\nPhi::SetResetUpdateFlag():-Invalid record index("<<PhiRecIndex<<")"<<std::endl;
                return false;
            }
            PhiFPtr.seekg(Pos+sizeof(Degree)+PhiRecIndex*RSize,ios::beg);
            PhiFPtr.read((char *)&R,RSize);
            R.SetUpdateFlag(parUpdateFlag);
            PhiFPtr.seekg(Pos+sizeof(Degree)+PhiRecIndex*RSize,ios::beg);
            PhiFPtr.write((char *)&R,RSize);
            PhiFPtr.close();
            return true;
        }
        Pos = sizeof(long long);
        //Pos += (sizeof(long)+Q*RSize+sizeof(long long))*NoOfPhiRec;
        Pos += (sizeof(long)+Q*RSize+sizeof(long long))*N;

        PhiFPtr.seekg(Pos+PhiRecIndex*R1Size,ios::beg);
        PhiFPtr.read((char *)&R1,R1Size);
        R1.SetUpdateFlag(parUpdateFlag);
        PhiFPtr.seekg(Pos+PhiRecIndex*R1Size,ios::beg);
        PhiFPtr.write((char *)&R1,R1Size);

        PhiFPtr.close();
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::SetResetUpdateFlag()- Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::SetResetUpdateFlag() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::SetResetUpdateFlag() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::SetResetUpdateFlag() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::SetResetUpdateFlag() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Phi::ResetAllUpdateFlags(){
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    PhiRec R;
    PhiRecOverflow R1;
    int RSize = R.Size(), R1Size = R1.Size();
    long long ReadRecCount,PhiRecIndex,Pos;
    long id, i, Degree, Q = ceil(f * q);
    try {
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        PhiFPtr.close();

        if (ReadRecCount != NoOfPhiRec){
            std::cout << "Error in Phi::ResetAllUpdateFlags() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }
        PhiFPtr.open(PhiFileName, ios::in | ios::out | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked
        
        for(id=0;id<N;id++){
            Pos = sizeof(long long);
            Pos += (sizeof(long)+Q*RSize+sizeof(long long))*id;
            PhiFPtr.seekg(Pos,ios::beg);
            PhiFPtr.read((char *)&Degree, sizeof(Degree));            
            if (Degree <= Q){
                for(i=0;i<Degree;i++){
                    PhiFPtr.seekg(Pos+sizeof(long)+i*RSize,ios::beg);
                    PhiFPtr.read((char *)&R,RSize);
                    R.SetUpdateFlag(false);
                    PhiFPtr.seekg(Pos+sizeof(long)+i*RSize,ios::beg);
                    PhiFPtr.write((char *)&R,RSize);
                }
            }
            if (Degree>Q){
                Pos = sizeof(long long);
                //Pos += (sizeof(long)+Q*RSize+sizeof(long long))*NoOfPhiRec;
                Pos += (sizeof(long)+Q*RSize+sizeof(long long))*N;
                Pos -= sizeof(long long);
                PhiFPtr.seekg(Pos,ios::beg);
                PhiFPtr.read((char *)&PhiRecIndex,sizeof(PhiRecIndex));
                Pos += sizeof(long long);
                while(PhiRecIndex>=0L){
                    PhiFPtr.seekg(Pos+PhiRecIndex*R1Size,ios::beg);
                    PhiFPtr.read((char *)&R1,R1Size);
                    R1.SetUpdateFlag(false);
                    PhiFPtr.seekg(Pos+PhiRecIndex*R1Size,ios::beg);
                    PhiFPtr.write((char *)&R1,R1Size);
                    PhiRecIndex = R1.GetNext();
                }
            }
        }
        PhiFPtr.close();
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::ResetAllUpdateFlags()  - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::ResetAllUpdateFlags() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::ResetAllUpdateFlags() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::ResetAllUpdateFlags() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::ResetAllUpdateFlags() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Phi::GetNoUpdateList(){
    /*
    This function forms the contact list, we call them as No-Update List, this is for 
    the existing contacts for which no close contact data is found in the last 
    streaming period.
    We stote this list in a newly created file.
    Structure of the list stored in a file:
    Field Names:1. Number of Record
    Each record with fields: 2. User ID, 3. No of records in non-overflow area
    4. No of records in overflow area, 5. Non-overflow records - Index of non-overflow Phi Records
    6. Overflow records - Index of overflow Phi records
    */
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream NoUpdatedListFPtr;
    NoUpdatedListFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

    PhiRec R;
    PhiRecOverflow R1;
    int RSize = R.Size(), R1Size = R1.Size();
    long long ReadRecCount,PhiRecIndex,Pos,Pos1,Pos2;
    long UID, Degree, Q = ceil(f * q);
    long NoOfDirectContacts=0,NoOfOvContacts=0;
    bool Flag;
    try {    
        NoUpdatedListFPtr.open(NoUpdateListFileName, ios::out|ios::binary);
        if (NoUpdatedListFPtr.fail()) throw NoUpdateListFileName;  // the exception being checked
        
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&ReadRecCount, sizeof(ReadRecCount));
        if (ReadRecCount != NoOfPhiRec){
            PhiFPtr.close();
            NoUpdatedListFPtr.close();
            std::cout << "Error in Phi::GetNoUpdateList() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }
        ReadRecCount=0; // Now use it as working variable
        NoUpdatedListFPtr.seekg(0, ios::beg);
        NoUpdatedListFPtr.write((char *)&ReadRecCount, sizeof(ReadRecCount));
        /*
            Process all records in the non-overflow area.
            Pos1 -> for positioning in the output list 
            Pos -> for positioning in the Phi records
        */
        Pos1 = NoUpdatedListFPtr.tellg();
        for(UID=0;UID<N;UID++){
            NoOfDirectContacts = 0;
            NoOfOvContacts = 0;
            Flag = false;
            Pos = sizeof(long long);
            Pos += (sizeof(long)+Q*RSize+sizeof(long long))*UID;
            PhiFPtr.seekg(Pos,ios::beg);
            PhiFPtr.read((char *)&Degree, sizeof(Degree));
            /*
                User has no records in the overflow area. So check whether there is any
                un-updated record or not. New list record is created only when there is
                at least one non-updated record either in non-overflow, overflow area or
                both area.  
            */
            if (Degree <= Q){
                /* User has no record in the overflow area.*/
                for(PhiRecIndex=0;PhiRecIndex<Degree;PhiRecIndex++){
                    PhiFPtr.seekg(Pos+sizeof(long)+PhiRecIndex*RSize,ios::beg);
                    PhiFPtr.read((char *)&R,RSize);
                    if(!R.GetUpdateFlag())break;
                }
                //if found then create a record in the list file.
                if (PhiRecIndex<Degree){
                    Flag = true;
                    NoUpdatedListFPtr.seekg(Pos1,ios::beg);
                    NoUpdatedListFPtr.write((char *)&UID, sizeof(UID));
                    NoUpdatedListFPtr.write((char *)&NoOfDirectContacts, sizeof(NoOfDirectContacts));
                    NoUpdatedListFPtr.write((char *)&NoOfOvContacts, sizeof(NoOfOvContacts));
                    NoUpdatedListFPtr.write((char *)&PhiRecIndex, sizeof(PhiRecIndex));
                    NoOfDirectContacts = 1;
                }
                for(++PhiRecIndex;PhiRecIndex<Degree;PhiRecIndex++){
                    PhiFPtr.seekg(Pos+sizeof(long)+PhiRecIndex*RSize,ios::beg);
                    PhiFPtr.read((char *)&R,RSize);
                    if(!R.GetUpdateFlag()){
                        NoUpdatedListFPtr.write((char *)&PhiRecIndex, sizeof(PhiRecIndex));
                        NoOfDirectContacts++;
                    }
                }
            }
            if(Degree>Q){
                /* 
                    User has records in the non-overflow and overflow areas. First 
                    process the non-overflow area and then the overflow area.
                */
                for(PhiRecIndex=0;PhiRecIndex<Q;PhiRecIndex++){
                    PhiFPtr.seekg(Pos+sizeof(long)+PhiRecIndex*RSize,ios::beg);
                    PhiFPtr.read((char *)&R,RSize);
                    if(!R.GetUpdateFlag())break;
                }
                if (PhiRecIndex<Q){
                    Flag = true;
                    NoUpdatedListFPtr.seekg(Pos1,ios::beg);
                    NoUpdatedListFPtr.write((char *)&UID, sizeof(UID));
                    NoUpdatedListFPtr.write((char *)&NoOfDirectContacts, sizeof(NoOfDirectContacts));
                    NoUpdatedListFPtr.write((char *)&NoOfOvContacts, sizeof(NoOfOvContacts));
                    NoUpdatedListFPtr.write((char *)&PhiRecIndex, sizeof(PhiRecIndex));
                    NoOfDirectContacts = 1;
                }
                for(++PhiRecIndex;PhiRecIndex<Q;PhiRecIndex++){
                    PhiFPtr.seekg(Pos+sizeof(long)+PhiRecIndex*RSize,ios::beg);
                    PhiFPtr.read((char *)&R,RSize);
                    if(!R.GetUpdateFlag()){
                        NoUpdatedListFPtr.write((char *)&PhiRecIndex, sizeof(PhiRecIndex));
                        NoOfDirectContacts++;
                    }
                }
                /* 
                    Now process records in the overflow area.
                    Get the index of the starting record in the overflow area for the
                    current user.
                */
                PhiFPtr.seekg(Pos+sizeof(long)+Q*RSize,ios::beg);
                PhiFPtr.read((char *)&PhiRecIndex,sizeof(PhiRecIndex));

                Pos = sizeof(long long);
                Pos += (sizeof(long)+Q*RSize+sizeof(long long))*N;
                while(PhiRecIndex>=0L){
                    PhiFPtr.seekg(Pos+PhiRecIndex*R1Size,ios::beg);
                    PhiFPtr.read((char *)&R1,R1Size);
                    if (!R1.GetUpdateFlag()){
                        if (Flag==false){
                            Flag = true;
                            NoUpdatedListFPtr.seekg(Pos1,ios::beg);
                            NoUpdatedListFPtr.write((char *)&UID, sizeof(UID));
                            NoUpdatedListFPtr.write((char *)&NoOfDirectContacts, sizeof(NoOfDirectContacts));
                            NoUpdatedListFPtr.write((char *)&NoOfOvContacts, sizeof(NoOfOvContacts));
                            NoUpdatedListFPtr.write((char *)&PhiRecIndex, sizeof(PhiRecIndex));
                            NoOfOvContacts = 1;
                        }
                        else{
                            NoUpdatedListFPtr.write((char *)&PhiRecIndex, sizeof(PhiRecIndex));
                            NoOfOvContacts++;
                        }
                    }
                    PhiRecIndex = R1.GetNext();
                }
            }

            if ((NoOfDirectContacts+NoOfOvContacts)>0){
                Pos2 = NoUpdatedListFPtr.tellg();
                if(NoOfDirectContacts>0L){
                    NoUpdatedListFPtr.seekg(Pos1+sizeof(UID),ios::beg);
                    NoUpdatedListFPtr.write((char *)&NoOfDirectContacts, sizeof(NoOfDirectContacts));
                }
                if(NoOfOvContacts>0L){
                    NoUpdatedListFPtr.seekg(Pos1+sizeof(UID)+sizeof(NoOfDirectContacts),ios::beg);
                    NoUpdatedListFPtr.write((char *)&NoOfOvContacts, sizeof(NoOfOvContacts));
                }
                Pos1 = Pos2;
                ReadRecCount += (NoOfDirectContacts+NoOfOvContacts);
            }
        }
        PhiFPtr.close();
        if(ReadRecCount>0L){
            NoUpdatedListFPtr.seekg(0,ios::beg);
            NoUpdatedListFPtr.write((char *)&ReadRecCount,sizeof(ReadRecCount));
            NoUpdatedListFPtr.close();
            return true;
        }
        else {
            NoUpdatedListFPtr.close();
            return false;
        }
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::GetNoUpdateList()  - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::GetNoUpdateList() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::GetNoUpdateList() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::GetNoUpdateList() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::GetNoUpdateList() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

void Phi::ShowNoUpdateList(){

    std::fstream NoUpdatedListFPtr;
    NoUpdatedListFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    
    long NoOfDirectContacts=0,NoOfOvContacts=0,UID,i;
    long long ReadRecCount,PhiRecIndex;
    
    try {    
        NoUpdatedListFPtr.open(NoUpdateListFileName, ios::in|ios::binary);
        if (NoUpdatedListFPtr.fail()) throw NoUpdateListFileName;  // the exception being checked
    
        NoUpdatedListFPtr.read((char *)&ReadRecCount, sizeof(ReadRecCount));

        if(ReadRecCount==0){
            //std::cout<<"No record in the No-Update List"<<std::endl;
            NoUpdatedListFPtr.close();
            return;
        }
        //std::cout<<"List Size(Non-Updated CV)="<<ReadRecCount<<std::endl;
        while(true){
            NoUpdatedListFPtr.read((char *)&UID, sizeof(UID));
            NoUpdatedListFPtr.read((char *)&NoOfDirectContacts, sizeof(NoOfDirectContacts));
            NoUpdatedListFPtr.read((char *)&NoOfOvContacts, sizeof(NoOfOvContacts));
            
            //std::cout<<"UID="<<UID<<" NoOfDirectContacts="<<NoOfDirectContacts
            //    <<" NoOfOvContacts="<<NoOfOvContacts<<std::endl;
            //std::cout<<"PhiRecIndex(Direct):"<<std::endl;
            for(i=0;i<NoOfDirectContacts;i++){
                NoUpdatedListFPtr.read((char *)&PhiRecIndex, sizeof(PhiRecIndex));
                //std::cout<<PhiRecIndex<<" ";
            }
            //std::cout<<"\nPhiRecIndex(Overflow):"<<std::endl;
            for(i=0;i<NoOfOvContacts;i++){
                NoUpdatedListFPtr.read((char *)&PhiRecIndex, sizeof(PhiRecIndex));
                //std::cout<<PhiRecIndex<<" ";
            }
            ReadRecCount-=(NoOfDirectContacts+NoOfOvContacts);
            if (ReadRecCount==0)break;
        }
        NoUpdatedListFPtr.close();
        return;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::ShowNoUpdateList()  - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::ShowNoUpdateList() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::ShowNoUpdateList() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::ShowNoUpdateList() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::ShowNoUpdateList() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

void Phi::GetNoUpdateListFileName(char parFileName[47]){
    strcpy(parFileName,NoUpdateListFileName);
    return;
}

void Phi::GetUsedCVIDListFileName(char parFileName[47]){
    strcpy(parFileName,UsedCVIDListFileName);
    return;
}

bool Phi::GetDgree(long parSID, long &parDegree){
    if (parSID<0L || parSID>=N){
        std::cout<<"Error in Phi::GetDgree()- Invalid user ID(s)"<<std::endl;
        return false;
    }
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    PhiRec R;
    int RSize = R.Size();
    long long Pos, RecCount;
    long Q = ceil(f * q);
    try {            
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&RecCount, sizeof(RecCount));
        if (RecCount != NoOfPhiRec){
            PhiFPtr.close();
            std::cout << "Error in Phi::GetDgree() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }
        Pos = sizeof(long long);
        Pos += (sizeof(long)+Q*RSize+sizeof(long long))*parSID;
        PhiFPtr.seekg(Pos,ios::beg);
        PhiFPtr.read((char *)&parDegree, sizeof(parDegree));
        PhiFPtr.close();
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::GetDgree()  - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::GetDgree() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::GetDgree() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::GetDgree() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::GetDgree() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

long long Phi::GetTotalDgree(){
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    PhiRec R;
    int RSize = R.Size();
    long long Pos=0, RecCount=0, TotalDegree=0;
    long id,Q = ceil(f * q),Degree;
    try {            
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&RecCount, sizeof(RecCount));
        if (RecCount != NoOfPhiRec){
            PhiFPtr.close();
            std::cout << "Error in Phi::GetTotalDgree() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }
        Pos = sizeof(long long);
        for(id=0;id<N;id++){
            Pos += (sizeof(long)+Q*RSize+sizeof(long long))*id;
            PhiFPtr.seekg(Pos,ios::beg);
            PhiFPtr.read((char *)&Degree, sizeof(Degree));
            TotalDegree+=Degree;
        }
        PhiFPtr.close();
        return TotalDegree;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::GetTotalDgree()  - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::GetTotalDgree() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::GetTotalDgree() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::GetTotalDgree() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::GetTotalDgree() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

long Phi::GetMaxDegree(){
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    PhiRec R;
    int RSize = R.Size();
    long long Pos, RecCount;
    long id, Q = ceil(f * q), MaxDegree=0, Degree;
    try {            
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&RecCount, sizeof(RecCount));
        if (RecCount != NoOfPhiRec){
            PhiFPtr.close();
            std::cout << "Error in Phi::GetMaxDgree() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }
        Pos = sizeof(long long);
        for(id=0;id<N;id++){
            Pos = sizeof(long long)+(sizeof(long)+Q*RSize+sizeof(long long))*id;
            PhiFPtr.seekg(Pos,ios::beg);
            PhiFPtr.read((char *)&Degree, sizeof(Degree));
            if(Degree > MaxDegree)MaxDegree=Degree;         
        }
        PhiFPtr.close();
        return MaxDegree;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::GetMaxDgree()  - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::GetMaxDgree() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::GetMaxDgree() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::GetMaxDgree() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::GetMaxDgree() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

long Phi::GetMinDegree(){
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    PhiRec R;
    int RSize = R.Size();
    long long Pos, RecCount;
    long id, Q = ceil(f * q), MinDegree=0, Degree;
    try {            
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&RecCount, sizeof(RecCount));
        if (RecCount != NoOfPhiRec){
            PhiFPtr.close();
            std::cout << "Error in Phi::GetMaxDgree() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }
        Pos = sizeof(long long);
        for(id=0;id<N;id++){
            Pos = sizeof(long long)+(sizeof(long)+Q*RSize+sizeof(long long))*id;
            PhiFPtr.seekg(Pos,ios::beg);
            PhiFPtr.read((char *)&Degree, sizeof(Degree));
            if(Degree > MinDegree)MinDegree=Degree;         
        }
        PhiFPtr.close();
        return MinDegree;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::GetMaxDgree()  - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::GetMaxDgree() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::GetMaxDgree() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::GetMaxDgree() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::GetMaxDgree() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Phi::GetDegreeDistribution(std::string parFlName){
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    
    std::fstream DegreeFPtr;
    DegreeFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

    PhiRec R;
    int RSize = R.Size();
    long long Pos, RecCount;
    long id, Q = ceil(f * q), Degree,DegreeCount=0;
    try {            
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&RecCount, sizeof(RecCount));
        if (RecCount != NoOfPhiRec){
            PhiFPtr.close();
            std::cout << "Error in Phi::GetDegreeDistribution() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }

        DegreeFPtr.open(parFlName.c_str(), ios::out | ios::binary);
        if (DegreeFPtr.fail()) throw parFlName.c_str();  // the exception being checked

        for(id=0;id<N;id++){
            DegreeFPtr.write((char *)&DegreeCount,sizeof(DegreeCount));
        }
        DegreeFPtr.close();

        DegreeFPtr.open(parFlName.c_str(), ios::in|ios::out|ios::binary);
        if (DegreeFPtr.fail()) throw parFlName.c_str();  // the exception being checked

        for(id = 0; id < N; id++){
            Pos = sizeof(long long)+(sizeof(long)+Q*RSize+sizeof(long long))*id;
            PhiFPtr.seekg(Pos,ios::beg);
            PhiFPtr.read((char *)&Degree, sizeof(Degree));
            DegreeFPtr.seekg(Degree*sizeof(long),ios::beg);
            DegreeFPtr.read((char *)&DegreeCount, sizeof(DegreeCount));
            DegreeCount++;
            DegreeFPtr.seekg(Degree*sizeof(long),ios::beg);
            DegreeFPtr.write((char *)&DegreeCount, sizeof(DegreeCount));
        }
        PhiFPtr.close();
        DegreeFPtr.close();
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::GetDegreeDistribution()  - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::GetDegreeDistribution() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::GetDegreeDistribution() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::GetDegreeDistribution() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::GetDegreeDistribution() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

bool Phi::GetUsedCVIDList(){
    std::fstream PhiFPtr;
    PhiFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::fstream UsedCVIDListFPtr;
    UsedCVIDListFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

    PhiRec R;
    PhiRecOverflow R1;
    int RSize = R.Size(), R1Size = R1.Size();
    long long RecCount,PhiRecIndex,Pos,CVID;
    long id,i, Degree, Q = ceil(f * q);
    try {    
        UsedCVIDListFPtr.open(UsedCVIDListFileName, ios::out|ios::binary);
        if (UsedCVIDListFPtr.fail()) throw UsedCVIDListFileName;  // the exception being checked
        
        PhiFPtr.open(PhiFileName, ios::in | ios::binary);
        if (PhiFPtr.fail()) throw PhiFileName;  // the exception being checked

        PhiFPtr.read((char*)&RecCount, sizeof(RecCount));
        if (RecCount != NoOfPhiRec){
            PhiFPtr.close();
            UsedCVIDListFPtr.close();
            std::cout << "Error in Phi::GetUsedCVIDList() " << " mismatch in the record count" << std::endl;
            std::exit(1);
        }
        RecCount=0;
        UsedCVIDListFPtr.seekg(0, ios::beg);
        UsedCVIDListFPtr.write((char *)&RecCount, sizeof(RecCount));
        for(id=0;id<N;id++){
            Pos = sizeof(long long);
            Pos += (sizeof(long)+Q*RSize+sizeof(long long))*id;
            PhiFPtr.seekg(Pos,ios::beg);
            PhiFPtr.read((char *)&Degree, sizeof(Degree));
            if (Degree <= Q){
                for(i=0;i<Degree;i++){
                    PhiFPtr.seekg(Pos+sizeof(long)+i*RSize,ios::beg);
                    PhiFPtr.read((char *)&R,RSize);
                    if(!R.GetDelFlag()){
                        CVID = R.GetCVID();
                        UsedCVIDListFPtr.write((char *)&CVID, sizeof(CVID));
                        RecCount++;
                    }                    
                }
            }
            if (Degree>Q){
                Pos = sizeof(long long);
                //Pos += (sizeof(long)+Q*RSize+sizeof(long long))*NoOfPhiRec;
                Pos += (sizeof(long)+Q*RSize+sizeof(long long))*N;
                Pos -= sizeof(long long);
                PhiFPtr.seekg(Pos,ios::beg);
                PhiFPtr.read((char *)&PhiRecIndex,sizeof(PhiRecIndex));
                Pos += sizeof(long long);
                while(PhiRecIndex>=0L){
                    PhiFPtr.seekg(Pos+PhiRecIndex*R1Size,ios::beg);
                    PhiFPtr.read((char *)&R1,R1Size);
                    if (!R1.GetDelFlag()){
                        CVID = R1.GetCVID();
                        UsedCVIDListFPtr.write((char *)&CVID, sizeof(CVID));
                        RecCount++;
                    }
                    PhiRecIndex = R1.GetNext();
                }
            }
        }
        PhiFPtr.close();
        if(RecCount>0L){
            UsedCVIDListFPtr.seekg(0,ios::beg);
            UsedCVIDListFPtr.write((char *)&RecCount,sizeof(RecCount));
            UsedCVIDListFPtr.close();
            return true;
        }
        else {
            UsedCVIDListFPtr.close();
            return false;
        }
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::GetNoUpdateList()  - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::GetNoUpdateList() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "Phi::GetNoUpdateList() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "Phi::GetNoUpdateList() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "Phi::GetNoUpdateList() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

//************************************ End of Phi Library ********************************

//****************************************************************************************
//*********************************** Contact Graph Library ******************************
//****************************************************************************************

ContactGraph::ContactGraph(){
	std::cout << "Loading contact graph from stored data" << std::endl;
	NoRecFlag = 0;
	try{
		PHI = new Phi();
		TH = new Theta();
	}
	catch (const std::bad_alloc& e){
		std::cout << "ContactGraph::ContactGraph() " << e.what() << " Exception occu" << std::endl;
		std::exit(1);
	}
	catch (exception& e){
		std::cout << "ContactGraph::ContactGraph() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

ContactGraph::ContactGraph(long parN,long parq,int parNoOfDays,int parDayDuration,int parSlotDuration,int parDelta, float parf){
	std::cout << "Creating a new Contact Graph" << std::endl;
	NoRecFlag = 0;
	try {
		PHI = new Phi(parN, parq, parf);
		TH = new Theta(parN, parq, parNoOfDays, parDayDuration, parSlotDuration, parDelta, parf);
	}
	catch (const std::bad_alloc& e){
		std::cout << "ContactGraph::ContactGraph(parN,parq, ...) " << e.what() << " Exception occu" << std::endl;
		std::exit(1);
	}
	catch (exception& e){
		std::cout << "ContactGraph::ContactGraph(parN,parq, ...) " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

void ContactGraph::Show(){
    try{
		std::cout << "\nStructure of the Contact Graph:" << std::endl;
        if (PHI == NULL || TH == NULL){
            std::cout<<"\nError:ContactGraph::Show() - either Phi, Theta or both are NULL"<<std::endl;
            std::exit(1);            
        }

		std::cout << *PHI << std::endl;
		std::cout << *TH << std::endl;
	}
	catch (const std::bad_alloc& e){
		std::cout << "ContactGraph::Show() " << e.what() << " Exception occu" << std::endl;
		std::exit(1);
	}
	catch (exception& e){
		std::cout << "ContactGraph::Show() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

ostream& operator<<(ostream& out, const ContactGraph& parCG){
	try{
		out << "\nStructure of the Contact Graph:" << std::endl;
		std::cout << *parCG.PHI << std::endl;
		std::cout << *parCG.TH << std::endl;
		return out;
	}
	catch (const std::bad_alloc& e){
		std::cout << "ContactGraph::operator<<() " << e.what() << " Exception occu" << std::endl;
		std::exit(1);
	}
	catch (exception& e){
		std::cout << "ContactGraph::operator<<() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

set<long> ContactGraph::GetContactTrace(set<long> parIndividuals){
	try{
		set<long> Trace;
		Trace.clear();
		return Trace;
	}
	catch (const std::bad_alloc& e){
		std::cout << "ContactGraph::GetContactTrace(parIndividuals) " << e.what() << " Exception occur" << std::endl;
		std::exit(1);
	}
	catch (exception& e){
		std::cout << "ContactGraph::GetContactTrace(parIndividuals) " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

int ContactGraph::GetNoOfObservationDays(){
    return TH->GetNoOfDays();
}
int ContactGraph::GetSlotDuration(){
    return TH->GetSlotDuration();

}
int ContactGraph::GetDayDuration(){
    return TH->GetDayDuration();
}
long ContactGraph::GetN(){
    return TH->GetN();
}
long ContactGraph::Getq(){
    return TH->Getq();
}

int ContactGraph::GetNoOfCloseContactSlots(){
    return TH->GetNoOfCloseContactSlots();
}

int ContactGraph::GetNoOfCloseContactsDuringADay(){
    int SlotDuration, DayDuration, NoOfClosedContacts;
    SlotDuration = TH->GetSlotDuration();
    DayDuration = TH->GetDayDuration();
    NoOfClosedContacts = ceil((double)(DayDuration * 60.0)/SlotDuration);
    return NoOfClosedContacts;
}

bool ContactGraph::InitializeContact(int parCCFront, int parCCRear, int parDCFront, int parDCRear){
	/* This function will initialize all close contacts slots (bits) to false which are
	immediate after rear of the contact vector queue. This will initialize all such bits
	available in the time duration parT hours. This is done for all the contact vectors
	currently in use. Note that we can find these contact vectors after following the contacts
	of edges available in Phi structure.
	This will do similar initialization for the day part of these contact vectors.
	*/
	char UsedCVIDListFileName[47];

    std::fstream UsedCVIDListFPtr;
    UsedCVIDListFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    
    long long RecCount,CVID, RecID;
    PhiRec R;
    try {  
		int NoOfDay = TH->GetNoOfDays();
		int DayDuration = TH->GetDayDuration();
		int SlotDuration = TH->GetSlotDuration();
        long long NoOfContactVectors=TH->GetNoOfContactVectors();

		if (SlotDuration == 0)
			throw std::overflow_error("Divide by zero exception");

		int NoOfSlots = ceil((double)(DayDuration * 60.0) / SlotDuration);
	    std::vector<bool> EdgeLabel(NoOfSlots, false);
	    ContactVector CV(NoOfDay, DayDuration, SlotDuration);
    	
        CV.SetCCQueueFront(parCCFront);
		CV.SetCCQueueRear(parCCRear);
		CV.SetDCQueueFront(parDCFront);
		CV.SetDCQueueRear(parDCRear);
        PHI->GetUsedCVIDListFileName(UsedCVIDListFileName);
        if(!PHI->GetUsedCVIDList()){
			std::cout << "Error in ContactGraph::InitializeContact(): Phi Record is not accessible" << std::endl;
			std::exit(1);
        }

        UsedCVIDListFPtr.open(UsedCVIDListFileName, ios::in|ios::binary);
        if (UsedCVIDListFPtr.fail()) throw UsedCVIDListFileName;  // the exception being checked

        UsedCVIDListFPtr.read((char *)&RecCount, sizeof(RecCount));
		if (RecCount == 0)
			return false;
		for (RecID = 0; RecID < RecCount; RecID++){
            UsedCVIDListFPtr.read((char *)&CVID, sizeof(CVID));
			if (CVID<0L||CVID>NoOfContactVectors){
				std::cout << "Error in ContactGraph::InitializeContact(): Phi Record is not accessible" << std::endl;
				std::exit(1);
			}
			if (TH->GetCV(CVID, CV)){
				CV.EnQueue(EdgeLabel);
				TH->SetCV(CVID, CV);
			}
			else{
				std::cout << "Error in ContactGraph::InitializeContact(): Contact Vector for the contact is not accessible" << std::endl;
				std::exit(1);
			}
		}
        UsedCVIDListFPtr.close();
		return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::GetNoUpdateList()  - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "Phi::GetNoUpdateList() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
	catch (const std::bad_alloc& e){
		std::cout << "ContactGraph::InitializeContact() " << e.what() << " Exception occu" << std::endl;
		std::exit(1);
	}
	catch (std::out_of_range& e){
		std::cout << "ContactGraph::InitializeContact() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
	catch (std::overflow_error & e){
		std::cout << "ContactGraph::InitializeContact() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "ContactGraph::InitializeContact() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

int ContactGraph::Size(){
	return sizeof(this);
}

//Shows the secondary / disk storage requirements of the contact graph
void ContactGraph::StorageSize(long long &parPhiCalSize, long long &parPhiObsrvSize,long long &parThetaCalSize,
	long long &parThetaObsrvSize){
	//std::cout << "\nSize of the Contact Graph is the sum of sizes of the Phi, Theta and other related objects" << std::endl;
	long long THCalSize=0,THObsrvSize=0, PHICalSize=0, PHIObsrvSize=0;
	try {
        //std::cout<<"\nGet Theta Storage size"<<std::endl;
		TH->StorageSize(THCalSize,THObsrvSize);
		//std::cout << "Size of Theta=" << THSize << std::endl;
        //std::cout<<"\nGet Phi Storage size"<<std::endl;
		PHI->StorageSize(PHICalSize, PHIObsrvSize);
        //std::cout << "Size of Phi=" << PHISize << std::endl;
		parThetaCalSize = THCalSize;
        parThetaObsrvSize = THObsrvSize;
		parPhiCalSize = PHICalSize;
        parPhiObsrvSize = PHIObsrvSize;
	}
	catch (std::bad_alloc& e){
		std::cout << "ContactGraph::StorageSize() " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (exception& e){
		std::cout << "ContactGraph::StorageSize() " << e.what() << "Exception in storage size evaluation" << std::endl;
		std::exit(1);
	}
}

//Shows the secondary / disk storage requirements of the contact graph
void ContactGraph::StorageSize(long long &parCalSize, long long &parObsrvSize){
	//std::cout << "\nSize of the Contact Graph is the sum of sizes of the Phi, Theta and other related objects" << std::endl;
	long long THCalSize=0,THObsrvSize=0, PHICalSize=0, PHIObsrvSize=0;
	try {
        //std::cout<<"\nGet Theta Storage size"<<std::endl;
		TH->StorageSize(THCalSize,THObsrvSize);
		//std::cout << "Size of Theta=" << THSize << std::endl;
        //std::cout<<"\nGet Phi Storage size"<<std::endl;
		PHI->StorageSize(PHICalSize, PHIObsrvSize);
        //std::cout << "Size of Phi=" << PHISize << std::endl;
		parCalSize = THCalSize+PHICalSize;
        parObsrvSize = THObsrvSize+PHIObsrvSize;
	}
	catch (std::bad_alloc& e){
		std::cout << "ContactGraph::StorageSize() " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (exception& e){
		std::cout << "ContactGraph::StorageSize() " << e.what() << "Exception in storage size evaluation" << std::endl;
		std::exit(1);
	}
}

bool ContactGraph::ClearContactGraph(int &parCCFront, int &parCCRear, int &parDCFront, 
        int &parDCRear){
        if (parCCFront < -1L || parCCRear <-1L || parDCFront < -1L || parDCRear < -1L){
            return false;
        }
    /*
        All sampling based contact data of the last observation duration must be available
        within a stipulated streaming time. After experiy of this streaming time a list of
        contacts is prepared for which no update i.e. contact data not received. Now, for
        these contact vectors this function update all contact slot bits for day duration
        with false value. 
        
        In an exceptional situation, if any such missed contact is received in later 
        time but not later than system's defined observations days then all these false
        contacts may be updated with actual values. 
        
        It is observed that if there is no contacts during last all observation days, 
        then this contact record will be removed from the contact graph structure.
    */
    PHI->ShowNoUpdateList();

    std::fstream NoUpdateListFPtr;
    NoUpdateListFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

	int NoOfDay = TH->GetNoOfDays();
	int DayDuration = TH->GetDayDuration();
	int SlotDuration = TH->GetSlotDuration();
	
	if (SlotDuration == 0)
		throw std::overflow_error("Divide by zero exception");

	int NoOfSlots = ceil((double)(DayDuration * 60.0) / SlotDuration);
		
  	std::vector<bool> EdgeLabel(NoOfSlots, false);
	ContactVector CV(NoOfDay, DayDuration, SlotDuration);

    char FileName[47];
    long long RecCount,DirectRecCount=0,OvRecCount=0,PhiRecIndex,CVID;
    long long Pos, UID=-1L;
    /*These local circular queue indices are required as en-queue and de-queue 
    operations are automatically returing these call by reference type indices*/
    int LCCFront, LCCRear, LDCFront, LDCRear;
    try{
        PHI->GetNoUpdateListFileName(FileName);
        NoUpdateListFPtr.open(FileName, ios::in|ios::binary);
        if (NoUpdateListFPtr.fail()) throw FileName;  // the exception being checked

        NoUpdateListFPtr.seekg(0, ios::beg);
        /*Get the Number of un-updated record*/
        NoUpdateListFPtr.read((char *)&RecCount, sizeof(RecCount));
        if(RecCount==0){
                NoUpdateListFPtr.close();
                std::cout<<"All records are updated"<<std::endl;
                return true;
        }
        std::cout<<"Number of records not updated in this day:"<<RecCount<<std::endl;
        Pos = sizeof(long long);
        /* Process all such non-updated records 
           Structure of the list stored in a file:
           Field Names:1. Number of Record
           Each record with fields: 2. User ID, 3. No of records in non-overflow area
           4. No of records in overflow area, 5. Non-overflow records - Index of non-overflow Phi Records
           6. Overflow records - Index of overflow Phi records
        */
        long long p;
        
        while(true){
            /*  Points to the starting of the next record
                and read the user ID, number of non-overflow records and number of 
                overflow records.
            */
            NoUpdateListFPtr.seekg(Pos,ios::beg);
            NoUpdateListFPtr.read((char *)&UID, sizeof(UID));
            NoUpdateListFPtr.read((char *)&DirectRecCount,sizeof(DirectRecCount));
            NoUpdateListFPtr.read((char *)&OvRecCount,sizeof(OvRecCount));

            //std::cout<<"UID="<<UID<<" DirectRecCount="<<DirectRecCount<<" OvRecCount="<<OvRecCount<<std::endl;

            /*  Process all non-overflow records which are not updated
                for the current user. 
            */
            //std::cout<<"Direct PhiRecIndex:";
            for(p=0;p<DirectRecCount;p++){
                if (RecCount==0){
                    NoUpdateListFPtr.close();
			        std::cout << "Error in ContactGraph::ClearContactGraph()-unable to access CVID" << std::endl;
			        exit(1);
                }
                NoUpdateListFPtr.read((char *)&PhiRecIndex,sizeof(PhiRecIndex));
                
                //std::cout<<PhiRecIndex<<" ";
                
                RecCount--;
                if (PHI->GetUpdateFlag(UID,false,PhiRecIndex))continue;

                if(!PHI->GetCVID(UID,false,PhiRecIndex,CVID)){
                    NoUpdateListFPtr.close();
			        std::cout << "Error in ContactGraph::ClearContactGraph()-unable to access CVID" << std::endl;
			        exit(1);
                }
                //std::cout<<"\nAccess the CV"<<std::endl;
                if (!TH->GetCV(CVID, CV)){
                    NoUpdateListFPtr.close();
		            std::cout << "Error in ContactGraph::ClearContactGraph():-CV inaccessible" << std::endl;
		            std::exit(1);
                }

                LCCFront = parCCFront;
                LCCRear = parCCRear;
                LDCFront = parDCFront;
                LDCRear = parDCRear;
                if(!(CV.SetCCQueueFront(LCCFront)&&CV.SetCCQueueRear(LCCRear)&&
                    CV.SetDCQueueFront(LDCFront)&&CV.SetDCQueueRear(LDCRear))){
                    NoUpdateListFPtr.close();
                    std::cout << "Error in ContactGraph::ClearContactGraph():-fail(s) in queue setting" << std::endl;
				    std::exit(1);
                }
  			    if(!CV.EnQueue(EdgeLabel)){
                    NoUpdateListFPtr.close();
    			    std::cout << "Error in ContactGraph::ClearContactGraph():-unable to enqueue" << std::endl;
				    std::exit(1);             
                }
                if (CV.IsQueueFull() && CV.IsFalse()){
                    if (!PHI->Delete(UID,false,PhiRecIndex)){
                        NoUpdateListFPtr.close();
    			        std::cout << "Error in ContactGraph::ClearContactGraph():-unable to delete empty CV"<<std::endl;
				        std::exit(1);             
                    }
                    if(!TH->FreeCV(CVID)){
                        NoUpdateListFPtr.close();
    			        std::cout << "Error in ContactGraph::ClearContactGraph():-unable to delete empty CV" << std::endl;
				        std::exit(1);             
                    }
                }
                else{
 			        if(!TH->SetCV(CVID, CV)){
                        NoUpdateListFPtr.close();
    			        std::cout << "Error in ContactGraph::ClearContactGraph():-CV("<<CVID<<") unable to save" << std::endl;
				        std::exit(1);             
                    }
                    if (!PHI->SetUpdateFlag(UID,false,PhiRecIndex)){
                        NoUpdateListFPtr.close();
    			        std::cout << "Error in ContactGraph::ClearContactGraph():-unsuccessful contacts upgrade" << std::endl;
				        std::exit(1);             
                    }
                }
            }
            //std::cout<<"\nOverflow PhiRecIndex:";
            for(p=0;p<OvRecCount;p++){
                if(RecCount==0){
                    NoUpdateListFPtr.close();
			        std::cout << "Error in ContactGraph::ClearContactGraph()-unable to access CVID" << std::endl;
			        exit(1);
                }
                NoUpdateListFPtr.read((char *)&PhiRecIndex,sizeof(PhiRecIndex));
                
                //std::cout<<PhiRecIndex<<" ";
                
                RecCount--;
                if (PHI->GetUpdateFlag(UID,true,PhiRecIndex))continue;

                if(!PHI->GetCVID(UID,true,PhiRecIndex,CVID)){
                    NoUpdateListFPtr.close();
			        std::cout << "Error in ContactGraph::ClearContactGraph()-unable to access CVID" << std::endl;
			        exit(1);
                }
                //std::cout<<"\nAccess the CV"<<std::endl;
                if (!TH->GetCV(CVID, CV)){
                    NoUpdateListFPtr.close();
		            std::cout << "Error in ContactGraph::ClearContactGraph():-CV inaccessible" << std::endl;
		            std::exit(1);
                }
                LCCFront = parCCFront;
                LCCRear = parCCRear;
                LDCFront = parDCFront;
                LDCRear = parDCRear;
                if(!(CV.SetCCQueueFront(LCCFront)&&CV.SetCCQueueRear(LCCRear)&&
                    CV.SetDCQueueFront(LDCFront)&&CV.SetDCQueueRear(LDCRear))){
                    NoUpdateListFPtr.close();
                    std::cout << "Error in ContactGraph::ClearContactGraph():-fail(s) in queue setting" << std::endl;
				    std::exit(1);
                }
  			    if(!CV.EnQueue(EdgeLabel)){
                    NoUpdateListFPtr.close();
    			    std::cout << "Error in ContactGraph::ClearContactGraph():-unable to enqueue" << std::endl;
				    std::exit(1);             
                }
                if (CV.IsQueueFull() && CV.IsFalse()){
                    if (!PHI->Delete(UID,true,PhiRecIndex)){
                        NoUpdateListFPtr.close();
    			        std::cout << "Error in ContactGraph::ClearContactGraph():-unable to delete empty CV"<<std::endl;
				        std::exit(1);             
                    }
                    if(!TH->FreeCV(CVID)){
                        NoUpdateListFPtr.close();
    			        std::cout << "Error in ContactGraph::ClearContactGraph():-unable to delete empty CV" << std::endl;
				        std::exit(1);             
                    }
                }
                else{
 			        if(!TH->SetCV(CVID, CV)){
                        NoUpdateListFPtr.close();
    			        std::cout << "Error in ContactGraph::ClearContactGraph():-CV("<<CVID<<") unable to save" << std::endl;
				        std::exit(1);             
                    }
                    if (!PHI->SetUpdateFlag(UID,true,PhiRecIndex)){
                        NoUpdateListFPtr.close();
    			        std::cout << "Error in ContactGraph::ClearContactGraph():-unsuccessful contacts upgrade" << std::endl;
				        std::exit(1);             
                    }
                }
            }
            if (RecCount==0)break;
            Pos += sizeof(UID)+sizeof(DirectRecCount)+sizeof(OvRecCount);
            Pos += (DirectRecCount+OvRecCount)*sizeof(PhiRecIndex);
        }
        NoUpdateListFPtr.close();
        parCCFront = CV.GetCCQueueFront();
        parCCRear = CV.GetCCQueueRear();
        parDCFront = CV.GetDCQueueFront();
        parDCRear = CV.GetDCQueueRear();
        return true;
    }
    catch (std::fstream::failure &e){
        std::cerr << "Phi::ClearContactGraph() - Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "ContactGraph::ClearContactGraph() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "ContactGraph::ClearContactGraph() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "ContactGraph::ClearContactGraph() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "ContactGraph::ClearContactGraph() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}


bool ContactGraph::BuildContactGraph(string parDataFName, int &parCCFront, int &parCCRear, 
    int &parDCFront, int &parDCRear){
	//New data will enter for the parT time duration. 
	//All bits for this time period will be uploaded after current parCCRear of the contact vector queue
	// 	   as well as day vector queue with entry point after parDRear.
	// 	   This will not change the front and rear indices of the both queues.
	//string ArtificialGraph = "Data\\GraphData.csv";
    //std::cout<<"\nBuilding of ContactGraph starts ....."<<std::endl;
	std::fstream GraphFPtr;
	GraphFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
	long N, UID1, UID2, UID;
    int Delta;
    long long FPos1;
    int LCCFront, LCCRear, LDCFront, LDCRear;
	char Delm = ',';
	std::string Line;
	std::vector<std::string> Token;

    try{
        if(!PHI->ResetAllUpdateFlags()){
			std::cout << "Error in ContactGraph::BuildContactGraph()-unable to reset all update flags" << std::endl;
            exit(1);
        }
		GraphFPtr.open(parDataFName, ios::in | ios::binary);
		if (GraphFPtr.fail()) throw parDataFName;  // the exception being checked
		N = TH->GetN();
        Delta = TH->GetDelta();
		//  int NoOfDays = TH->GetNoOfDays();
		int DayDuration = TH->GetDayDuration();
		int SlotDuration = TH->GetSlotDuration();
		if (SlotDuration == 0){
            GraphFPtr.close();
            throw std::overflow_error("Divide by zero exception");
        }
		int NoOfSlots = ceil((double)(DayDuration * 60.0) / SlotDuration);
        GraphFPtr.seekg(0,ios::end);
        FPos1 = GraphFPtr.tellg();
        GraphFPtr.seekg(0,ios::beg);
		if (!std::getline(GraphFPtr, Line)){
            GraphFPtr.close();
			std::cout << "Error in ContactGraph::BuildContactGraph() -" << parDataFName << " No data to read" << std::endl;
			return false;
		}
		Token = Split(Line, Delm);
		UID1 = stoll(Token[0], nullptr, 10);
		UID2 = stoll(Token[1], nullptr, 10);
		Omega Omg(UID1, NoOfSlots, N, Delta);
		//std::cout << "(" << UID1 << "," << UID2 << ")" << std::endl;
		std::vector<bool> EdgeLabel(NoOfSlots, false);
		static std::mt19937 generator;
		static std::uniform_int_distribution<int> distribution(0, 1);
		for (auto val : EdgeLabel) val = distribution(generator);
		Omg.CreateEdge(UID2, EdgeLabel);
        
		while (true){
            std::getline(GraphFPtr, Line);
			Token = Split(Line, Delm);
			UID = stoll(Token[0], nullptr, 10); 
			UID2 = stoll(Token[1], nullptr, 10);
			//std::cout << "(" << UID << "," << UID2 << ")" << std::endl;
			static std::mt19937 generator;
			static std::uniform_int_distribution<int> distribution(0, 1);
			for (auto val : EdgeLabel) val = distribution(generator);
			if (UID != UID1){
                //std::cout << "Omega(ready for process)=" << Omg;
                //std::cout<<"\nProcess Omega"<<std::endl;
                LCCFront = parCCFront;
                LCCRear = parCCRear;
                LDCFront = parDCFront;
                LDCRear = parDCRear;
				ProcessOmega(Omg, LCCFront, LCCRear, LDCFront, LDCRear);
				Omg.Reset(UID);
                UID1 = UID;
			}
			Omg.CreateEdge(UID2, EdgeLabel);
            if((FPos1 - GraphFPtr.tellg())<=1)break;
		}
        LCCFront = parCCFront;
        LCCRear = parCCRear;
        LDCFront = parDCFront;
        LDCRear = parDCRear;
 		ProcessOmega(Omg, LCCFront, LCCRear, LDCFront, LDCRear);
        Omg.Reset(UID);

        GraphFPtr.close();

        //std::cout<<"\nStarts Clear Contacts"<<std::endl;
        PHI->GetNoUpdateList();
        if(ClearContactGraph(parCCFront,parCCRear,parDCFront,parDCRear)){
            std::cout << "ContactGraph::BuildContactGraph()-all contacts are updated" << std::endl;
        }
        parCCFront = LCCFront;
        parCCRear = LCCRear;
        parDCFront = LDCFront;
        parDCRear = LDCRear;
		return true;
	}
	catch (std::fstream::failure &e){
		std::cerr << "ContactGraph::BuildContactGraph() opening/reading/closing file\n";
		std::exit(1);
	}
	catch (std::string e){
		std::cout << "ContactGraph::BuildContactGraph() " << e << " error in opening" << std::endl;
		std::exit(1);
	}
	catch (std::out_of_range& e){
		std::cout << "ContactGraph::BuildContactGraph() " << e.what() << " Not in range" << std::endl;
		std::exit(1);
	}
	catch (std::bad_alloc& e){
		std::cout << "ContactGraph::BuildContactGraph() " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "ContactGraph::BuildContactGraph() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}


void ContactGraph::ProcessOmega(Omega parOmega, int &parCCFront, int &parCCRear, int &parDCFront, 
        int &parDCRear){
	/* Omega is prepared for each individual
	* So (u,v) and (v,u) two times data will appear for processing
	* To minimize the number of such operation each record in Phi should be marked
	* so that if it is already processed then skip otherwise process it 
	*/
	long long CVID,CVID1,CVID2,PhiRecIndex1,PhiRecIndex2;
    long UID1, UID;
    bool OvFlag1,OvFlag2;
    bool Flag1,Flag2,Flag3,Flag4;
	try{
		int NoOfDay = TH->GetNoOfDays();
		int DayDuration = TH->GetDayDuration();
		int SlotDuration = TH->GetSlotDuration();
	
		if (SlotDuration == 0)
			throw std::overflow_error("Divide by zero exception");
		int NoOfSlots = ceil((double)(DayDuration * 60.0) / SlotDuration);
		UID1 = parOmega.GetUID();
  		std::vector<bool> EdgeLabel(NoOfSlots, false);
		ContactVector CV(NoOfDay, DayDuration, SlotDuration);
        if(!(CV.SetCCQueueFront(parCCFront)&&CV.SetCCQueueRear(parCCRear)&&
            CV.SetDCQueueFront(parDCFront)&&CV.SetDCQueueRear(parDCRear))){
            std::cout << "Error in ContactGraph::ProcessOmega():-fail(s) in queue setting" << std::endl;
			std::exit(1);
        }
		while (parOmega.RemoveEdge(UID, EdgeLabel)){
            //std::cout<<"\nInside Omega Processing loop"<<std::endl;
            Flag1 = PHI->GetCVID(UID1,UID,CVID1,OvFlag1,PhiRecIndex1);
            Flag2 = PHI->GetCVID(UID,UID1,CVID2,OvFlag2,PhiRecIndex2);
            if ((Flag1&&(!Flag2))||((!Flag1)&&Flag2)){
					std::cout << "Error in ContactGraph::ProcessOmega():-Both not exist" << std::endl;
					std::exit(1);
            }
            Flag3 = true;
            Flag4 = true;
            if (Flag1==false && Flag2==false){
                if (!TH->GetEmptyCV(CVID)){
					std::cout << "Error in ContactGraph::ProcessOmega():-Empty Contact Vector is not available" << std::endl;
					std::exit(1);
                }
                Flag3 = PHI->Insert(UID1,UID,CVID,OvFlag1,PhiRecIndex1);
                Flag4 = PHI->Insert(UID,UID1,CVID,OvFlag2,PhiRecIndex2);
                if (!(Flag3 && Flag4)){
                    std::cout << "Error in ContactGraph::ProcessOmega():-new contacts are not inserted" << std::endl;
				    std::exit(1);
                }
                if((OvFlag1&&OvFlag2)&&(PhiRecIndex1==PhiRecIndex2)){
                    std::cout << "Error in ContactGraph::ProcessOmega():-Both point to same overflow record" << std::endl;
					std::exit(1);
                }
            }
            if (Flag1&&Flag2){
                std::cout<<"CVID1="<<CVID1<<" CVID2="<<CVID2<<std::endl;
                if (CVID1 != CVID2){
					std::cout << "Error in ContactGraph::ProcessOmega():-Different CVID" << std::endl;
					std::exit(1);
                }
                if((OvFlag1&&OvFlag2)&&(PhiRecIndex1==PhiRecIndex2)){
                    std::cout << "Error in ContactGraph::ProcessOmega():-Both point to same overflow record" << std::endl;
					std::exit(1);
                }
                CVID = CVID1;
                //std::cout<<"\nContactGraph::ProcessOmega():-Existing Contact"<<std::endl;
                //std::cout<<"\nContactGraph::ProcessOmega():-CVID="<<CVID<<"("<<UID1<<","<<UID<<")"<<std::endl;
            }
            //std::cout<<"\nAccess the CV"<<std::endl;
            if (!TH->GetCV(CVID, CV)){
				std::cout << "Error in ContactGraph::ProcessOmega():-CV("<<CVID<<") is inaccessible" << std::endl;
				std::exit(1);
            }
            //std::cout<<"\nCV("<<CVID<<") has been opened"<<std::endl;

            if(!(CV.SetCCQueueFront(parCCFront)&&CV.SetCCQueueRear(parCCRear)&&
                CV.SetDCQueueFront(parDCFront)&&CV.SetDCQueueRear(parDCRear))){
                std::cout << "Error in ContactGraph::ProcessOmega():-fail(s) in queue setting" << std::endl;
				std::exit(1);
            }
  			if(!CV.EnQueue(EdgeLabel)){
    			std::cout << "Error in ContactGraph::ProcessOmega():-unable to enqueue" << std::endl;
				std::exit(1);             
            }
 			if(!TH->SetCV(CVID, CV)){
    			std::cout << "Error in ContactGraph::ProcessOmega():-CV("<<CVID<<") unable to save" << std::endl;
				std::exit(1);             
            }
            Flag1 = PHI->SetUpdateFlag(UID1,UID,OvFlag1,PhiRecIndex1);
            Flag2 = PHI->SetUpdateFlag(UID,UID1,OvFlag2,PhiRecIndex2);
            if (!(Flag1&&Flag2)){
    			std::cout << "Error in ContactGraph::ProcessOmega():-unsuccessful contacts upgrade" << std::endl;
				std::exit(1);             
            }
			//std::cout << "Omega=" << parOmega;
		}
        parCCFront = CV.GetCCQueueFront();
        parCCRear = CV.GetCCQueueRear();
        parDCFront = CV.GetDCQueueFront();
        parDCRear = CV.GetDCQueueRear();
    
        //std::cout << "Theta=" << *TH;
	    //std::cout << "Phi=" << *PHI;
        return;
	}
	catch (const std::bad_alloc& e){
		std::cout << "ContactGraph::ProcessOmega() " << e.what() << " Exception occu" << std::endl;
		std::exit(1);
	}
	catch (std::out_of_range& e){
		std::cout << "ContactGraph::ProcessOmega() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
	catch (std::overflow_error& e){
		std::cout << "ContactGraph::ProcessOmega() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "ContactGraph::ProcessOmega() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

bool ContactGraph::Add(string parDataFName, int &parCCFront, int &parCCRear, 
    int &parDCFront, int &parDCRear, long double &parExecTime){
        /*
            For the simulation this function take a undirected graph represented 
            with the help of set of edge list stored in a CSV file as an input. 
            As a preprocessing for simulation an artificial close contacts for a 
            day long duration is randomly created and stored in a binary vector. 
            This binary contacts is strored in this input data file for each edge.
            This is simulating the Omega.
            Now this preprocessed simulation Omega is sequentially fetched and uploaded 
            into the Phi and Theta so that we can form the ContactGraph structure. 
        */

	std::fstream OmgFPtr;
	OmgFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
	long long RecCount=0;

    std::string SimulationResultFName;
    #ifdef _WIN32
        SimulationResultFName = "C:\\DCTSystem\\Data\\Output\\Results.txt";
    #elif defined __linux__
        SimulationResultFName = "/home/gautam/DCTSystem/Output/Results.txt";
    #endif

    std::fstream SimulationResultFPtr;
    SimulationResultFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

    try {
        SimulationResultFPtr.open(SimulationResultFName,ios::app);
        if (SimulationResultFPtr.fail()) throw SimulationResultFName;  // the exception being checked            
    }
    catch (std::ofstream::failure &e){
        std::cerr << "ContactGraph::Add() - opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::exception& e){
		std::cout << "ContactGraph::Add() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
    catch (std::string e){
        std::cout << "ContactGraph::Add() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }

    long long CVID,CVID1,CVID2,PhiRecIndex1,PhiRecIndex2;
    long UID1,UID2;
    bool OvFlag1,OvFlag2;
    bool Flag1,Flag2,Flag3,Flag4;
    long N;
    char OmegaDataFName[50];
    int Delta;
	try{
        N = TH->GetN();
        Delta = TH->GetDelta();
		int NoOfDays = TH->GetNoOfDays();
		int DayDuration = TH->GetDayDuration();
		int SlotDuration = TH->GetSlotDuration();
		if (SlotDuration == 0){
            throw std::overflow_error("Divide by zero exception");
        }
		int NoOfSlots = ceil((double)(DayDuration * 60.0) / SlotDuration);
        
        auto begin = std::chrono::high_resolution_clock::now();
        Omega Omg(UID1,NoOfSlots,N,Delta,parDataFName);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        parExecTime = elapsed.count() * 1e-9;
        std::cout << "Time measured (Omega Processing):" << parExecTime << "seconds."<<std::endl;
        SimulationResultFPtr<<parExecTime<<std::endl;
        SimulationResultFPtr.close();
        
        if(!Omg.GetOmegaFName(OmegaDataFName)){
            std::cout<<"Error:ContactGraph::Add()- Empty Omega Data File"<<std::endl;
            return false;
        }
		OmgFPtr.open(OmegaDataFName, ios::in | ios::binary);
		if (OmgFPtr.fail()) throw OmegaDataFName;  // the exception being checked
		OmgFPtr.read((char *)&RecCount, sizeof(RecCount));
		
  		std::vector<bool> EdgeLabel(NoOfSlots, false);
		ContactVector CV(NoOfDays, DayDuration, SlotDuration);

        if(!(CV.SetCCQueueFront(parCCFront)&&CV.SetCCQueueRear(parCCRear)&&
            CV.SetDCQueueFront(parDCFront)&&CV.SetDCQueueRear(parDCRear))){
            std::cout << "Error in ContactGraph::Add():-fail(s) in queue setting" << std::endl;
			std::exit(1);
        }
        int Len = (int)ceil(NoOfSlots / 8.0);
        char* Str = new char[Len + 1];

		for(long long i=0;i<RecCount;i++){
		    OmgFPtr.read((char *)&UID1, sizeof(UID1));
		    OmgFPtr.read((char *)&UID2, sizeof(UID2));
		    OmgFPtr.read((char *)Str, Len);

            Flag1 = PHI->GetCVID(UID1,UID2,CVID1,OvFlag1,PhiRecIndex1);
            Flag2 = PHI->GetCVID(UID2,UID1,CVID2,OvFlag2,PhiRecIndex2);
            if ((Flag1&&(!Flag2))||((!Flag1)&&Flag2)){
					std::cout << "Error in ContactGraph::Add():-Both not exist" << std::endl;
					std::exit(1);
            }
            Flag3 = true;
            Flag4 = true;
            if (Flag1==false && Flag2==false){
                if (!TH->GetEmptyCV(CVID)){
					std::cout << "Error in ContactGraph::Add():-Empty Contact Vector is not available" << std::endl;
					std::exit(1);
                }
                Flag3 = PHI->Insert(UID1,UID2,CVID,OvFlag1,PhiRecIndex1);
                Flag4 = PHI->Insert(UID2,UID1,CVID,OvFlag2,PhiRecIndex2);
                if (!(Flag3 && Flag4)){
                    std::cout << "Error in ContactGraph::Add():-new contacts are not inserted" << std::endl;
				    std::exit(1);
                }
                if((OvFlag1&&OvFlag2)&&(PhiRecIndex1==PhiRecIndex2)){
                    std::cout << "Error in ContactGraph::Add():-Both point to same overflow record" << std::endl;
					std::exit(1);
                }
            }
            if (Flag1&&Flag2){
                std::cout<<"CVID1="<<CVID1<<" CVID2="<<CVID2<<std::endl;
                if (CVID1 != CVID2){
					std::cout << "Error in ContactGraph::Add():-Different CVID" << std::endl;
					std::exit(1);
                }
                if((OvFlag1&&OvFlag2)&&(PhiRecIndex1==PhiRecIndex2)){
                    std::cout << "Error in ContactGraph::Add():-Both point to same overflow record" << std::endl;
					std::exit(1);
                }
                CVID = CVID1;
                //std::cout<<"\nContactGraph::Add():-Existing Contact"<<std::endl;
                //std::cout<<"\nContactGraph::Add():-CVID="<<CVID<<"("<<UID1<<","<<UID<<")"<<std::endl;
            }
            //std::cout<<"\nAccess the CV"<<std::endl;
            if (!TH->GetCV(CVID, CV)){
				std::cout << "Error in ContactGraph::Add():-CV("<<CVID<<") is inaccessible" << std::endl;
				std::exit(1);
            }
            //std::cout<<"\nCV("<<CVID<<") has been opened"<<std::endl;

            if(!(CV.SetCCQueueFront(parCCFront)&&CV.SetCCQueueRear(parCCRear)&&
                CV.SetDCQueueFront(parDCFront)&&CV.SetDCQueueRear(parDCRear))){
                std::cout << "Error in ContactGraph::Add():-fail(s) in queue setting" << std::endl;
				std::exit(1);
            }
  			if(!CV.EnQueue(EdgeLabel)){
    			std::cout << "Error in ContactGraph::Add():-unable to enqueue" << std::endl;
				std::exit(1);             
            }
 			if(!TH->SetCV(CVID, CV)){
    			std::cout << "Error in ContactGraph::Add():-CV("<<CVID<<") unable to save" << std::endl;
				std::exit(1);             
            }
            Flag1 = PHI->SetUpdateFlag(UID1,UID2,OvFlag1,PhiRecIndex1);
            Flag2 = PHI->SetUpdateFlag(UID2,UID1,OvFlag2,PhiRecIndex2);
            if (!(Flag1&&Flag2)){
    			std::cout << "Error in ContactGraph::Add():-unsuccessful contacts upgrade" << std::endl;
				std::exit(1);             
            }
        }
        OmgFPtr.close();
        parCCFront = CV.GetCCQueueFront();
        parCCRear = CV.GetCCQueueRear();
        parDCFront = CV.GetDCQueueFront();
        parDCRear = CV.GetDCQueueRear();
        return true;
	}
	catch (const std::bad_alloc& e){
		std::cout << "ContactGraph::Add() " << e.what() << " Exception occu" << std::endl;
		std::exit(1);
	}
	catch (std::out_of_range& e){
		std::cout << "ContactGraph::Add() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
	catch (std::overflow_error& e){
		std::cout << "ContactGraph::Add() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "ContactGraph::Add() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

//************************************ End of Contact Graph Library **********************