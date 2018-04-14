
#include "checker.hpp"

bool Checker::Checker_BasicSemaphoreIntegrity(wBasicBlock *BB, wInstruction *Inst)
{
	
	int arg = -1; 
	

	if(Inst->getInstruction()->getOpcodeName() == std::string("br")){	
		/* Do not consider "br" instruction */
		return true;
	}

	/* Unlock before Lock */
	if(Inst->getInstruction()->getOpcodeName() == std::string("call"))
	{		
		
		llvm::Instruction *bare_inst = Inst->getInstruction();
		Value *get_opnd = bare_inst->getOperand(bare_inst->getNumOperands() - 1);			
		
		/* if free is found */
		if(get_opnd->getName().str() == std::string("pthread_mutex_unlock")){
			
			#define SEM_VAL 0
			
			Value* SemVal = bare_inst->getOperand(SEM_VAL);
		
			TraceData* returnIterPtr
				= SearchTraceVal(SemVal);

			if(returnIterPtr == nullptr)
			{

				Value* new_traceVal = SemVal;
				llvm::Function* fid = BB->getParentFunction();
				int* checker_state_flag = new int(-1);

				TraceData* new_trace_data
					= new TraceData(new_traceVal, checker_state_flag, fid);

				new_trace_data->LocationBB = BB;
				new_trace_data->Location = bare_inst;
				new_trace_data->bug_location_flag = 1;
				new_trace_data->unique_number = -1;
				traceValState.push_back(new_trace_data);
				
				std::cout << "UnLock without Lock: " << SemVal->getName().str() << std::endl;
				ShowTraceData();

			}

			else
			{
				/* emppty */
			}

		}

		/* if not pthread_mutex_nulock */
		else 
		{
			/* empty */
		}
	}

	/* Before starting trace, if there is improper free instruction */
	if(Inst->getInstruction()->getOpcodeName() == std::string("call"))
	{		
		
		llvm::Instruction *bare_inst = Inst->getInstruction();
		Value *get_opnd = bare_inst->getOperand(bare_inst->getNumOperands() - 1);			
		
		/* if free is found */
		if(get_opnd->getName().str() == std::string("pthread_mutex_lock")){
			
#ifdef BASICSEM_DEBUG
			std::cout << "Found: ";
			std::cout << get_opnd->getName().str() << std::endl;
#endif
			#define SEM_VAL 0
			
			Value* SemVal = bare_inst->getOperand(SEM_VAL);

#ifdef BASICSEM_DEBUG
			std::cout << "SemVal: ";
			std::cout << bare_inst->getOperand(SEM_VAL)->getName().str();
			std::cout << std::endl;
#endif
			/* First TraceVal */
			if(traceValState.empty() == true)
			{
				//Value* traceVal = reinterpret_cast<llvm::Value* >(bare_inst);
				Value* traceVal = reinterpret_cast<llvm::Value* >(SemVal);
				int *checker_state_flag = new int(1);
				llvm::Function *fid = BB->getParentFunction();
		
				TraceData* trace_data
					= new TraceData(traceVal, checker_state_flag, fid);

				trace_data->setUniqueNumber(this);
				trace_data->LocationBB = BB;
				trace_data->Location = bare_inst;
				traceValState.push_back(trace_data);
				trace_flag = 1;

				std::cout << "Lock1: " << SemVal->getName().str() << std::endl;
				ShowTraceData();
			}

			else
			{
				//Value* traceVal = reinterpret_cast<llvm::Value* >(bare_inst);
				Value* traceVal = reinterpret_cast<llvm::Value* >(SemVal);

				TraceData* returnIterPtr
					= SearchTraceVal(traceVal);

				if(nullptr == returnIterPtr)
				{

					llvm::Function* fid = BB->getParentFunction();
					int* checker_state_flag = new int(1);

					TraceData* trace_data
						= new TraceData(traceVal, checker_state_flag, fid);


					trace_data->setUniqueNumber(this);
					trace_data->LocationBB = BB;
					trace_data->Location = bare_inst;
					traceValState.push_back(trace_data);
					trace_flag = 1;

					std::cout << "Lock2: " << SemVal->getName().str() << std::endl;
					ShowTraceData();

				}
		
				/* double Lock */
				else
				{
					TraceData* get_trace_data = returnIterPtr;
					*(get_trace_data->checker_state_flag) += 1;
					get_trace_data->bug_location_flag = 1;

					std::cout << "Double Lock2: " << SemVal->getName().str() << std::endl;
					ShowTraceData();
				}

			}

			#undef SEM_VAL
		}

	}

	else 
	{
		/* if not sem_lock */
		/* empty */
	}

	if(trace_flag == 0)
	{
		llvm::Instruction* bare_inst = Inst->getInstruction();

		/* Before starting trace, if there is improper free instruction */
		if(Inst->getInstruction()->getOpcodeName() == std::string("call"))
		{		
			
			llvm::Instruction *bare_inst = Inst->getInstruction();
			Value *get_opnd 
				= bare_inst->getOperand(bare_inst->getNumOperands() - 1);			
			
			/* if free is found */
			if(get_opnd->getName().str() == std::string("pthread_mutex_unlock"))
			{
		
				#define SEM_VAL 0
			
				llvm::Value* SemVal = bare_inst->getOperand(SEM_VAL);
				Value* traceVal = reinterpret_cast<llvm::Value* >(SemVal);
				
				TraceData* returnIterPtr
					= SearchTraceVal(traceVal);
				
				/* if not locked SemVal */
				
				*(returnIterPtr->checker_state_flag) -= 1;
				
				std::cout << "UnLock1: " << SemVal->getName().str() << std::endl;
				ShowTraceData();
				
			
				/* Double Unlock */
				if( *(returnIterPtr->checker_state_flag) < 0)
				{
					returnIterPtr->LocationBB = BB;
					returnIterPtr->Location = bare_inst;
					returnIterPtr->bug_location_flag = 1;
				}



				#undef SEM_VAL
			

			}
		}

		/* Function Retrun Value does not need to be considered */
		else if(bare_inst->getOpcodeName() == std::string("ret"))
		{
		
			llvm::Function* fid = BB->getParentFunction();

			if(fid->getName().str() == "main"
					|| fid->getName().str() == "task1"
					|| fid->getName().str() == "task2"
			  )
			{
				setBugLocationFlag(fid);
				std::cout << "[Return phase]" << std::endl;
				ShowTraceData();

				if(p_SRL->empty())
				{
					/* empty */
				}
				else
				{
					std::cout << "Not protected SharedResources: ";
					for(auto iter1 = p_SRL->begin();
							iter1 != p_SRL->end(); iter1++)
					{
						llvm::Value* currentVar = *iter1;
						std::cout <<" " <<  currentVar->getName().str();
					}
					std::cout << std::endl;
				}
			}
			else
			{
				/* empty*/
				/* No demand for deleteTraceValwithReturn(fid) */
				/* SemVal is all global Value */
			}
		}
		
		else
		{
			/* empty */
		}
		

	}


	if(trace_flag >= 1)
	{

		llvm::Instruction* bare_inst = Inst->getInstruction();

		/* trace_new_func_flag does not need to be considered */
		if(trace_new_func_flag == 1)
		{
			/* empty */
		}
		else
		{
			/* empty */
		}
	
		/* Function Retrun Value does not need to be considered */
		if(bare_inst->getOpcodeName() == std::string("ret"))
		{
		
			llvm::Function* fid = BB->getParentFunction();

			if(fid->getName().str() == "main"
					|| fid->getName().str() == "task1"
					|| fid->getName().str() == "task2"
			  )
			{
				setBugLocationFlag(fid);
				std::cout << "[Return phase]" << std::endl;
				ShowTraceData();

				if(p_SRL->empty())
				{
					/* empty */
				}
				else
				{
					std::cout << "Not protected SharedResources: ";
					for(auto iter1 = p_SRL->begin();
							iter1 != p_SRL->end(); iter1++)
					{
						llvm::Value* currentVar = *iter1;
						std::cout <<" " <<  currentVar->getName().str();
					}
					std::cout << std::endl;
				}
			}
			else
			{
				/* empty*/
				/* No demand for deleteTraceValwithReturn(fid) */
				/* SemVal is all global Value */
			}
		}
		else if(bare_inst->getOpcodeName() == std::string("br"))
		{
			/* empty */
		}

	
		/* Before starting trace, if there is improper free instruction */
		else if(Inst->getInstruction()->getOpcodeName() == std::string("call"))
		{		
			
			llvm::Instruction *bare_inst = Inst->getInstruction();
			Value *get_opnd 
				= bare_inst->getOperand(bare_inst->getNumOperands() - 1);			
			
			/* if free is found */
			if(get_opnd->getName().str() == std::string("pthread_mutex_unlock")){
#ifdef BASICSEM_DEBUG
				std::cout << "Found: ";
				std::cout << get_opnd->getName().str() << std::endl;
#endif			
				#define SEM_VAL 0
				
#ifdef BASICSEM_DEBUG
				std::cout << "SemVal: ";
				std::cout << bare_inst->getOperand(SEM_VAL)->getName().str();
				std::cout << std::endl;
#endif
			
				llvm::Value* SemVal = bare_inst->getOperand(SEM_VAL);
				Value* traceVal = reinterpret_cast<llvm::Value* >(SemVal);
				
				TraceData* returnIterPtr
					= SearchTraceVal(traceVal);
			

				/* if not locked SemVal */
				if(returnIterPtr == nullptr)
				{
					Value* new_traceVal = bare_inst->getOperand(SEM_VAL);
					llvm::Function* fid = BB->getParentFunction();
					int* checker_state_flag = new int(-1);

					TraceData* new_trace_data
						= new TraceData(new_traceVal, checker_state_flag, fid);

					new_trace_data->LocationBB = BB;
					new_trace_data->Location = bare_inst;
					new_trace_data->bug_location_flag = 1;
					new_trace_data->unique_number = -1;
					traceValState.push_back(new_trace_data);
	
					std::cout << "UnLock2: " << SemVal->getName().str() << std::endl;
					ShowTraceData();
				}

				else
				{
					/* Proper Unlock Case */
					*(returnIterPtr->checker_state_flag) -= 1;
					
					std::cout << "UnLock1: " << SemVal->getName().str() << std::endl;
					ShowTraceData();
					trace_flag -= 1;
				
					/* Double Unlock */
					if( *(returnIterPtr->checker_state_flag) < 0)
					{
						returnIterPtr->LocationBB = BB;
						returnIterPtr->Location = bare_inst;
						returnIterPtr->bug_location_flag = 1;
					}


				}

				#undef SEM_VAL
			}

			/* if unlock is not found */
			/* other function is not considered */
			/* basically, semVal is rarely used for function argument */
			/* load/store(value passing function) is also not considered */
			else
			{
				/* empty */
			}

		}

		else if( bare_inst->getOpcodeName() == std::string("load") 
				|| bare_inst->getOpcodeName() == std::string("bitcast") 

			   )
		{

			#define LOADED_VALUE 0
			Value* sharedVar = bare_inst->getOperand(LOADED_VALUE);			

			for(auto iter1 = p_SRL->begin();
					iter1 != p_SRL->end(); )
			{
				Value* SharedResource = *iter1;
				if(sharedVar == SharedResource)
				{
					p_SRL->erase(iter1++);		
				}
				else
				{
					iter1++;
				}
			}

		}
		
		else if(bare_inst->getOpcodeName() == std::string("store"))
		{		
			#define STORED_VALUE 0
			#define STORAGE 1
		
			Value* sharedVar = bare_inst->getOperand(STORAGE);
			
			for(auto iter1 = p_SRL->begin();
					iter1 != p_SRL->end(); )
			{
				Value* SharedResource = *iter1;
				if(sharedVar == SharedResource)
				{
					p_SRL->erase(iter1++);		
				}
				else
				{
					iter1++;
				}
			}

		}	
	}

	/* if trace_flag < 0 */
	/*
	
	else if(trace_flag == 0)
	{
		if(Inst->getInstruction()->getOpcodeName() == std::string("call"))
		{		
			
			llvm::Instruction *bare_inst = Inst->getInstruction();
			Value *get_opnd 
				= bare_inst->getOperand(bare_inst->getNumOperands() - 1);			
			
			if(get_opnd->getName().str() == std::string("pthread_mutex_unlock")){
		
		
				#define SEM_VAL 0	
				llvm::Value* SemVal = bare_inst->getOperand(SEM_VAL);
				Value* traceVal = reinterpret_cast<llvm::Value* >(SemVal);
				
				TraceData* returnIterPtr
					= SearchTraceVal(traceVal);
		
				*(returnIterPtr->checker_state_flag) -= 1;	
				std::cout << "Double UnLock: " << SemVal->getName().str() << std::endl;
				ShowTraceData();
				trace_flag -= 1;
			
				if( *(returnIterPtr->checker_state_flag) < 0)
				{
					returnIterPtr->LocationBB = BB;
					returnIterPtr->Location = bare_inst;
					returnIterPtr->bug_location_flag = 1;
				}
				#undef SEM_VAL

			}
		}

	}
	*/
	return true;
}
