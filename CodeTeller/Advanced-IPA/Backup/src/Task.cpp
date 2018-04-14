
#include "Task.hpp"

using namespace IPA;


Task::Task(EntryFunctionTy* entry)
{
	SetEntryFunction(entry);
	GeneratePathList();
	DetermineFunctionList();
	DetermineReentrantFunctionList();

}


bool Task::SetEntryFunction(EntryFunctionTy* EntryFunction)
{
	m_entryFunction = EntryFunction; 
	return true;
}

EntryFunctionTy* Task::GetEntryFunction()
{
	return m_entryFunction;
}



bool Task::GeneratePathList()
{
	PathImpl pathImpl;
	m_pathList = pathImpl.GenerateGPL(m_entryFunction->getPathList());

	return true;
}

PathListTy* Task::getPathList()
{
	return m_pathList;
}

bool Task::DetermineFunctionList()
{

	for(auto iter1 = m_pathList->begin();
			iter1 != m_pathList->end(); iter1++)
	{
		Path* currentPath = *iter1;
		
		for(auto iter2 = currentPath->begin();
				iter2 != currentPath->end(); iter2++)
		{
			wBasicBlock* currentBasicBlock = *iter2;	
			wFunction* parentFunction = currentBasicBlock->getParent();	
		
			if(m_FunctionList.empty())
			{
				m_FunctionList.push_back(parentFunction);
			}

			else
			{
				bool FunctionFoundFlag = false;
				for(auto iter3 = m_FunctionList.begin();
						iter3 != m_FunctionList.end(); iter3++)
				{
					if(parentFunction == *iter3)
					{
						FunctionFoundFlag = true;
						break;
				 	}
				
				}
	
				if(FunctionFoundFlag == false)
				{
					m_FunctionList.push_back(parentFunction);
				}
			}

		}

	}

	return true;
}

bool Task::DetermineReentrantFunctionList()
{

	for(auto iter1 = m_pathList->begin();
			iter1 != m_pathList->end(); iter1++)
	{
		Path* currentPath = *iter1;
		
		for(auto iter2 = currentPath->begin();
				iter2 != currentPath->end(); iter2++)
		{
			wBasicBlock* currentBasicBlock = *iter2;	
			wFunction* parentFunction = currentBasicBlock->getParent();	
		
			if(m_ReentrantFunctionList.empty())
			{
				m_ReentrantFunctionList.push_back(parentFunction);
			}

			else
			{
				bool FunctionFoundFlag = false;
				for(auto iter3 = m_ReentrantFunctionList.begin();
						iter3 != m_ReentrantFunctionList.end(); iter3++)
				{
					if(parentFunction == *iter3)
					{
						FunctionFoundFlag = true;
						break;
					}
					
				}
				if(FunctionFoundFlag == false)
				{
					if(parentFunction->isReentrant())
					{
						m_ReentrantFunctionList.push_back(parentFunction);
					}
				}
			}
		}

	}

	return true;
}


bool Task::ShowFunctionList()
{
	std::cout << "Function" << std::endl;
	std::cout << "Num: " << m_FunctionList.size() << std::endl;
	for(auto iter1 = m_FunctionList.begin();
			iter1 != m_FunctionList.end(); iter1++)
	{
		wFunction* currentFunc = *iter1;
		//std::cout << currentFunc->getName() << " ";
		std::cout << currentFunc->getFunction()->getName().str() << " ";


	}
	std::cout << std::endl;

	return true;
}

bool Task::ShowReentrantFunctionList()
{
	std::cout << "Reentrant Function" << std::endl;
	std::cout << "Num: " << m_ReentrantFunctionList.size() << std::endl;
	for(auto iter1 = m_ReentrantFunctionList.begin();
			iter1 != m_ReentrantFunctionList.end(); iter1++)
	{
		wFunction* currentFunc = *iter1;
		//std::cout << currentFunc->getName() << " ";
		std::cout << currentFunc->getFunction()->getName().str() << " ";

	}
	std::cout << std::endl;

	return true;
}




/*
TaskCreator::TaskCreator(Task* task, EntryFunctionTy* entry)
{
	task->SetEntryFunction(entry)
	task->GeneratePathList();

}

TaskCreator::~TaskCreator()
{

}
*/










