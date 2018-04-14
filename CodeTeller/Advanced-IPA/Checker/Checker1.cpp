
#include "checker.hpp"

bool Checker::check1(wBasicBlock *BB, wInstruction *Inst){

	llvm::Instruction *bare_inst = Inst->getInstruction();

	if(bare_inst->getOpcodeName() == std::string("call")){
		std::cout << *(BB->getName()) << " ";
		std::cout << "[CheckF: " << bare_inst->getOpcodeName() << "]" << std::endl;
	}

	return true;
}
