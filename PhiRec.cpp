#include "PhiRec.h"

using namespace std;

//****************************************************************************************
//*********************************** PhiRec Library *************************************
//****************************************************************************************
PhiRec::PhiRec(){
    CVID = 0L;
    CVID = CVID | 0x80000000;
    DelFlag = true; //all newly created contact is having deleted status true.
    UpdateFlag = false;
    NeighborID = -1L;
}

PhiRec::PhiRec(long long parCVID, long parNeighborID){
    NeighborID = parNeighborID;
    CVID = parCVID;
    DelFlag = false;
    UpdateFlag = false;
}

ostream& operator<<(ostream& out, const PhiRec& parPhiRec){
    out << "(";
    out << parPhiRec.NeighborID;
    out << ")(";
    out << parPhiRec.CVID;
    out << ")(";
    out << parPhiRec.DelFlag;
    out << ")(";
    out << parPhiRec.UpdateFlag;
    out << ")";
    return out;
}

//Size is the multiple of largest data type among the fields.
int PhiRec::Size(){
    return sizeof(PhiRec);
}

long PhiRec::GetNeighborID(){
    return NeighborID;
}
long long PhiRec::GetCVID(){
    return CVID;
}
bool PhiRec::GetDelFlag(){
    return DelFlag;
}

bool PhiRec::GetUpdateFlag(){
    return UpdateFlag;
}

void PhiRec::SetNeighborID(long parNeighborID){
    NeighborID = parNeighborID;
    return;
}
void PhiRec::SetCVID(long long parCVID){
    CVID = parCVID;
    return;
}

void PhiRec::SetDelFlag(bool parDelFlag){
    DelFlag = parDelFlag;
    return;
}
void PhiRec::SetUpdateFlag(bool parUpdateFlag){
    UpdateFlag = parUpdateFlag;
    return;
}
//************************************ End of PhiRec Library *************************************
