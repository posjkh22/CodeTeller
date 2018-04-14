
#include "checker.hpp"

bool Checker::check2(wBasicBlock *BB, wInstruction *Inst, GlobalVariable *gv){
	
	llvm::Instruction *bare_inst = Inst->getInstruction();

	if(bare_inst->getOpcodeName() == std::string("store")){
		

		for(unsigned int i = 0; i < bare_inst->getNumOperands(); i++){
			if(bare_inst->getOperand(i) == reinterpret_cast<Value *>(gv)){
			
				std::cout << i <<": ";
				std::cout << *(BB->getName()) << " ";
				std::cout << "[CheckF: " << bare_inst->getOpcodeName() << "]";
				std::cout << " " << gv->getName().str() << std::endl;
			}
		}
	}
	
	return true;
}
