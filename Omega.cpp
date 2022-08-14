#include "Omega.h"
#include "Helper.h"

using namespace std;

//****************************************************************************************
//*********************************** Omega Library **************************************
//****************************************************************************************

Omega::Omega(){
	UID = 0;
	NoOfSlots = 0;
	N = 0;
	Delta = 0;
	Edges.clear();
	long long RecCount;
	std::fstream OmgFPtr;
	OmgFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
	std::fstream OmgHeaderFPtr;
	OmgHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
	try{
		OmgHeaderFPtr.open(OmegaHeaderFName, ios::in | ios::binary);
		if (OmgHeaderFPtr.fail()) throw OmegaHeaderFName;  // the exception being checked

		OmgHeaderFPtr.read((char*)this, sizeof(Omega));
		OmgHeaderFPtr.close();
	
		OmgFPtr.open(OmegaFName, ios::in | ios::binary);
		if (OmgFPtr.fail()) throw OmegaFName;  // the exception being checked
		
		OmgFPtr.read((char *)&RecCount, sizeof(RecCount));
		OmgFPtr.close();
		if ((N > 0 && RecCount==0)|| (N<=0 && RecCount>0) ){
			std::cout << "\nError:Omega::Omega()-unsuccessful in openning the existing Omega" << std::endl;
			return;
		}
		std::cout << "\nExisting Omega Object opened successfully." << std::endl;
		return;
	}
	catch (std::fstream::failure &e){
		std::cerr << "Omega::Omega() opening/reading/closing file\n";
		std::exit(1);
	}
	catch (std::string e){
		std::cout << "Omega::Omega() " << e << " error in opening" << std::endl;
		std::exit(1);
	}
	catch (std::bad_alloc& e){
		std::cout << "Omega::Omega() " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "Omega::Omega() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

Omega::Omega(long parUID,int parNoOfSlots,long parN,int parDelta){
	UID = parUID;
	NoOfSlots = parNoOfSlots;
	N = parN;
	Delta = parDelta;
	Edges.clear();

	std::fstream OmgFPtr;
	OmgFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

	std::fstream OmgHeaderFPtr;
	OmgHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
	try{
		OmgHeaderFPtr.open(OmegaHeaderFName, ios::out | ios::binary);
		if (OmgHeaderFPtr.fail()) throw OmegaHeaderFName;  // the exception being checked

		OmgHeaderFPtr.write((char*)this, sizeof(Omega));
		OmgHeaderFPtr.close();

		OmgFPtr.open(OmegaFName, ios::out | ios::binary);
		if (OmgFPtr.fail()) throw OmegaFName;  // the exception being checked
	
		std::cout << "\nNew Omega Object created successfully." << std::endl;
		return;
	}
	catch (std::fstream::failure &e){
		std::cerr << "Omega::Omega(parUID,parNoOfSlots,parN,parDelta) opening/reading/closing file\n";
		std::exit(1);
	}
	catch (std::string e){
		std::cout << "Omega::Omega(parUID,parNoOfSlots,parN,parDelta) " << e << " error in opening" << std::endl;
		std::exit(1);
	}
	catch (std::bad_alloc& e){
		std::cout << "Omega::Omega(parUID,parNoOfSlots,parN,parDelta) " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "Omega::Omega(parUID,parNoOfSlots,parN,parDelta) " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

Omega::Omega(long parUID,int parNoOfSlots,long parN,int parDelta,std::string parDataFName){
	UID = parUID;
	NoOfSlots = parNoOfSlots;
	N = parN;
	Delta = parDelta;
	Edges.clear();

	std::fstream GraphFPtr;
	GraphFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
	std::fstream OmgFPtr;
	OmgFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
	std::fstream OmgHeaderFPtr;
	OmgHeaderFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);

	long UID1, UID2;
	long long RecCount=0,FPos;	
	//char Delm = ',';
	std::string Line;
	std::vector<std::string> Token;
	char Delm = ',';

	try{
		OmgHeaderFPtr.open(OmegaHeaderFName, ios::out | ios::binary);
		if (OmgHeaderFPtr.fail()) throw OmegaHeaderFName;  // the exception being checked

		OmgHeaderFPtr.write((char*)this, sizeof(Omega));
		OmgHeaderFPtr.close();

		OmgFPtr.open(OmegaFName, ios::out | ios::binary);
		if (OmgFPtr.fail()) throw OmegaFName;  // the exception being checked
	
		GraphFPtr.open(parDataFName, ios::in | ios::binary);
		if (GraphFPtr.fail()) throw parDataFName;  // the exception being checked

		if (!std::getline(GraphFPtr, Line)){
			OmgFPtr.close();
		    GraphFPtr.close();
			std::cout << "Error in ContactGraph::BuildContactGraph() -" << parDataFName << " No data to read" << std::endl;
			return;
		}
        GraphFPtr.seekg(0,ios::end);
        FPos = GraphFPtr.tellg();
		GraphFPtr.seekg(0,ios::beg);
		
		OmgFPtr.write((char *)&RecCount, sizeof(RecCount));
		Token = Split(Line, Delm);
		UID1 = stoll(Token[0], nullptr, 10);
		UID2 = stoll(Token[1], nullptr, 10);
		//std::cout << "(" << UID1 << "," << UID2 << ")" << std::endl;
	
		std::vector<bool> EdgeLabel(NoOfSlots, false);
		static std::mt19937 generator;
		static std::uniform_int_distribution<int> distribution(0, 1);
		for (auto val : EdgeLabel) val = distribution(generator);

		int Len = (int)ceil(NoOfSlots / 8.0);
        char* Str = new char[Len + 1];
        Str = writeAsBytes(EdgeLabel);
		OmgFPtr.write((char *)&UID1, sizeof(UID1));
		OmgFPtr.write((char *)&UID2, sizeof(UID2));
		OmgFPtr.write((char *)Str, Len);
		RecCount++;
		while (true){
			if((FPos - GraphFPtr.tellg())<=1)break;

            std::getline(GraphFPtr, Line);
			Token = Split(Line, Delm);
			UID1 = stoll(Token[0], nullptr, 10); 
			UID2 = stoll(Token[1], nullptr, 10);
			std::cout << "Inside Omega:(" << UID << "," << UID2 << ")" << std::endl;
			static std::mt19937 generator;
			static std::uniform_int_distribution<int> distribution(0, 1);
			for (auto val : EdgeLabel) val = distribution(generator);

			Str = writeAsBytes(EdgeLabel);
			OmgFPtr.write((char *)&UID1, sizeof(UID1));
			OmgFPtr.write((char *)&UID2, sizeof(UID2));
			OmgFPtr.write((char *)Str, Len);
			RecCount++;
		}
		OmgFPtr.seekg(0,ios::beg);
		OmgFPtr.write((char *)&RecCount, sizeof(RecCount));
		OmgFPtr.close();
		GraphFPtr.close();
		std::cout << "\nNew Omega Object created successfully." << std::endl;
		return;
	}
	catch (std::fstream::failure &e){
		std::cerr << "Omega::Omega(parUID,parNoOfSlots,parN,parDelta) opening/reading/closing file\n";
		std::exit(1);
	}
	catch (std::string e){
		std::cout << "Omega::Omega(parUID,parNoOfSlots,parN,parDelta) " << e << " error in opening" << std::endl;
		std::exit(1);
	}
	catch (std::bad_alloc& e){
		std::cout << "Omega::Omega(parUID,parNoOfSlots,parN,parDelta) " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "Omega::Omega(parUID,parNoOfSlots,parN,parDelta) " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

bool Omega::GetOmegaFName(char parOmegaFName[50]){
	std::fstream OmgFPtr;
	OmgFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
	long long RecCount=0;
	try{
		OmgFPtr.open(OmegaFName, ios::in | ios::binary);
		if (OmgFPtr.fail()) throw OmegaFName;  // the exception being checked

		OmgFPtr.read((char *)&RecCount, sizeof(RecCount));
		OmgFPtr.close();

		if(RecCount==0){
			std::cout << "Error in Omega::GetOmegaFName() - empty edge list" << std::endl;
			return false;
		}
		strcpy(parOmegaFName, OmegaFName);
		return true;
	}
	catch (std::fstream::failure &e){
		std::cerr << "Omega::GetOmegaFName() opening/reading/closing file\n";
		std::exit(1);
	}
	catch (std::string e){
		std::cout << "Omega::GetOmegaFName() " << e << " error in opening" << std::endl;
		std::exit(1);
	}
	catch (std::bad_alloc& e){
		std::cout << "Omega::GetOmegaFName() " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "Omega::GetOmegaFName() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

bool Omega::GetEdgeLabel(long parUID, std::vector<bool>& parEdgeLabel){
	if (parUID < 0L || parUID >= N){
		std::cout << "Error in Omega::GetEdgeLabel()-user ID (" << parUID << ") not in range" << std::endl;
		return false;
	}
	try{
		if (!Edges.empty()){
			auto it = find_if(Edges.begin(), Edges.end(), [parUID](const pair<long, vector<bool>>& p) { return p.first == parUID; });
			if (it == Edges.end()){
				std::cout << "Error in Omega::GetEdgeLabel()- user (ID:" << parUID << ") is not related" << std::endl;
				return false;
			}
			parEdgeLabel = it->second;
			return true;
		}
		std::cout << "Error in Omega::GetEdgeLabel() - empty edge list" << std::endl;
		return false;
	}
	catch (std::out_of_range& e){
		std::cout << "Omega::GetEdgeLabel() " << e.what() << " Not in range" << std::endl;
		std::exit(1);
	}
	catch (std::bad_alloc& e){
		std::cout << "Omega::GetEdgeLabel() " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "Omega::GetEdgeLabel() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

bool Omega::GetEdges(std::set<std::pair<long, std::vector<bool>>>& parEdges){
	return true;
}

long Omega::GetUID(){
	return UID;
}
unsigned long Omega::GetNoOfSlots(){
	return NoOfSlots;
}
long Omega::GetN(){
	return N;
}

bool Omega::Build(string parDataFName){
	//Value of the slots are generated by the sliding window on the data stream coming 
	//from the smartphone users. Here Delta is used.
	return true;
}

bool Omega::CreateEdge(long parUID){
	//Default edge creation. This is the edge with all false slots.
	if (parUID < 0L || parUID >= N){
		std::cout << "Error in Omega::CreateEdge()- user ID(" << parUID << ") not in range" << std::endl;
		return false;
	}
	try{
		if (!Edges.empty()){
			auto it = find_if(Edges.begin(), Edges.end(), [parUID](const pair<long, vector<bool>>& p) { return p.first == parUID; });
			if (it != Edges.end()){
				std::cout << "Error in Omega::CreateEdge()- user ID(" << parUID << ") duplicate edge" << std::endl;
				return false;
			}
		}
		std::vector<bool> Label(NoOfSlots, false);
		std::pair<long, std::vector<bool>> E;
		E = std::make_pair(parUID, Label);
		Edges.insert(E);
		return true;
	}
	catch (std::out_of_range& e){
		std::cout << "Omega::CreateEdge() " << e.what() << " Not in range" << std::endl;
		std::exit(1);
	}
	catch (std::bad_alloc& e){
		std::cout << "Omega::CreateEdge() " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "Omega::CreateEdge() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

bool Omega::CreateEdge(long parUID, std::vector<bool> parEdgeLabel){
	//This will create the edge with supplied edge labels
	if (parUID < 0L || parUID >= N){
		std::cout << "Error in Omega::CreateEdge()-user ID (" << parUID << ") not in range" << std::endl;
		return false;
	}
	try{
		if (parEdgeLabel.size() != NoOfSlots){
			std::cout << "Error in Omega::CreateEdge()- Size(" << parEdgeLabel.size() << ") is not correct" << std::endl;
			return false;
		}
		std::pair<long, std::vector<bool>> E;
		E = std::make_pair(parUID, parEdgeLabel);
		Edges.insert(E);
		return true;
	}
	catch (std::out_of_range& e){
		std::cout << "Omega::CreateEdge() " << e.what() << " Not in range" << std::endl;
		std::exit(1);
	}
	catch (std::bad_alloc& e){
		std::cout << "Omega::CreateEdge() " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "Omega::CreateEdge() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

bool Omega::RemoveEdge(long& parUID, std::vector<bool>& parEdgeLabel){
	parUID = -1L;
	std::pair<long, std::vector<bool>> e;
	try
	{
		if(Edges.empty()) return false;

		auto it = Edges.begin();
		e = *it;
		parUID = e.first;
		parEdgeLabel = e.second;
		Edges.erase(it);
		return true;	
	}
	catch (std::out_of_range& e){
		std::cout << "Omega::RemoveEdge() " << e.what() << " Not in range" << std::endl;
		std::exit(1);
	}
	catch (std::bad_alloc& e){
		std::cout << "Omega::RemoveEdge() " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "Omega::RemoveEdge() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}	
}

bool Omega::BuildRandom(string parDataFName){
	//value of the slots are random for randomly generated graph
	//New data will enter for the parT time duration. 
	//All bits for this time period will be uploaded after current parCCRear of the contact vector queue
	// 	   as well as day vector queue with entry point after parDRear.
	// 	   This will not change the front and rear indices of the both queues.
	//string ArtificialGraph = "Data\\GraphData.csv";

	std::fstream GraphFPtr;
	GraphFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
	//char Delm = ',';
	std::string Line;
	std::vector<std::string> Token;
	long UID;
	try{
		GraphFPtr.open(parDataFName, ios::in | ios::binary);
		if (GraphFPtr.fail()) throw parDataFName;  // the exception being checked

		while (std::getline(GraphFPtr, Line)){
			UID = stoll(Line, nullptr, 10);

			std::vector<bool> Vec(NoOfSlots, false);
			static std::mt19937 generator;
			static std::uniform_int_distribution<int> distribution(0, 1);
			for (auto val : Vec) val = distribution(generator);
			
			if (!CreateEdge(UID, Vec)){
				GraphFPtr.close();
				std::cout << "Error in Omega::BuildRandom()- unsuccessful in edge creation" << std::endl;
				return false;
			}
		}
		GraphFPtr.close();
		return true;
	}
	catch (std::fstream::failure &e){
		std::cerr << "Omega::BuildRandom()-" << e.what() << "opening/reading/closing file\n";
		std::exit(1);
	}
	catch (std::string e){
		std::cout << "Omega::BuildRandom() " << e << " error in opening" << std::endl;
		std::exit(1);
	}
	catch (std::out_of_range& e){
		std::cout << "Omega::BuildRandom() " << e.what() << " Not in range" << std::endl;
		std::exit(1);
	}
	catch (std::bad_alloc& e){
		std::cout << "Omega::BuildRandom() " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "Omega::BuildRandom() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}


bool Omega::Reset(long parUID){
	if (parUID < 0L || parUID >= N){
		std::cout << "Error in Omega::Reset()- user ID (" << parUID << ") not in range" << std::endl;
		return false;
	}
	UID = parUID;
	try{
		Edges.clear();
		return true;
	}
	catch (std::string e){
		std::cout << "Omega::Reset() " << e << " error in opening" << std::endl;
		std::exit(1);
	}
	catch (std::out_of_range& e){
		std::cout << "Omega::Reset() " << e.what() << " Not in range" << std::endl;
		std::exit(1);
	}
	catch (std::bad_alloc& e){
		std::cout << "Omega::Reset() " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "Omega::Reset(parUID) " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

bool Omega::GetASlot(long parUID, unsigned long parSlotNo, bool& parSlotVal){
	if (parUID < 0L || parUID >= N){
		std::cout << "Error in Omega::GetASlot()- user ID (" << parUID << ") not in range" << std::endl;
		return false;
	}
	if (parSlotNo < 0L || parSlotNo >= NoOfSlots){
		std::cout << "Error in Omega::GetASlot()- slot number (" << parSlotNo << ") not in range" << std::endl;
		return false;
	}
	try{
		if (Edges.empty()){
			std::cout << "Error in Omega::GetASlot()- empty edge list" << std::endl;
			return false;
		}

		auto it = find_if(Edges.begin(), Edges.end(), [parUID](const pair<long, vector<bool>>& p) { return p.first == parUID; });
		if (it == Edges.end()){
			std::cout << "Error in Omega::GetASlot()- user (ID:" << parUID << ") is not related" << std::endl;
			return false;
		}
		std::vector<bool> Label = it->second;
		parSlotVal = Label[parSlotNo];
		return true;
	}
	catch (std::out_of_range& e){
		std::cout << "Omega::GetASlot() " << e.what() << " Not in range" << std::endl;
		std::exit(1);
	}
	catch (std::bad_alloc& e){
		std::cout << "Omega::GetASlot() " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "Omega::GetASlot() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

bool Omega::SetASlot(long parUID, unsigned long parSlotNo, bool parSlotVal){
	if (parUID < 0L || parUID >= N){
		std::cout << "Error in Omega::SetASlot()- user ID (" << parUID << ") not in range" << std::endl;
		return false;
	}
	if (parSlotNo < 0L || parSlotNo >= NoOfSlots){
		std::cout << "Error in Omega::SetASlot()- slot number (" << parSlotNo << ") not in range" << std::endl;
		return false;
	}
	try{
		if (Edges.empty()){
			std::cout << "Error in Omega::SetASlot()- empty edge list" << std::endl;
			return false;
		}
		
		auto it = find_if(Edges.begin(), Edges.end(), [parUID](const pair<long, vector<bool>>& p) { return p.first == parUID; });
		if (it == Edges.end()){
			std::cout << "Error in Omega::SetASlot()- user (ID:" << parUID << ") is not related" << std::endl;
			return false;
		}
		std::vector<bool> Label = it->second;
		Label[parSlotNo] = parSlotVal;
		Edges.erase(it);
		Edges.insert(std::make_pair(parUID, Label));
		return true;
	}
	catch (std::out_of_range& e){
		std::cout << "Omega::SetASlot() " << e.what() << " Not in range" << std::endl;
		std::exit(1);
	}
	catch (std::bad_alloc& e){
		std::cout << "Omega::SetASlot() " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "Omega::SetASlot() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

void Omega::Show(){
	std::cout << "\nNumber of Users:" << N << std::endl;
	std::cout << "Number of Slots in the Edge Label:" << NoOfSlots << std::endl;
	std::cout << "Contacts for User:" << UID << std::endl;
	std::cout << "Detail Contact Information:" << std::endl;
	std::fstream OmgFPtr;
	OmgFPtr.exceptions(std::fstream::failbit | std::fstream::badbit);
	
	long UID1, UID2;
	long long RecCount=0;	

	try{
		for (auto const& e : Edges){
			std::cout << e.first << " ";
			for (auto const& v : e.second)
				std::cout << v;
			std::cout << std::endl;
		}
		OmgFPtr.open(OmegaFName, ios::in | ios::binary);
		if (OmgFPtr.fail()) throw OmegaFName;  // the exception being checked

		OmgFPtr.read((char *)&RecCount, sizeof(RecCount));

		std::cout<<"Number of Omega Record:"<<RecCount<<std::endl;

		int Len = (int)ceil(NoOfSlots / 8.0);
        char* Str = new char[Len + 1];
		for(int i=0;i<RecCount;i++){
			OmgFPtr.read((char *)&UID1, sizeof(UID1));
			OmgFPtr.read((char *)&UID2, sizeof(UID2));
			OmgFPtr.read((char *)Str, Len);
			Str[Len]='\0';
			std::cout<<UID1<<","<<UID2<<",";
			std::cout<<Str<<std::endl;
		}
		OmgFPtr.close();
		return;
	}
	catch (std::fstream::failure &e){
		std::cerr << "Omega::Show()-" << e.what() << "opening/reading/closing file\n";
		std::exit(1);
	}
	catch (std::string e){
		std::cout << "Omega::Show() " << e << " error in opening" << std::endl;
		std::exit(1);
	}
	catch (std::out_of_range& e){
		std::cout << "Omega::Show() " << e.what() << " Not in range" << std::endl;
		std::exit(1);
	}
	catch (std::bad_alloc& e){
		std::cout << "Omega::Show() " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "Omega::Show() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}

ostream& operator<<(ostream& parOut, const Omega& parOmega){
	parOut << "\nNumber of Users:" << parOmega.N << std::endl;
	parOut << "Number of Slots in the Edge Label:" << parOmega.NoOfSlots << std::endl;
	parOut << "Contacts for User:" << parOmega.UID << std::endl;
	parOut << "Detail Contact Information:" << std::endl;
	try{
		for (auto const& e : parOmega.Edges){
			parOut << e.first << " ";
			for (auto const& v : e.second)
				parOut << v;
			parOut << std::endl;
		}
		return parOut;
	}
	catch (std::out_of_range& e){
		std::cout << "Omega::operator<<() " << e.what() << " Not in range" << std::endl;
		std::exit(1);
	}
	catch (std::bad_alloc& e){
		std::cout << "Omega::operator<<() " << e.what() << " bad allocation" << std::endl;
		std::exit(1);
	}
	catch (std::exception& e){
		std::cout << "Omega::operator<<() " << e.what() << "Exception occur" << std::endl;
		std::exit(1);
	}
}
//************************************ End of Omega Library *****************************
