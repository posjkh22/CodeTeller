
	bool check1(wBasicBlock *BB, wInstruction *Inst);
	bool check2(wBasicBlock *BB, wInstruction *Inst, GlobalVariable *gv);
	bool check3(wBasicBlock *BB, wInstruction *Inst, GlobalVariable *gv);
	bool check4(wBasicBlock *BB, wInstruction *Inst);
	bool Checker_FilePointerAnalysisC(wBasicBlock *BB, wInstruction *Inst);
	bool Checker_MemoryAllocationC(wBasicBlock *BB, wInstruction *Inst);
	bool Checker_BasicSemaphoreIntegrity(wBasicBlock *BB, wInstruction *Inst);
