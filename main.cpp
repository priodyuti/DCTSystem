#include<iostream>
#include<chrono>
#include<stdio.h>
#include<cstdio>

//Author: Gautam Mahapatra

//#include"MyLib.h"

#include "DCTSystem.h"


using namespace std;
 

void GetParameters(int &parID,long &parN,long &parq, int &parNoOfDays,
    int &parSlotDuration,int &parDelta,int &parDayDuration, float parf,
    Date &parStartDate, Date &parCurrentDate){
    std::cout << "Enter unique identity (ID, an   integer):";
    std::cin >> parID;
    std::cout << "Enter Population Size(N):";
    std::cin >> parN;
    std::cout << "Enter contacts(q):";
    std::cin >> parq;
    std::cout << "Enter factor(f):";
    std::cin >> parf;
    std::cout << "Enter Number of Days(D, in days):";
    std::cin >> parNoOfDays;
    std::cout << "Enter Day Duration(T, in Hours):";
    std::cin >> parDayDuration;
    std::cout << "Enter minimum duration (d, in min):";
    std::cin >> parSlotDuration;
    std::cout << "Enter sample interval size(Delta, in min):";
    std::cin >> parDelta;
    std::cout << "Enter System Start Date:";
    std::cin >> parStartDate;
    std::cout << "Enter System Current Date:";
    std::cin >> parCurrentDate;
}

void TestContactVector(int parNoOfDays, int parDayDuration,
    int parSlotDuration, int parCCFront, int parCCRear, int parDCFront, int parDCRear){
    ContactVector C(parNoOfDays, parDayDuration,parSlotDuration);
    std::cout<<"C="<<C<<std::endl;
    C.SetCCQueueFront(parCCFront);
    C.SetCCQueueRear(parCCRear);
    C.SetDCQueueFront(parDCFront);
    C.SetDCQueueRear(parDCRear);
}

void TestTheta(long parN,long parq,int parNoOfDays,int parDayDuration,
    int parSlotDuration,int parDelta, float parf){
    Theta TH(parN, parq, parNoOfDays, parDayDuration, parSlotDuration, parDelta, parf);
    std::cout << "TH=" << TH << std::endl;
    system("pause");
}

