#include "DCTSystem.h"

using namespace std;


//****************************************************************************************
//*********************************** DCTSystem Library ******************************
//****************************************************************************************


DCTSystem::DCTSystem(){
    std::cout << "\nCreating DCTSystem object[DCTSystem::DCTSystem()]" << std::endl;
    std::fstream DCTSystemConfigFPtr;
    DCTSystemConfigFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    try {
        DCTSystemConfigFPtr.open(SysConfigFName, ios::in | ios::binary);
        if (DCTSystemConfigFPtr.fail()) throw SysConfigFName;  // the exception being checked

        DCTSystemConfigFPtr.read((char*)this, sizeof(DCTSystem));
        DCTSystemConfigFPtr.close();
        std::cout << "Current System Configuration:" << endl;
        if (Delta == 0)
            throw std::overflow_error("Divide by zero exception");

        Tau = ceil((double)d / Delta);
        CG = new ContactGraph(); //Create contact graph from stored data
        Show();
        std::cout << "\nExisting DCTSystem created successfully" << std::endl;
        return;
    }
    catch (std::fstream::failure &e){
        std::cerr << "DCTSystem::DCTSystem() -Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "DCTSystem::DCTSystem() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "DCTSystem::DCTSystem() " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "DCTSystem::DCTSystem() " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "DCTSystem::DCTSystem() " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

DCTSystem::DCTSystem(int parID,long parN,long parq,int parNoOfDays,
    int parSlotDuration,int parDelta,int parDayDuration, float parf,
    Date parStartDate, Date parCurrentDate){
    //std::cout << "\nCreating DCTSystem object[DCTSystem::DCTSystem(ID, N, q, D, d, Delta, T, StartDate, CurrentDate)]...." << std::endl;
    
    if (CheckExistingDCTSystem()){
        std::cout << "System / Folder structure exists" << std::endl;
        if (!RemoveExistingDCTSystem()){
            std::cout << "Unable to remove the structure" << std::endl;
            std::exit(1);
        }
    }

    if(!CreateNewDCTSystemStructure()){
        std::cout << "Unable to remove the structure" << std::endl;
        std::exit(1);
    }
    ID = parID;
    N = parN;
    q = parq;
    D = parNoOfDays;
    d = parSlotDuration;
    Delta = parDelta;
    T = parDayDuration; //DayDuration - Interval for contact data upload
    f = parf;
    StartDate = parStartDate;
    CurrentDate = parCurrentDate;
    std::fstream DCTSystemConfigFPtr;
    DCTSystemConfigFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
    try {
        if (Delta == 0)
            throw std::overflow_error("Divide by zero exception");

        Tau = ceil((double)d / Delta);          //Sliding Window size

        DCTSystemConfigFPtr.open(SysConfigFName, ios::out | ios::binary);
        if (DCTSystemConfigFPtr.fail()) throw SysConfigFName;  // the exception being checked

        DCTSystemConfigFPtr.write((char*)this, sizeof(DCTSystem));
        DCTSystemConfigFPtr.close();

        CG = new ContactGraph(N, q, D, T, d, Delta,f); //Create an empty graph structure
    }
    catch (std::fstream::failure &e){
        std::cerr << "DCTSystem::DCTSystem(parID,parN,...) -Exception opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::string e){
        std::cout << "DCTSystem::DCTSystem(parID,parN,...) " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }
    catch (std::out_of_range& e){
        std::cout << "DCTSystem::DCTSystem(parID,parN,...) " << e.what() << " Not in range" << std::endl;
        std::exit(1);
    }
    catch (std::bad_alloc& e){
        std::cout << "DCTSystem::DCTSystem(parID,parN,...) " << e.what() << " bad allocation" << std::endl;
        std::exit(1);
    }
    catch (std::exception& e){
        std::cout << "DCTSystem::DCTSystem(parID,parN,...) " << e.what() << "Exception occur" << std::endl;
        std::exit(1);
    }
}

void DCTSystem::Show(){ 
    std::cout << "DCTSystem Descriptions:" << std::endl;
    std::cout << "ID: " << ID << std::endl;
    std::cout << "N = " << N << std::endl;
    std::cout << "D = " << D << std::endl;
    std::cout << "d = " << d << std::endl;
    std::cout << "q = " << q << std::endl;
    std::cout << "Delta = " << Delta << std::endl;
    std::cout << "f = " << f << std::endl;
    std::cout << "Tau = " << Tau << std::endl;
    std::cout << *CG <<std::endl;
    long long CalSize = 0, ObsrvSize = 0;
    CG->StorageSize(CalSize,ObsrvSize);
    std::cout << "Total Disk Storage Requirement:" << std::endl;
    std::cout << "Calculated="<<CalSize<<" Observed="<<ObsrvSize<<std::endl;
}

void DCTSystem::SimulateV1(std::string parWorkingFolderName){
    int CCFront = -1, CCRear = -1, DCFront = -1, DCRear = -1;
    //std::cout << "This simulate the DSTSystem" << std::endl;
    std::string DataFileName;
    #ifdef _WIN32
        DataFileName = "C:\\DCTSystem\\Data\\Graph.csv";
    #elif defined __linux__
        DataFileName = "/home/gautam/DCTSystem/Samples/Graph.csv";
    #endif

    std::string SimulationResultFName;
    #ifdef _WIN32
        SimulationResultFName = "C:\\DCTSystem\\Data\\Output\\Results.txt";
    #elif defined __linux__
        SimulationResultFName = "/home/gautam/DCTSystem/Output/Results.txt";
    #endif

    std::fstream SimulationResultFPtr;
    SimulationResultFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

    regex reg("Net");
    DIR* rep=NULL;
    std::list<std::string> SampleFileList;
    rep = opendir(parWorkingFolderName.c_str());
    SampleFileList.clear();
    if ( rep != NULL ){
        struct dirent* ent;
        while ( (ent = readdir(rep) ) != NULL ){
            if(regex_search(ent->d_name,reg))
                SampleFileList.push_back(std::string(ent->d_name));
        }
        closedir(rep);
    }
    try {
        SimulationResultFPtr.open(SimulationResultFName,ios::app);
        if (SimulationResultFPtr.fail()) throw SimulationResultFName;  // the exception being checked
            
        SimulationResultFPtr << "Day NO,"<<"Simulation Data File Name,";
        SimulationResultFPtr <<"PhiCalSize,PhiObsrvSize,ThetaCalSize,ThetaObsrvSize,";
        SimulationResultFPtr <<"Omega Processing Time(in Sec),Day Execution Time(in sec)"<<std::endl;
    }
    catch (std::ofstream::failure &e){
        std::cerr << "DCTSystem::SimulateV1() - opening/reading/closing file\n";
        std::exit(1);
    }
    catch (std::exception& e){
		std::cout << "DCTSystem::SimulateV1() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
    catch (std::string e){
        std::cout << "DCTSystem::SimulateV1() " << e << " error in opening/creating" << std::endl;
        std::exit(1);
    }

    int i=0;
    long double ExecTime,OmegaProcessTime;
    int NoOfObservationDays, NoOfCloseContactsDuringADay;
    int NoOfCloseContactSlots;
    NoOfObservationDays = CG->GetNoOfObservationDays();
    NoOfCloseContactsDuringADay = CG->GetNoOfCloseContactsDuringADay();
    NoOfCloseContactSlots = CG->GetNoOfCloseContactSlots();
    FieldSort(SampleFileList);
    long long PhiCalSize, PhiObsrvSize;
    long long ThetaCalSize, ThetaObsrvSize;
    
    for(auto f: SampleFileList){
            auto begin = std::chrono::high_resolution_clock::now();
            DataFileName = parWorkingFolderName;
            #ifdef _WIN32
                DataFileName += "\\"; 
            #elif defined __linux__
                DataFileName +="/" ;
            #endif
            DataFileName +=f;
            std::cout<<DataFileName.c_str()<<std::endl;

            if (i>=NoOfObservationDays){
                CCFront = (CCFront + NoOfCloseContactsDuringADay) % NoOfCloseContactSlots;
                DCFront = (DCFront + 1) % NoOfObservationDays; 
            }
            //CG->BuildContactGraph(DataFileName,CCFront,CCRear,DCFront,DCRear);
            CG->Add(DataFileName,CCFront,CCRear,DCFront,DCRear,OmegaProcessTime);
            CG->StorageSize(PhiCalSize,PhiObsrvSize,ThetaCalSize,ThetaObsrvSize);
            std::cout<<"PhiCalSize="<<PhiCalSize<<" PhiObsrvSize="<<PhiObsrvSize<<std::endl;
            std::cout<<"ThetaCalSize="<<ThetaCalSize<<" ThetaObsrvSize="<<ThetaObsrvSize<<std::endl;
            i++;
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
            ExecTime = elapsed.count() * 1e-9;
            std::cout << "Time measured:" << ExecTime << "seconds."<<std::endl;
            try {            
                SimulationResultFPtr <<i<<","<<DataFileName<<",";
                SimulationResultFPtr <<PhiCalSize<<","<<PhiObsrvSize<<",";
                SimulationResultFPtr <<ThetaCalSize<<","<<ThetaObsrvSize<<",";
                SimulationResultFPtr<<OmegaProcessTime<<",";
                SimulationResultFPtr<<ExecTime<<std::endl;
            }
            catch (std::ofstream::failure &e){
                std::cerr << "DCTSystem::SimulateV1() - opening/reading/closing file\n";
                std::exit(1);
            }
            catch (std::exception& e){
		        std::cout << "DCTSystem::SimulateV1() " << e.what() << "Exception occur" << std::endl;
		        std::exit(1);
	        }
            catch (std::string e){
                std::cout << "DCTSystem::SimulateV1() " << e << " error in opening/creating" << std::endl;
                std::exit(1);
            }
        }
    SimulationResultFPtr.close();
    return;
}


void DCTSystem::Simulate(){
    //  int Tau = (  int)ceil((double) d / Delta); //Sliding Window size
    int CCFront = -1, CCRear = -1, DCFront = -1, DCRear = -1;
    std::cout << "This simulate the DSTSystem" << std::endl;
    std::string DataFileName;
    #ifdef _WIN32
        DataFileName = "C:\\DCTSystem\\Data\\Graph.csv";
    #elif defined __linux__
        DataFileName = "/home/gautam/DCTSystem/Samples/Graph.csv";
    #endif
    int NoOfSimulationDays,i=0;
    std::cout<<"\nEnter number of days the simulation runs?";
    std::cin>>NoOfSimulationDays;
    int NoOfObservationDays, NoOfCloseContactsDuringADay;
    int NoOfCloseContactSlots;
    NoOfObservationDays = CG->GetNoOfObservationDays();
    NoOfCloseContactsDuringADay = CG->GetNoOfCloseContactsDuringADay();
    NoOfCloseContactSlots = CG->GetNoOfCloseContactSlots();
    while(i<NoOfSimulationDays){
        std::cout<<"Inside Simulation loop"<<std::endl;
        std::cout<<std::string(60,'*')<<std::endl;
        std::cout<<"\nDay No:"<<i+1<<std::endl;
        std::cout<<std::string(60,'*')<<std::endl;
        if (i>=NoOfObservationDays){
            /*Queue-Rear advances on enque operation, however here no direct dequeue
            operation is used so we need this code fragment for advancing the circular 
            queue, mainly when the queue is full. 
            */
            CCFront = (CCFront + NoOfCloseContactsDuringADay) % NoOfCloseContactSlots;
            DCFront = (DCFront + 1) % NoOfObservationDays; 
        }
        std::cout<<"Before:"<<std::endl;
        std::cout<<"CCFront="<<CCFront<<" CCRear="<<CCRear<<std::endl;
        std::cout<<"DCFront="<<DCFront<<" DCRear="<<DCRear<<std::endl;
        CG->BuildContactGraph(DataFileName,CCFront,CCRear,DCFront,DCRear);
        std::cout<<"Contact Graph:"<<std::endl;
        std::cout<<*CG<<std::endl;
        i++;
        std::cout<<"\nInside Simulation"<<std::endl;
        std::cout<<"After:"<<std::endl;
        std::cout<<"CCFront="<<CCFront<<" CCRear="<<CCRear<<std::endl;
        std::cout<<"DCFront="<<DCFront<<" DCRear="<<DCRear<<std::endl;
        std::cout<<std::string(20,'*')<<std::endl;
    }
}

ostream& operator<<(ostream& out, const DCTSystem& parDCTSys){
    out << "\nDCTSystem Descriptions:" << std::endl;
    out << "ID: " << parDCTSys.ID << std::endl;
    out << "N: "<< parDCTSys.N << std::endl;
    out << "D: " << parDCTSys.D << std::endl;
    out << "d: " << parDCTSys.d << std::endl;
    out << "q: " << parDCTSys.q << std::endl;
    out << "Delta: " << parDCTSys.Delta << std::endl;
    out << "Tau: " << parDCTSys.Tau << std::endl;
    out << *parDCTSys.CG <<std::endl;
    //out<<"\nEnd of Showing CG"<<std::endl;
    long long CalSize = 0, ObsrvSize = 0;
    parDCTSys.CG->StorageSize(CalSize,ObsrvSize);
    out << "Total Disk Storage Requirement:" << std::endl;
    out << "Calculated="<<CalSize<<" Observed="<<ObsrvSize<<std::endl;
    //out << "Total Disk Storage Requirement:" << parDCTSys.CG->StorageSize() << std::endl;
    return out;
}

//************************************ End of DCTSystem Library **********************