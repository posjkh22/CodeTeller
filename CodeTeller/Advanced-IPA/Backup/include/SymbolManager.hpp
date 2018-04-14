

#ifndef SYMBOLMANAGER
#define SYMBOLMANAGER

#include <iostream>
#include <fstream>
#include <string>
#include <jsoncpp/json/json.h>
#include <list>

namespace IPA
{

class ThreadAPI;
class PathUncountedFunction;
class ThreadSupportOS;

typedef std::list<std::string* > PathUncountedFunctionListTy;
typedef std::list<ThreadAPI* > ThreadSupportOSListTy;

class SymbolManager
{

public:
	/*
	SymbolManager(
			PathUncountedFunctionListTy* PUFL,
			ThreadSupportOSListTy* TSOL)
		: p_PUFL(PUFL), p_TSOL(TSOL){}
	*/

	SymbolManager(
			PathUncountedFunction* PUF,
			ThreadSupportOS* TSO);


public:
	PathUncountedFunctionListTy& getPUFL();



public:
	void DEBUG_showPUFL();


public:
	bool AddPUFL(std::string*);
	bool AddTSOL(std::string*,
			std::string*,
			std::string*, 
			std::string*, 
			std::string*,
			std::string*);

public:
	bool Process(const char* , const char*);

private:
	bool ReadOperatingSystemList(const char* filename);
	bool ReadTargetFunctionList(const char* filename);

private:
	PathUncountedFunctionListTy* p_PUFL;
	ThreadSupportOSListTy* p_TSOL;

};

class PathUncountedFunction
{



public:
	PathUncountedFunctionListTy* getPUFL();

private:
	PathUncountedFunctionListTy m_PUFL;

	

};


class ThreadAPI
{

private:
	std::string* os_name;
	std::string* taskgen;
	std::string* taskend;
	std::string* p_sem_init; 
	std::string* p_sem_wait;
	std::string* p_sem_post;

public:
	ThreadAPI(
			std::string* s0,
			std::string* s1,
			std::string* s2,
			std::string* s3,
			std::string* s4,
			std::string* s5)
		: os_name(s0), 
		taskgen(s1),
		taskend(s2),
		p_sem_init(s3), 
		p_sem_wait(s4), 
		p_sem_post(s5){}
			

};

class ThreadSupportOS
{


public:
	ThreadSupportOSListTy* getTSOL();

private:
	ThreadSupportOSListTy m_TSOL;

};



class CheckTarget
{







};










};

#endif
