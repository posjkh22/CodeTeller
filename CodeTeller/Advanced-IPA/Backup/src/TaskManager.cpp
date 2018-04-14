
#include "TaskManager.hpp"

using namespace IPA;

TaskManager::TaskManager(ArgumentPass *arg, IRcodeData *IRcode)
{
	
	
	if(arg->getArgument()->getThreadTy() == Argument::threadTy::UniThread)
	{
		
		isMultiTask = false;
		
		Task* t0 = new Task(
			IRcode->getIRcodeTextDataSet()->SearchFunction(std::string("main")));
		
		m_TaskList.push_back(t0);
	}

	else if(arg->getArgument()->getThreadTy() == Argument::threadTy::MultiThread)
	{
		
		setTaskNum(2);
		/*
		for(int i = 0 ; i < getTaskNum(); i++){  }
		*/

		Task* t1 = new Task(
				IRcode->getIRcodeTextDataSet()->SearchFunction(std::string("task1")));
		Task* t2 = new Task(
				IRcode->getIRcodeTextDataSet()->SearchFunction(std::string("task2")));


		isMultiTask = true;
		m_TaskList.push_back(t1);
		m_TaskList.push_back(t2);

		setTaskNum(2);

	}
	else
	{
		std::cerr << "Undefined TaskTy " << std::endl;
	}
}

TaskList* TaskManager::getTaskList()
{
	return &m_TaskList;
}

Task* TaskManager::getTask()
{ 
	Task* retTask = m_TaskList.back();
	m_TaskList.pop_back();
	
	return retTask;
}

bool TaskManager::setTaskNum(unsigned int num)
{
	taskNum = num;
	return true;
}

unsigned int TaskManager::getTaskNum()
{
	if(isMultiTask == true)
		return taskNum;

	else
		return 1;
}


