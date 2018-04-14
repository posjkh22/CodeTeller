
#ifndef IPATASK
#define IPATASK

#include <list>
#include "wFunction.hpp"
#include "wBasicBlock.hpp"
#include "PathImpl.hpp"
#include "Path.hpp"
#include "checker.hpp"

class wFunction;
class wBasicBlock;
class PathImpl;
class Path;

namespace IPA
{

typedef std::list<llvm::Value* > SharedResourcesListTy;
typedef std::list<wFunction* > FunctionListTy;
typedef std::list<wFunction* > NonRetrantFunctionListTy;
typedef wFunction EntryFunctionTy;
typedef std::list<llvm::Value* > SemaphoreValueListTy;


typedef std::list<Path *> PathListTy;


class Task
{
public:
	friend class Checker;

private:
	IPA::SymbolManager* p_symbolManager;

public:
	Task(EntryFunctionTy*, SymbolManager* );

	bool Process();

private:
	EntryFunctionTy* m_entryFunction;

public:
	bool SetEntryFunction(EntryFunctionTy* );
	EntryFunctionTy* GetEntryFunction();

public:
	std::string getTaskName();

private:
	PathListTy* m_pathList;

public:
	bool GeneratePathList();
	PathListTy* getPathList();



private:
	FunctionListTy m_FunctionList;
	NonRetrantFunctionListTy m_NonRFL;
	SemaphoreValueListTy m_DSVL;
	SemaphoreValueListTy m_LSVL;
	SemaphoreValueListTy m_USVL;
public:
	SharedResourcesListTy m_SRL;

public:
	bool DetermineFunctionList();
	bool DetermineNonRFL();
	bool ShowSemaphoreVariableList();
	bool ShowSemaphoreVariableList(const char*);
	bool DetermineSemaphoreVariableList();
	bool DetermineSharedResourcesList();
	bool ShowFunctionList();
	bool ShowFunctionList(const char*);
	bool ShowNonRFL();
	bool ShowNonRFL(const char*);
	bool ShowSRL();
	bool ShowSRL(const char*);

};

/*
class TaskCreator
{

public:
	TaskCreator(Task* , EntryFunctionTy* );
	~TaskCreator();

};
*/
};

#endif
