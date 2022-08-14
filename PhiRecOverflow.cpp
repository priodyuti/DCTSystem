#include "PhiRecOverflow.h"

using namespace std;

//****************************************************************************************
//*********************************** PhiRecOverflow Library **************************************
//****************************************************************************************

PhiRecOverflow::PhiRecOverflow(){
    CVID = 0L;
    CVID = CVID | 0x80000000;
    DelFlag = true; //all newly created contact is having deleted status true.
    UpdateFlag = false;
    NeighborID = -1L;
    HostID = -1L;
}

PhiRecOverflow::PhiRecOverflow(long long parCVID,long parHostID,long parNeighborID){
    NeighborID = parNeighborID;
    HostID = parHostID;
    CVID = parCVID;
    DelFlag = false;
    UpdateFlag = false;
}

ostream& operator<<(ostream& out, const PhiRecOverflow& parPhiRecOverflow){
    out << "(";
    out << parPhiRecOverflow.HostID<<","<<parPhiRecOverflow.NeighborID;
    out << ")(";
    out << parPhiRecOverflow.CVID;
    out << ")(";
    out << parPhiRecOverflow.DelFlag;
    out << ")(";
    out << parPhiRecOverflow.UpdateFlag;
    out << ")(";
    out << parPhiRecOverflow.BLink;
    out << ")(";
    out << parPhiRecOverflow.FLink;
    out << ")";
    return out;
}

//Size is the multiple of largest data type among the fields.
int PhiRecOverflow::Size(){
    return sizeof(PhiRecOverflow);
}

long PhiRecOverflow::GetNeighborID(){
    return NeighborID;
}

long PhiRecOverflow::GetHostID(){
    return HostID;
}

long long PhiRecOverflow::GetCVID(){
    return CVID;
}
bool PhiRecOverflow::GetDelFlag(){
    return DelFlag;
}

bool PhiRecOverflow::GetUpdateFlag(){
    return UpdateFlag;
}

void PhiRecOverflow::SetHostID(long parHostID){
    HostID = parHostID;
    return;
}

void PhiRecOverflow::SetNeighborID(long parNeighborID){
    NeighborID = parNeighborID;
    return;
}

void PhiRecOverflow::SetCVID(long long parCVID){
    CVID = parCVID;
    return;
}
void PhiRecOverflow::SetDelFlag(bool parDelFlag){
    DelFlag = parDelFlag;
    return;
}
void PhiRecOverflow::SetUpdateFlag(bool parUpdateFlag){
    UpdateFlag = parUpdateFlag;
    return;
}

long long PhiRecOverflow::GetNext(){
    return FLink;
}

bool PhiRecOverflow:: SetNext(long long parFLink){
    if (parFLink < -1L) return false;
    FLink = parFLink;
    return true;
}

long long PhiRecOverflow::GetPrevious(){
    return BLink;
}

bool PhiRecOverflow:: SetPrevious(long long parBLink){
    if (parBLink < -1L) return false;
    BLink = parBLink;
    return true;
}
//************************************ End of PhiRecOverflow Library ********************
