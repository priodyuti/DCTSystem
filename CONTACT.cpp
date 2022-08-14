#include "CONTACT.h"

using namespace std;

//****************************************************************************************
//******************************** Library for CONTACT ****************************
//****************************************************************************************

ostream& operator<<(ostream& out, CONTACT& parOut){
    out << "UID=" << parOut.UID << "CVID=" << parOut.CVID << endl;
    return out;
}

int CONTACT:: Size(){
    return sizeof(CONTACT);
}
//******************************** End of CONTACT Library *********************************
