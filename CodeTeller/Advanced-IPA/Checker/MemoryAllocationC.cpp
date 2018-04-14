
#include "checker.hpp"

bool Checker::Checker_MemoryAllocationC(wBasicBlock *BB, wInstruction *Inst){
	
	int arg = -1; 
	
	if(Inst->getInstruction()->getOpcodeName() == std::string("br")){	
		/* Do not consider "br" instruction */
		//std::cout << "BR" << std::endl;
		return true;
	}


	/* Before starting trace, if there is improper free instruction */
	if(Inst->getInstruction()->getOpcodeName() == std::string("call")){		
		
		llvm::Instruction *bare_inst = Inst->getInstruction();
		Value *get_opnd = bare_inst->getOperand(bare_inst->getNumOperands() - 1);			
		
		/* if free is found */
		if(get_opnd->getName().str() == std::string("free")){
#ifdef DEBUG_CHECKER				
			std::cout << "[First Free]: ";
			std::cout << get_opnd->getName().str() << std::endl;
#endif
		
			#define FFREE_FP_ARG 0
			
#ifdef DEBUG_CHECKER				
			std::cout << "PASSING: (";
			std::cout << bare_inst->getOperand(FFREE_FP_ARG);
			std::cout << ")" << std::endl;
#endif
			TraceData *returnIterPtr
				= SearchTraceVal(bare_inst->getOperand(FFREE_FP_ARG));			
			if(returnIterPtr == nullptr){
#ifdef DEBUG_CHECKER				
				std::cout << "Not allocated Memory is freed" << std::endl;
#endif					
				Value *new_traceVal = bare_inst->getOperand(FFREE_FP_ARG);
				llvm::Function *fid = BB->getParentFunction();	
				int *checker_state_flag = new int(-1);;		

				TraceData *new_trace_data
					= new TraceData(new_traceVal, checker_state_flag, fid);	

				new_trace_data->LocationBB = BB;
				new_trace_data->Location = bare_inst;
				new_trace_data->bug_location_flag = 1;
				new_trace_data->unique_number = -1;
				traceValState.push_back(new_trace_data);

			}
			else{
				/* blank */
			}

			#undef FFREE_FP_ARG
			
		}
		else{
			/* blank */
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////


	if(Inst->getInstruction()->getOpcodeName() == std::string("call")){		
		
		llvm::Instruction *bare_inst = Inst->getInstruction();
		
		Value *get_opnd = bare_inst->getOperand(bare_inst->getNumOperands() - 1);
		/* if malloc is found */
		if(get_opnd->getName().str() == std::string("malloc")){
			
			/* First malloc */
			if(traceValState.empty() == true){
				Value *traceVal = reinterpret_cast<llvm::Value *>(bare_inst);
				int *checker_state_flag = new int(1);	
				llvm::Function *fid = BB->getParentFunction();

				TraceData *trace_data
					= new TraceData(traceVal, checker_state_flag, fid);
			
				trace_data->setUniqueNumber(this);

				trace_data->LocationBB = BB;
				trace_data->Location = bare_inst;
				traceValState.push_back(trace_data);
				trace_flag = 1;
#ifdef DEBUG_CHECKER				
				std::cout << "[New malloc]";
				std::cout << trace_data->traceVal << std::endl;
				ShowTraceData();
#endif	
			}
			/* After the second malloc */
			else{
				Value *traceVal = reinterpret_cast<llvm::Value *>(bare_inst);
				
				/* not found & new insert */
				TraceData *returnIterPtr
					= SearchTraceVal(traceVal);

				if(nullptr == returnIterPtr){
					llvm::Function *fid = BB->getParentFunction();
					int *checker_state_flag = new int(1);	
					
					TraceData *trace_data
						= new TraceData(traceVal, checker_state_flag, fid);				
				
					trace_data->setUniqueNumber(this);
			
					trace_data->LocationBB = BB;
					trace_data->Location = bare_inst;
					traceValState.push_back(trace_data);
					trace_flag = 1;
#ifdef DEBUG_CHECKER				
					std::cout << "[New malloc]";
					std::cout << trace_data->traceVal << std::endl;
					ShowTraceData();					
#endif
				}
				
				else{
				
					/* If double open, */
					TraceData *get_trace_data = returnIterPtr;
					*(get_trace_data->checker_state_flag) += 1;
					get_trace_data->bug_location_flag = 1;

					//std::cout << "Double open: (";
					//std::cout << get_trace_data->traceVal << ")";
					//std::cout << std::endl;
					//trace_flag = 0;
				}

			}
		
		}
	}
	
	//if(traceVal != nullptr){
	if(traceValState.empty() == false){

#ifdef DEBUG_CHECKER				
		ShowTraceData();
#endif
	}


	/* if treace_flag == 1 */
	if(trace_flag == 1){
		
		llvm::Instruction *bare_inst = Inst->getInstruction();

		if(trace_new_func_flag == 1){
			
			if(PassingArgument.empty() == false){

				llvm::BasicBlock *bb = BB->getBasicBlock();
				llvm::Function *f = bb->getParent();	
#ifdef DEBUG_CHECKER				
				std::cout << "[Jump to " << f->getName().str() << "]" << std::endl;
#endif
				int tmp_count = 0;

				std::list<llvm::Argument *> temp_arg_list;
				llvm::Argument *argument;

#ifdef DEBUG_CHECKER				
				std::cout << "Func Argument size: (" << f->arg_size() << ") ";
				std::cout << "Passing  Argument size: (" << PassingArgument.size() << ")" << std::endl;
#endif
				for(auto iter = f->arg_begin(); 
						iter != f->arg_end(); iter++, tmp_count++){

					for(auto iter2 = PassingArgument.begin();
							iter2 != PassingArgument.end(); iter2++){

						arg = (*iter2)->number;				
						//arg = PassingArgument.front();
						//PassingArgument.pop_front();

						if(tmp_count == arg){
							argument = &(*iter);
							temp_arg_list.push_back(argument);
#ifdef DEBUG_CHECKER				
							std::cout << "Func Argument (" << argument << ")"  << std::endl;
#endif
						}
					}
				}

				/*
				   	Always
					tem_arg_list.size() >= PassingArgument.size()
	

				*/

				unsigned int tmp_count2 = 0;
				for(auto iter = temp_arg_list.begin();
						iter != temp_arg_list.end(); iter++, tmp_count2++){

					llvm::Argument *passArg = *iter;

					unsigned int passIter = PassingArgument.front()->number;
					
					if(tmp_count2 == passIter){

						//Checker::Argument *argu = PassingArgument.front();


						TraceData *returnIterPtr
							= SearchTraceVal(PassingArgument.front()->traceVal);
						
						//TraceData *returnIterPtr
						//	= SearchTraceVal(passIter);
						
						if(returnIterPtr == nullptr){
#ifdef DEBUG_CHECKER				
							std::cout << "Something Wrong!" << std::endl;
#endif
							PassingArgument.pop_front();
							continue;
						}
						else{


							PassingArgument.pop_front();
							
							int *checker_state_flag 
								= returnIterPtr->checker_state_flag;
							
							int unique_number 
								= returnIterPtr->unique_number;
							
							Value *new_traceVal	
								= reinterpret_cast<Value *>(passArg);
						
							llvm::Function *fid = BB->getParentFunction();

							TraceData *new_trace_data 
								= new TraceData(new_traceVal, checker_state_flag, fid, unique_number);
							new_trace_data->LocationBB = BB;
							new_trace_data->Location = bare_inst;
							traceValState.push_back(new_trace_data);
								
										
							//returnIterPtr->traceVal
							//	= reinterpret_cast<Value *>(passArg);
						}

					}
					
					else{
						continue;
					}

					//Argument *passArg = temp_arg_list.front();
					//temp_arg_list.pop_front();
					//traceVal = reinterpret_cast<Value *>(passArg);
				}
				
				trace_new_func_flag = 0;



			}
			/* No argument */
			else{

#ifdef DEBUG_CHECKER				
				std::cout << "Need to be considered" << std::endl;
#endif
				trace_new_func_flag = 0;
			}		

		}

		if(bare_inst->getOpcodeName() == std::string("ret")){		

			llvm::Function *fid = BB->getParentFunction();
#ifdef DEBUG_CHECKER				
			std::cout << "[RET from ";
			std::cout << fid->getName().str(); 
			std::cout << "] ";
#endif
			/* Pass ReturnVal */
			if(bare_inst->getNumOperands() != 0){

				/* return Not Void */

				#define RET_OPERAND_ITER 0
#ifdef DEBUG_CHECKER				
				std::cout << bare_inst->getOperand(RET_OPERAND_ITER) << std::endl;
#endif
				TraceData *returnIterPtr
					= SearchTraceVal(bare_inst->getOperand(RET_OPERAND_ITER));
			
				if(returnIterPtr == nullptr){
#ifdef DEBUG_CHECKER				
					std::cout <<"[No Pass] "<< std::endl;
#endif	
				}

				else{
				
					Value* new_traceVal = reinterpret_cast<Value *>(bare_inst);
					llvm::Function *fid = BB->getParentFunction();	
					int *checker_state_flag = returnIterPtr->checker_state_flag;		
					int unique_number = returnIterPtr->unique_number;		
					
#ifdef DEBUG_CHECKER				
					std::cout <<"[Pass] " << new_traceVal << std::endl;
#endif
					TraceData *trace_data
						= new TraceData(new_traceVal, checker_state_flag, fid, unique_number);	
					trace_data->LocationBB = BB;
					trace_data->Location = bare_inst;
					traceValState.push_back(trace_data);

					/* Transfer TraceData */
					/* 1. get PassingReturnVal element(TraceData1) & remove it from PassingReturnVal  */
					/* 2. get TraceValState last element(TraceData2) & remove it from TraceValState */
					/* 3. copy TraceData2 to TraceData1 */
					/* 4. Insert the updated TraceData1 to TraceValState */
					/* Delete PassingRetunVal element & insert TraceValState element */
					

#ifdef DEBUG_CHECKER				
					ShowPassingReturnVal();
#endif					
					wFunction *f = BB->getParent();
					if(f->getFuncRetTy() == wFunction::FuncRetTy::NonVoidTy){
						

						TraceData *new_trace_data 
							= PassingReturnVal.back();
						PassingReturnVal.pop_back();

						if(new_trace_data == nullptr){
							std::cout << "RET ERROR1" << std::endl;
						}

						TraceData *ret_trace_data
							= traceValState.back();
						if(ret_trace_data == nullptr){
							std::cout << "RET ERROR2" << std::endl;
						}

						traceValState.pop_back();
						
						new_trace_data->checker_state_flag
							= ret_trace_data->checker_state_flag;
						new_trace_data->bug_location_flag
							= ret_trace_data->bug_location_flag;
						new_trace_data->unique_number
							= ret_trace_data->unique_number;

						traceValState.push_back(new_trace_data);
					

					}
					else{
							std::cout << "RET ERROR3" << std::endl;
					}
					

				}
				#undef RET_OPERAND_ITER


			}

			else{
				/* return Void */
				

#ifdef DEBUG_CHECKER				
				std::cout << std::endl;
#endif
			}


			/* Detach tracer: main function or task function */
			//if(fid->getName().str() != "main"){

			if(fid->getName().str() == "main" 
					|| fid->getName().str() == "task1"					
					|| fid->getName().str() == "task2"					
				){

				setBugLocationFlag(fid);
			}
			else{
				
				deleteTraceValwithReturn(fid);
				/* blank */
			}

			return true;
		}
		else if(Inst->getInstruction()->getOpcodeName() == std::string("br")){	
			/* Do not consider "br" instruction */
#ifdef DEBUG_CHECKER				
			std::cout << "[BR]: SOMETHING WRONG!" << std::endl;
#endif
			return true;
		}

		else if(bare_inst->getOpcodeName() == std::string("call")){		
			Value *get_opnd = bare_inst->getOperand(bare_inst->getNumOperands() - 1);			
			/* if free is found */
			if(get_opnd->getName().str() == std::string("free")){
			
				#define FFREE_FP_ARG 0
				
#ifdef DEBUG_CHECKER				
				std::cout << "PASSING: (";
				std::cout << bare_inst->getOperand(FFREE_FP_ARG);
				std::cout << ")" << std::endl;
#endif
				TraceData *returnIterPtr
					= SearchTraceVal(bare_inst->getOperand(FFREE_FP_ARG));			
				if(returnIterPtr == nullptr){
#ifdef DEBUG_CHECKER				
					std::cout << "Not allocated Memory is freed" << std::endl;
#endif					
					Value *new_traceVal = bare_inst->getOperand(FFREE_FP_ARG);
					llvm::Function *fid = BB->getParentFunction();	
					int *checker_state_flag = new int(-1);;		

					TraceData *new_trace_data
						= new TraceData(new_traceVal, checker_state_flag, fid);

					new_trace_data->LocationBB = BB;
					new_trace_data->Location = bare_inst;
					new_trace_data->bug_location_flag = 1;
					new_trace_data->unique_number = -1;
					traceValState.push_back(new_trace_data);


				}
				else{
#ifdef DEBUG_CHECKER				
					std::cout << "[FREE]: ";
					std::cout << returnIterPtr->traceVal << std::endl;
#endif
					*(returnIterPtr->checker_state_flag) -= 1;
					
					if( *(returnIterPtr->checker_state_flag) < 0){
						returnIterPtr->bug_location_flag = 1;
					}
				}

				#undef FFREE_FP_ARG
				
			}
			/* if FILE ptr is an argument of a function */
			else{

#ifdef DEBUG_CHECKER				
				std::cout << "[call] ";
				std::cout << get_opnd->getName().str();
#endif		

				wFunction *fid = BB->getFuncCall();
				if(fid->getFuncRetTy() == wFunction::FuncRetTy::NonVoidTy){

#ifdef DEBUG_CHECKER				
					std::cout << "(NonVoidTy)" << std::endl;
#endif		

					Value *new_traceVal = reinterpret_cast<Value *>(bare_inst);
					llvm::Function *fid = BB->getParentFunction();	
					int *checker_state_flag = &basic_checker_state_flag;		

					TraceData *trace_data
						= new TraceData(new_traceVal, checker_state_flag, fid);
				
					trace_data->LocationBB = BB;
					trace_data->Location = bare_inst;
					PassingReturnVal.push_back(trace_data);
#ifdef DEBUG_CHECKER				
					ShowPassingReturnVal();
#endif
					
				}
				else {

#ifdef DEBUG_CHECKER				
					std::cout << "(VoidTy)" << std::endl;
#endif
					/* Blink */
				}


				/* arguemnt */
				for(unsigned int i = 0; i < bare_inst->getNumOperands() -1; i++){
				



					TraceData *returnIterPtr
						= SearchTraceVal(bare_inst->getOperand(unsigned(i)));
				
					if(returnIterPtr != nullptr){	
#ifdef DEBUG_CHECKER				
						std::cout << " Passing(before): ";
						std::cout << bare_inst->getOperand(unsigned(i));
						std::cout << std::endl;
#endif
						arg = i;
						
						Checker::Argument *argument = new Argument(arg, bare_inst->getOperand(unsigned(i)));
						PassingArgument.push_back(argument);
						trace_new_func_flag = 1;
					
					}
					else{
	
						continue;
					}
					
				}
			}
		}
		else if( bare_inst->getOpcodeName() == std::string("load") 
				|| bare_inst->getOpcodeName() == std::string("bitcast") 
			){


			//llvm::BasicBlock *bb = BB->getBasicBlock();
			//llvm::Function *f = bb->getParent();	
			
			#define LOADED_VALUE 0			
			TraceData *returnIterPtr
				= SearchTraceVal(bare_inst->getOperand(LOADED_VALUE));

			if(returnIterPtr == nullptr){
#ifdef DEBUG_CHECKER				
				std::cout << "[" << f->getName().str() << "]";
				std::cout << "[load/bitcast] No Pass " << bare_inst->getOpcodeName() << std::endl;
#endif
			}

			else{
				Value* new_traceVal = reinterpret_cast<Value *>(bare_inst);
				llvm::Function *fid = BB->getParentFunction();	
				int *checker_state_flag = returnIterPtr->checker_state_flag;		
				int unique_number = returnIterPtr->unique_number;

				TraceData *trace_data
					= new TraceData(new_traceVal, checker_state_flag, fid, unique_number);
				trace_data->LocationBB = BB;
				trace_data->Location = bare_inst;
				traceValState.push_back(trace_data);

#ifdef DEBUG_CHECKER				
				std::cout << "[" << f->getName().str() << "]";
				std::cout << "[load/bitcast]: ";
				std::cout << bare_inst->getOperand(LOADED_VALUE) << " to " << new_traceVal << std::endl;
#endif
			}
			#undef LOADED_VALUE	

		}

		else if(bare_inst->getOpcodeName() == std::string("store")){	
			//llvm::BasicBlock *bb = BB->getBasicBlock();
			//llvm::Function *f = bb->getParent();	
			
			#define STORED_VALUE 0
			#define STORAGE 1
		
			TraceData *returnIterPtr
				= SearchTraceVal(bare_inst->getOperand(STORED_VALUE));
			
			if(returnIterPtr == nullptr){
#ifdef DEBUG_CHECKER				
				std::cout << "[" << f->getName().str() << "]";
				std::cout <<"[store] No Pass " << bare_inst->getOpcodeName() << std::endl;
#endif
			}

			else{
				
				Value* new_traceVal = bare_inst->getOperand(STORAGE);
				llvm::Function *fid = BB->getParentFunction();	
				int *checker_state_flag = returnIterPtr->checker_state_flag;		
				int unique_number = returnIterPtr->unique_number;

				TraceData *trace_data
					= new TraceData(new_traceVal, checker_state_flag, fid, unique_number);
				trace_data->LocationBB = BB;
				trace_data->Location = bare_inst;
				traceValState.push_back(trace_data);


				/*
				TraceData *get_trace_data = returnIterPtr;
				
				get_trace_data->traceVal
						= bare_inst->getOperand(STORAGE);
				*/
		
#ifdef DEBUG_CHECKER				
				std::cout << "[" << f->getName().str() << "]";
				std::cout << "[store]: ";
				std::cout << bare_inst->getOperand(STORED_VALUE) << " to " << new_traceVal << std::endl;
#endif
			}
			
			#undef STORED_VALUE
			#undef STORAGE


		}else{

				//llvm::BasicBlock *bb = BB->getBasicBlock();
				//llvm::Function *f = bb->getParent();	
#ifdef DEBUG_CHECKER				
				std::cout << "[" << f->getName().str() << "]";
				std::cout <<"[Other instructions] " << bare_inst->getOpcodeName() << std::endl;
#endif
		}


	}

	else{

		/* blank */
	}


	return true;
}

