
#ifndef IPATASK
#define IPATASK

#include <list>
#include "wFunction.hpp"
#include "wBasicBlock.hpp"
#include "PathImpl.hpp"
#include "Path.hpp"

class wFunction;
class wBasicBlock;
class PathImpl;
class Path;

namespace IPA
{

typedef std::list<wFunction* > FunctionListTy;
typedef std::list<wFunction* > ReentrantFunctionListTy;
typedef wFunction EntryFunctionTy;

typedef std::list<Path *> PathListTy;


class Task
{

public:
	Task(EntryFunctionTy* );

private:
	EntryFunctionTy* m_entryFunction;

public:
	bool SetEntryFunction(EntryFunctionTy* );
	EntryFunctionTy* GetEntryFunction();


private:
	PathListTy* m_pathList;

public:
	bool GeneratePathList();
	PathListTy* getPathList();



private:
	FunctionListTy m_FunctionList;
	ReentrantFunctionListTy m_ReentrantFunctionList;

public:
	bool DetermineFunctionList();
	bool DetermineReentrantFunctionList();
	bool ShowFunctionList();
	bool ShowReentrantFunctionList();





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
