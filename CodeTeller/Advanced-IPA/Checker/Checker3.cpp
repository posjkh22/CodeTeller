
#include "checker.hpp"

bool Checker::check3(wBasicBlock *BB, wInstruction *Inst, GlobalVariable *gv){
	
	llvm::Instruction *bare_inst = Inst->getInstruction();

	if(bare_inst->getOpcodeName() == std::string("store")){
		

		for(unsigned int i = 0; i < bare_inst->getNumOperands(); i++){
			if(bare_inst->getOperand(i) == reinterpret_cast<Value *>(gv)){
			
				std::cout << i <<": ";
				std::cout << *(BB->getName()) << " ";
				std::cout << "[CheckF: " << bare_inst->getOpcodeName() << "]";
				std::cout << " " << gv->getName().str() << std::endl;

				std::cout << "size: " << bare_inst->getNumOperands() << std::endl;
				std::cout << "0: " << bare_inst->getOperand(0) << std::endl;

				Value *load_val = bare_inst->getOperand(0);
				llvm::Instruction *load_inst = reinterpret_cast<llvm::Instruction *>(load_val);
				std::cout << load_inst->getOpcodeName() << std::endl;			

			}
		}
	}
	return true;
}
