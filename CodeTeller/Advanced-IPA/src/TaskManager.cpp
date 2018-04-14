
#include "TaskManager.hpp"

using namespace IPA;


bool TaskManager::Process()
{
	for(auto iter = m_TaskList.begin();
			iter != m_TaskList.end(); iter++)
	{

		Task* t = *iter;
		t->Process();
	}

	return true;

}


TaskManager::TaskManager(ArgumentPass *arg, IRcodeData *IRcode)
{
	
	
	if(arg->getArgument()->getThreadTy() == Argument::threadTy::UniThread)
	{
		
		isMultiTask = false;
		
		Task* t0 = new Task(
			IRcode->getIRcodeTextDataSet()->SearchFunction(std::string("main")),
			IRcode->getSymbolManager()
			);
		
		m_TaskList.push_back(t0);
	}

	else if(arg->getArgument()->getThreadTy() == Argument::threadTy::MultiThread)
	{
		
		setTaskNum(3);
		/*
		for(int i = 0 ; i < getTaskNum(); i++){  }
		*/

		Task* t0 = new Task(
			IRcode->getIRcodeTextDataSet()->SearchFunction(std::string("main")),
			IRcode->getSymbolManager()
			);
	

		Task* t1 = new Task(
			IRcode->getIRcodeTextDataSet()->SearchFunction(std::string("task1")),
			IRcode->getSymbolManager()
			);
		Task* t2 = new Task(
			IRcode->getIRcodeTextDataSet()->SearchFunction(std::string("task2")),
			IRcode->getSymbolManager()
			);

		isMultiTask = true;
		m_TaskList.push_back(t0);
		m_TaskList.push_back(t1);
		m_TaskList.push_back(t2);

		setTaskNum(3);

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


