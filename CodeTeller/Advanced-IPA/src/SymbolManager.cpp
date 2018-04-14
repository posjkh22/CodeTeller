
#include "SymbolManager.hpp"

using namespace IPA;


void SymbolManager::DEBUG_showPUFL()
{

	std::ofstream fout;
	fout.open("./dat/PathUnCountedFunctionList.dat", std::ofstream::out);
	
#ifdef SYM_DEBUG
	std::cout << "[DEBUG] PUFL" << std::endl;
#endif
	for(auto iter = (*p_PUFL).begin();
			iter != (*p_PUFL).end(); iter++)
	{
#ifdef SYM_DEBUG
		std::cout << *(*iter) << std::endl;
#endif
		fout << *(*iter) << std::endl;
	}
	std::cout << std::endl;

	fout.close();

}

SymbolManager::SymbolManager(
			IPA::PathUncountedFunction* PUF,
			IPA::ThreadSupportOS* TSO)
{
		p_PUFL = PUF->getPUFL();
		p_TSOL = TSO->getTSOL();
}

bool SymbolManager::AddPUFL(std::string* str)
{
	p_PUFL->push_back(str);

	return true;
}

PathUncountedFunctionListTy& SymbolManager::getPUFL()
{
	return *p_PUFL;
}


bool SymbolManager::AddTSOL(
		std::string* s0,
		std::string* s1,
		std::string* s2,
		std::string* s3,
		std::string* s4,
		std::string* s5
		)
{
	p_TSOL->push_back(
			new ThreadAPI(s0, s1, s2, s3, s4, s5));

	return true;
}


bool SymbolManager::Process(
		const char* f1, const char* f2)
{
	ReadOperatingSystemList(f1);
	ReadTargetFunctionList(f2);

	return true;
}



bool SymbolManager::ReadOperatingSystemList(const char* filename)
{

	std::ifstream ifs(filename);
    Json::Reader reader;

	/* Json value : storing & representing the pair-value */
	Json::Value obj;

	/* Read file from ifs, store to obj(Value) */
    reader.parse(ifs, obj); // reader can also read strings

	/* String Output */
    const Json::Value& OperatingSystem = obj["OperatingSystem"]; // array of OperatingSystem

	for (auto iter1 = OperatingSystem.begin(); iter1 != OperatingSystem.end(); iter1++)
	{

		const Json::Value& OSType = *iter1;
#ifdef SYM_DEBUG
		std::cout << "OS Name: " << OSType["OSName"].asString() << std::endl;
#endif

		const Json::Value& OSTI = OSType["ThreadInterface"];

#ifdef SYM_DEBUG
		std::cout << "-SEM_INIT/ SEM_WAIT/ SEM_POST" << std::endl;
		std::cout << " " << OSTI["sem_init"].asString() << "/ ";
		std::cout << OSTI["sem_wait"].asString() << "/ ";
		std::cout << OSTI["sem_post"].asString() << std::endl;
#endif


		AddTSOL(
				new std::string(OSType["OSName"].asString()),
				new std::string(OSTI["TaskGen"].asString()),
				new std::string(OSTI["TaskEnd"].asString()),
				new std::string(OSTI["sem_init"].asString()), 
				new std::string(OSTI["sem_wait"].asString()), 
				new std::string(OSTI["sem_post"].asString())
				);
		
		//AddPUFL(new std::string(OSType["OSName"].asString()));
		AddPUFL(new std::string(OSTI["TaskGen"].asString()));
		AddPUFL(new std::string(OSTI["TaskEnd"].asString()));
		AddPUFL(new std::string(OSTI["sem_init"].asString()));
		AddPUFL(new std::string(OSTI["sem_wait"].asString()));
		AddPUFL(new std::string(OSTI["sem_post"].asString()));


	}

	return true;
}


bool SymbolManager::ReadTargetFunctionList(const char* filename)
{

	std::ifstream ifs(filename);
    Json::Reader reader;

	/* Json value : storing & representing the pair-value */
	Json::Value obj;

	/* Read file from ifs, store to obj(Value) */
    reader.parse(ifs, obj); // reader can also read strings

	/* String Output */
    const Json::Value& TargetFunctionList = obj["TargetFunctionList"]; // array of OperatingSystem
    
	for (auto iter1 = TargetFunctionList.begin(); 
			iter1 != TargetFunctionList.end(); iter1++)
	{

		const Json::Value& FP = *iter1;

		if(FP["Purpose"].asString() == "MemoryAllocationC")
		{
#ifdef SYM_DEBUG
			std::cout << "Purpose: " << FP["Purpose"].asString() << std::endl;
#endif
			const Json::Value& FL = FP["FunctionNameList"];
			for(unsigned int i = 0; i < FL.size(); i++)
			{
#ifdef SYM_DEBUG
				std::cout << "Alloc: " << FL[i]["Alloc"].asString() << std::endl;
				std::cout << "Dealloc: " << FL[i]["Dealloc"].asString() << std::endl;
#endif
				AddPUFL(new std::string(FL[i]["Alloc"].asString()));
				AddPUFL(new std::string(FL[i]["Dealloc"].asString()));
			}
#ifdef SYM_DEBUG
			std::cout << std::endl;
#endif
		}

		else if(FP["Purpose"].asString() == "FileAccess")
		{
#ifdef SYM_DEBUG
			std::cout << "Purpose: " << FP["Purpose"].asString() << std::endl;
#endif
			const Json::Value& FL = FP["FunctionNameList"];
			for(unsigned int i = 0; i < FL.size(); i++)
			{
#ifdef SYM_DEBUG
				std::cout << "open: " << FL[i]["open"].asString() << std::endl;
				std::cout << "close: " << FL[i]["close"].asString() << std::endl;
#endif	
				AddPUFL(new std::string(FL[i]["open"].asString()));
				AddPUFL(new std::string(FL[i]["close"].asString()));
			}
#ifdef SYM_DEBUG
			std::cout << std::endl;
#endif
		}

		else if(FP["Purpose"].asString() == "llvmDebugCore")
		{
#ifdef SYM_DEBUG
			std::cout << "Purpose: " << FP["Purpose"].asString() << std::endl;
#endif

			const Json::Value& FL = FP["FunctionNameList"];
			for(unsigned int i = 0; i < FL.size(); i++)
			{
#ifdef SYM_DEBUG
				std::cout << "Name: " << FL[i]["Name"].asString() << std::endl;
#endif
				AddPUFL(new std::string(FL[i]["Name"].asString()));
			}
#ifdef SYM_DEBUG
			std::cout << std::endl;
#endif
		}

	}

	return true;
}

PathUncountedFunctionListTy* PathUncountedFunction::getPUFL()
{
	return &m_PUFL;
}

ThreadSupportOSListTy* ThreadSupportOS::getTSOL()
{
	return &m_TSOL;
}