void SimulateDCT(){
    //Default values
    int ID = 1;    //Each DCT System has unique ID
    long N = 10; //Total population smart-phone users // 4 bytes
    long q = 3;       //Average number individuals may come in close contacts during last D days of observation period.
    int D = 14;     //Number of days under observation
    int d = 15;     //SlotDuration - Minimum Time Duration for a close contact
    int Delta = 3;  //Sampling Time Quanta
    
    float f = 1.0;
    int T = 24; //DayDuration - Interval for contact data upload
    Date StartDate(01, 01, 2022), CurrentDate(03, 01, 2022);

    DCTSystem *DCT;

    //int CCFront = -1, CCRear = -1, DCFront = -1, DCRear = -1;
    //std::cout << "This simulate the DSTSystem" << std::endl;
    std::string SamplesFolderName;
    #ifdef _WIN32
        SamplesFolderName = "C:\\DCTSystem\\Data\\Samples\\";
    #elif defined __linux__
        SamplesFolderName = "/home/gautam/DCTSystem/Samples/";
    #endif

    std::string SimulationResultFName;
    #ifdef _WIN32
        SimulationResultFName = "C:\\DCTSystem\\Data\\Output\\Results.txt";
    #elif defined __linux__
        SimulationResultFName = "/home/gautam/DCTSystem/Output/Results.txt";
    #endif

    std::ofstream SimulationResultFPtr;
    SimulationResultFPtr.exceptions(std::ofstream::failbit | std::ofstream::badbit);

    try {
        SimulationResultFPtr.open(SimulationResultFName);
        if (SimulationResultFPtr.fail()) throw SimulationResultFName;  // the exception being checked
            
        SimulationResultFPtr << "DCTSystem Simulation Results";
        SimulationResultFPtr.close();
    }
    catch (std::ofstream::failure &e){
        std::cerr << "SimulateDCT() - opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::exception& e){
		std::cout << "SimulateDCT() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
    catch (std::string e){
        std::cout << "SimulateDCT() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    
    regex reg("Net");
    std::list<std::string> DirectoryList, Temp;
    //DIR* rep = opendir(".");
    DIR* rep = opendir(SamplesFolderName.c_str());
    if ( rep != NULL )
    {
        struct dirent* ent;
        while ( (ent = readdir(rep) ) != NULL )
        {
            cout << ent->d_name << endl;
            if (regex_search(ent->d_name,reg))
                DirectoryList.push_back(std::string(ent->d_name));
        }
        closedir(rep);
    }
    //std::cout<<"Directory Content (filtered):";
    for(auto x: DirectoryList)
        std::cout<<x.c_str()<<std::endl;
    long double ExecTime;
    long NetSize,Degree;
    std::vector<std::string> Vals;
    FieldSort(DirectoryList);
    std::string WorkingFolderName;
    std::list<std::string> SampleFileList;
    std::string DataFileName;
    for(auto x : DirectoryList){
        WorkingFolderName = SamplesFolderName;
        std::cout<<x.c_str()<<std::endl;
        Vals = Split(x,'_');
        NetSize = stoll(Vals[1].c_str(),nullptr,10);
        Degree = stoll(Vals[2].c_str(),nullptr,10);
        std::cout<<"NetSize="<<NetSize<<" Degree="<<Degree<<std::endl;
        //std::cout<<"\nPress any key to continue...";
        //std::cin.ignore();
        N = NetSize;
        q = Degree;
        //Create the DCTSystem for this network
        DCT = NULL;
        auto begin = std::chrono::high_resolution_clock::now();
        DCT = new DCTSystem(ID, N, q, D, d, Delta, T, f, StartDate, CurrentDate);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        ExecTime = elapsed.count() * 1e-9;
        std::cout << "Time measured:" << ExecTime << "seconds."<<std::endl;
        try {
            
            SimulationResultFPtr.open(SimulationResultFName, ios::app);
            if (SimulationResultFPtr.fail()) throw SimulationResultFName;  // the exception being checked
            
            SimulationResultFPtr << "ID, N, q, D, d, Delta, T, f,System Creation time(in Sec)"<<std::endl;
            SimulationResultFPtr <<ID<<","<<N<<","<<q<<","<<D<<","<<d<<","<<Delta<<",";
            SimulationResultFPtr <<T<<","<<f<<","<<ExecTime<<std::endl;
            SimulationResultFPtr.close();
        }
        catch (std::ofstream::failure &e){
            std::cerr << "SimulateDCT() - opening/reading/closing file\n";
            std::exit(1);
        }
        catch (std::exception& e){
		    std::cout << "SimulateDCT() " << e.what() << "Exception occur" << std::endl;
		    std::exit(1);
	    }
        catch (std::string e){
            std::cout << "SimulateDCT() " << e << " error in opening/creating" << std::endl;
            std::exit(1);
        }
        
        //std::cout<<"DCTSystem:"<<std::endl;
        //DCT->Show();
        //std::cout<<"DCTSystem Simulation Starts:"<<std::endl;
        WorkingFolderName += x;
        std::cout<<"Working Folder="<<WorkingFolderName<<std::endl;
        DCT->SimulateV1(WorkingFolderName);
    }
        
        /*
        rep = opendir(WorkingFolderName.c_str());
        SampleFileList.clear();
        if ( rep != NULL )
        {
            struct dirent* ent;
            while ( (ent = readdir(rep) ) != NULL )
            {
                //cout << ent->d_name << endl;
                if(regex_search(ent->d_name,reg))
                    SampleFileList.push_back(std::string(ent->d_name));
            }
            closedir(rep);
        }
        FieldSort(SampleFileList);
        for(auto f: SampleFileList){
            //std::cout<<f.c_str()<<std::endl;
            DataFileName = WorkingFolderName;
            #ifdef _WIN32
                DataFileName += "\\"; 
            #elif defined __linux__
                DataFileName +="/" ;
            #endif
            DataFileName +=f;
            std::cout<<DataFileName.c_str()<<std::endl;
        }
        */
}

void TestDCTSystem(){
    int ID = 1;    //Each DCT System has unique ID
    long N = 10; //Total population smart-phone users // 4 bytes
    long q = 3;       //Average number individuals may come in close contacts during last D days of observation period.
    int D = 14;     //Number of days under observation
    int d = 15;     //SlotDuration - Minimum Time Duration for a close contact
    int Delta = 3;  //Sampling Time Quanta
    float f = 1.0;
    int T = 24; //DayDuration - Interval for contact data upload
    Date StartDate(01, 01, 2022), CurrentDate(03, 01, 2022);
    
    DCTSystem *DCT;
    
    char Resp;
    std::cout << "\nDCTSystem already exists(Y/N)?";
    std::cin >> Resp;
    if (Resp == 'N' || Resp == 'n'){
        std::cout << "\nDo you use default system parameters(Y/N)?";
        std::cin >> Resp;
        if (Resp == 'N' || Resp == 'n')
            GetParameters(ID,N,q,D,d,Delta,T,f,StartDate,CurrentDate);
        DCT = new DCTSystem(ID, N, q, D, d, Delta, T, f, StartDate, CurrentDate);
    }
    else
        DCT = new DCTSystem();
    
    std::cout<<"DCTSystem:"<<std::endl;
    DCT->Show();
    //std::cout<<*DCT;
    std::cout<<"DCTSystem Simulation Starts:"<<std::endl;
    DCT->Simulate();
}

int main(){
    // Start measuring time
    auto begin = std::chrono::high_resolution_clock::now();
    std::cout << "SystemO2-A Digital Contract Tracing System" << std::endl;
    //std::cout<<"\nTest of Contact Vector Object"<<std::endl;
    //TestContactVector(D,T,d,CCFront,CCRear,DCFront,DCRear);
    //std::cout<<"\nTest of Theta Object"<<std::endl;
    //TestTheta(N, q, D, T, d, Delta);
    //std::cout<<"End of Tests"<<std::endl;
    std::cout<<"Start:DCTSystem Test"<<std::endl;
    //TestDCTSystem();
    SimulateDCT();
    std::cout<<"End:DCTSystem Test"<<std::endl;
    // Stop measuring time and calculate the elapsed time
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    std::cout << "Time measured:" << elapsed.count() * 1e-9 << "seconds."<<std::endl;
}
