
#ifndef MODESELECTOR
#define MODESELECTOR

#include "ArgumentPass.hpp"
#include "wFunction.hpp"
#include "IRcodeData.hpp"
#include "Task.hpp"
#include <list>

namespace IPA
{
	class TaskManager;
	class Task;


//typedef std::list< wFunction* > TaskList;
//typedef wFunction Task;

typedef std::list<Task* > TaskList;

class TaskManager
{

public:
	TaskManager(ArgumentPass* arg, IRcodeData* IRcode);


public:
	TaskList *getTaskList();
	Task* getTask();
	unsigned int getTaskNum();

private:
	bool setTaskNum(unsigned int);

private:
	bool isMultiTask;
	unsigned int taskNum;
	TaskList m_TaskList;

};

};

#endif
