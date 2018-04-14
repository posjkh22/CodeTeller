
#include "checker.hpp"


//#define DEBUG_CHECKER				

Checker::Checker(
		IPA::BugReport *bugReport, 
		IPA::ArgumentPass *argument,
		IPA::Task* task)
{
	trace_flag = -1;
	trace_new_func_flag = 0;
	basic_checker_state_flag = 0;
	reset_flag = 1;

	p_SRL = new ValueListTy(task->m_SRL);
	
	//checker_state_flag = 0;
	//traceVal = nullptr;
	m_BugReport = bugReport;
	m_ArgumentPass = argument;
	p_Task = task;

	checkerTyDetermination();
};


Checker::Checker(PathList *pathlist){

	if(attachPathList(pathlist)){
		std::cout << "Checker(PathList) create! " << std::endl;
	}
}

Checker::Checker(Path *path){

	if(attachPath(path)){
		std::cout << "Checker(Path) create! " << std::endl;
	}

}



bool Checker::setCheckerTy(Checker::checker_ty ty){
	m_CheckerType = ty;
	return true;
}

Checker::checker_ty Checker::getCheckerTy()
{
	return m_CheckerType;
}


bool Checker::attachPathList(PathList *pathlist){


	setCurrentPathList(pathlist);

	Path *entry_path = pathlist->getPathList()->front();

	setCurrentPath(entry_path);

	wBasicBlock *entry_BB = entry_path->getPath()->front();

	setCurrentBB(entry_BB);

	/*
	   More specific to Target
	*/

	return true;
}

bool Checker::setCurrentPathList(PathList *pathlist){

	currentPathList = pathlist;

	return true;
}


PathList *Checker::getCurrentPathList(){

	return currentPathList;
}


/* 
   	In this function, 
	Implement movePath at the end of PathList
	is Not considered(also return false).

*/
bool Checker::movePath(){

	PathList *currentPathList = getCurrentPathList();

	std::list<Path *>::iterator iter;
	std::list<Path *> *CurrentPathList = currentPathList->getPathList();

	int found_flag = 0;

	for(iter = CurrentPathList->begin(); 
			iter != CurrentPathList->end(); iter++){

		Path *currentPath = (*iter);

		/* if found, break when next iter */
		if(currentPath == getCurrentPath()){

			found_flag = 1;
			iter++;
			break;
		}
	}

	/* No found */
	if(!found_flag){
		return false;
	}

	/* update currentPath */
	return setCurrentPath(*iter);
}

bool Checker::setCheckerState(){

	reset_flag = 1;
	return true;
}

bool Checker::ResetCheckerState(){


	if(ResetCheckerVariable() == true
		&& ResetTraceValState() == true
		&& ResetPassingReturnVal() == true
		&& ResetPassingArgument() == true){

		return true;
	}
	else{

		return false;
	}
}


bool Checker::ResetCheckerVariable(){

	trace_flag = 0;
	trace_new_func_flag = 0;
	reset_flag = 1;	

	return true;
}


bool Checker::ResetTraceValState(){

	if(traceValState.empty() == true){

		return true;
	}
	else{

		int num = traceValState.size();
		for(int i = 0 ; i < num; i++){
			traceValState.pop_front();
		}

		return true;
	}

}
	
bool Checker::ResetPassingReturnVal(){
	
	if(PassingReturnVal.empty() == true){

		return true;
	}
	else{

		int num = PassingReturnVal.size();
		for(int i = 0 ; i < num; i++){
			PassingReturnVal.pop_front();
		}

		return true;
	}

}

bool Checker::ResetPassingArgument(){
	
	if(PassingArgument.empty() == true){

		return true;
	}
	else{

		int num = PassingArgument.size();
		for(int i = 0 ; i < num; i++){
			PassingArgument.pop_front();
		}

		return true;
	}

}


/* Path */

bool Checker::attachPath(Path *path){

	setCurrentPath(path);
	
	wBasicBlock *entry_BB = path->getPath()->front();

	setCurrentBB(entry_BB);
	
	/*

	More specific to Target

	*/

	return true;
}


bool Checker::setCurrentPath(Path *path){

	currentPath = path;

	return true;
}


Path *Checker::getCurrentPath(){

	return currentPath;
}

bool Checker::move(){

	Path *currentPath = getCurrentPath();
	
	std::list<wBasicBlock *> *CurrentPath = currentPath->getPath();

	std::list<wBasicBlock *>::iterator iter;
	
	int found_flag = 0;

	for(iter = CurrentPath->begin(); 
			iter != CurrentPath->end(); iter++){

		wBasicBlock *currentBB = (*iter);

		if(currentBB == getCurrentBB()){
			found_flag = 1;
			iter++;
			break;
		}
	}

	if(!found_flag){
		return false;
	}

	return setCurrentBB(*iter);
}


/* BB */

wBasicBlock *Checker::getCurrentBB(){

	return currentBB;
}

bool Checker::setCurrentBB(wBasicBlock *BB){
	
	currentBB = BB;
	return true;
}

wBasicBlock *Checker::getNextBB(){

	Path *currentPath = getCurrentPath();
	
	std::list<wBasicBlock *> *CurrentPath = currentPath->getPath();

	std::list<wBasicBlock *>::iterator iter;
	
	int found_flag = 0;

	for(iter = CurrentPath->begin(); 
			iter != CurrentPath->end(); iter++){

		wBasicBlock *currentBB = (*iter);

		if(currentBB == getCurrentBB()){
			found_flag = 1;
			iter++;
			break;
		}
	}

	if(!found_flag){
		return nullptr;
	}

	return (*iter);

}





unsigned int Checker::check_checker_state_flag(){

	unsigned int sumAbsolValue = 0;

	for(auto iter = traceValState.begin();
			iter != traceValState.end(); iter++){

		TraceData *current = (*iter);

		sumAbsolValue += std::abs(*(current->checker_state_flag));
	}

	return sumAbsolValue;

}



bool Checker::BugReport(){

	/* trace_flag check */
	/* if the value is not set, the error is detected */

	std::ofstream fout;
	fout.open("./dat/FoundErrors.dat", std::ofstream::out | std::ofstream::app);

	if(check_checker_state_flag() != 0 
			&& ((getCheckerTy() == Checker::checker_ty::FilePointerAnalysisC) 
				|| (getCheckerTy() == Checker::checker_ty::MemoryAllocationC)
				|| (getCheckerTy() == Checker::checker_ty::SemaphoreIntegrity))){


		std::cout << "[Checker: Error Detected]" << std::endl;
		fout << "[Checker: Error Detected]" << std::endl;

		for(auto iter = traceValState.begin();
				iter != traceValState.end(); iter++){

			TraceData *current = (*iter);
			
			if(*(current->checker_state_flag) < 0 
					&& current->bug_location_flag == 1){
				
				if(getCheckerTy() == Checker::checker_ty::FilePointerAnalysisC){
					std::cout << " - FILE Double close";
					fout << " - FILE Double close";
				}
				else if(getCheckerTy() == Checker::checker_ty::MemoryAllocationC){
					std::cout << " - Improper free";
					fout << " - Improper free";
				}
				else if(getCheckerTy() == Checker::checker_ty::SemaphoreIntegrity){
					std::cout << " - Improper unlock";
					fout << " - Improper unlock";
				}
				std::cout << "(" << current->traceVal << "/";
				std::cout << *(current->checker_state_flag) << "/";
				std::cout << current->f->getName().str() << "/";
				std::cout << current->bug_location_flag << "/";
				std::cout << current->unique_number << ")"; 
				std::cout << std::endl;
				
				fout << "(" << current->traceVal << "/";
				fout << *(current->checker_state_flag) << "/";
				fout << current->f->getName().str() << "/";
				fout << current->bug_location_flag << "/";
				fout << current->unique_number << ")"; 
				fout << std::endl;
				
				
				m_BugReport->AddBugLocation(current->Location);
				m_BugReport->showBugLocation(current->Location, fout);
				
				fout << " - BasicBlock: " << *(current->LocationBB->getName());
				fout << std::endl << std::endl;
			}

			/*
			if(*(current->checker_state_flag) > 0 
				|| ( (*(current->checker_state_flag) > 0 
					&& current->bug_location_flag == 1) )
			*/

			if(( (*(current->checker_state_flag) == 1 
					&& current->bug_location_flag == 1) )
				
				){

				if(getCheckerTy() == Checker::checker_ty::FilePointerAnalysisC){
					std::cout << " - FILE is Not close";
					fout << " - FILE is Not close";
				}
				else if(getCheckerTy() == Checker::checker_ty::MemoryAllocationC){
					std::cout << " - Allocated Memory is Not freed";
					fout << " - Allocated Memory is Not freed";
				}
				else if(getCheckerTy() == Checker::checker_ty::SemaphoreIntegrity){
					std::cout << " - Not unlock";
					fout << " - Not unlock";
				}
				std::cout << "(" << current->traceVal << "/";
				std::cout << *(current->checker_state_flag) << "/";
				std::cout << current->f->getName().str() << "/";
				std::cout << current->bug_location_flag << "/";
				std::cout << current->unique_number;
				std::cout << ")";
				std::cout << std::endl;
				
				fout << "(" << current->traceVal << "/";
				fout << *(current->checker_state_flag) << "/";
				fout << current->f->getName().str() << "/";
				fout << current->bug_location_flag << "/";
				fout << current->unique_number;
				fout << ")";
				fout << std::endl;
				
				m_BugReport->AddBugLocation(current->Location);	
				m_BugReport->showBugLocation(current->Location, fout);

				fout << " - BasicBlock: " << *(current->LocationBB->getName());
				fout << std::endl << std::endl;
			}

			if(( (*(current->checker_state_flag) > 1 
					&& current->bug_location_flag == 1) )
				
				){

				if(getCheckerTy() == Checker::checker_ty::SemaphoreIntegrity){
					std::cout << " - Double lock";
					fout << " - Double lock";
				}
				std::cout << "(" << current->traceVal << "/";
				std::cout << *(current->checker_state_flag) << "/";
				std::cout << current->f->getName().str() << "/";
				std::cout << current->bug_location_flag << "/";
				std::cout << current->unique_number;
				std::cout << ")";
				std::cout << std::endl;
				
				fout << "(" << current->traceVal << "/";
				fout << *(current->checker_state_flag) << "/";
				fout << current->f->getName().str() << "/";
				fout << current->bug_location_flag << "/";
				fout << current->unique_number;
				fout << ")";
				fout << std::endl;
				
				m_BugReport->AddBugLocation(current->Location);	
				m_BugReport->showBugLocation(current->Location, fout);

				fout << " - BasicBlock: " << *(current->LocationBB->getName());
				fout << std::endl << std::endl;
			}

			
		}

	}
	/* If error is not found */
	else{
		std::cout << "[No Error Code was Found] " << std::endl;
		fout << "[No Error Code was Found] " << std::endl;
	}

	fout.close();

	return true;
}


bool Checker::CheckerRunsOnPathList(PathList *pathlist){


	attachPathList(pathlist);

	PathList *targetPathlist = getCurrentPathList();

	std::list<Path *>::iterator iter;
	std::list<Path *> *TargetPathList;

	TargetPathList = targetPathlist->getPathList();

	for(iter = TargetPathList->begin();
			iter != TargetPathList->end(); iter++){

		Path* currentPath = (*iter);
		CheckerRunsOnPath(currentPath);
	}

	return true;
}

bool Checker::CheckerRunsOnPath(Path *path){

	std::ofstream fout;
	fout.open("./dat/FoundErrors.dat", std::ofstream::out | std::ofstream::app);
	
	std::cout << std::endl;
	std::cout << "[Checker is running on Path ";
	std::cout << *(path->getName());
	std::cout << "..]" << std::endl;
	
	fout << "[Checker is running on Path ";
	fout << *(path->getName());
	fout << "..]" << std::endl;

	fout.close();

	/* Initialize CheckerState */
	setCheckerState();

	attachPath(path);

	Path *targetPath = getCurrentPath();

	std::list<wBasicBlock *>::iterator iter;
	std::list<wBasicBlock *> *TargetPath;

	TargetPath = targetPath->getPath();

	for(iter = TargetPath->begin();
			iter != TargetPath->end(); iter++){

		wBasicBlock *currentBB = (*iter);
		CheckerRunsOnBasicBlock(currentBB);	
	}
	
	std::cout << "[Path ";
	std::cout << *(currentPath->getName());
	std::cout << "]";

	BugReport();
		
	/* Reset Checker State */	
	if(ResetCheckerState() == true){

#ifdef DEBUG_CHECKER				
		std::cout << "Checker state is reset" << std::endl;
#endif
	}
	else{
#ifdef DEBUG_CHECKER				
		std::cout << "[ERROR] Checker state is not reset" << std::endl;
#endif
	}
	

	return true;
}


bool Checker::CheckerRunsOnBasicBlock(wBasicBlock *BB){

	std::list<wInstruction *>::iterator iter;
	std::list<wInstruction *> *InstList;


	InstList = BB->getwInstList();

	for(iter = InstList->begin();
			iter != InstList->end(); iter++){

		wInstruction *currentInst = (*iter);

		/* checker set */
		//check1(BB, currentInst);
		//check2(BB, currentInst, comp_gv);
		//check3(BB, currentInst, comp_gv);
		//setCheckerTy(Checker::checker_ty::FilePointerAnalysisC);
		//check4(BB, currentInst); 
		//setCheckerTy(Checker::checker_ty::MemoryAllocationC);
		
		if(getCheckerTy() == checker_ty::CHECK1)
		{
			check1(BB, currentInst);
		}
		else if(getCheckerTy() == checker_ty::CHECK2)
		{
			check2(BB, currentInst, comp_gv);
		}
		else if(getCheckerTy() == checker_ty::CHECK3)
		{
			check3(BB, currentInst, comp_gv);
		}
		else if(getCheckerTy() == checker_ty::CHECK4)
		{
			check4(BB, currentInst); 
		}
		else if(getCheckerTy() == checker_ty::FilePointerAnalysisC)
		{
			Checker_FilePointerAnalysisC(BB, currentInst);
		}
		else if(getCheckerTy() == checker_ty::MemoryAllocationC)
		{
			Checker_MemoryAllocationC(BB, currentInst); 
		}
		else if(getCheckerTy() == checker_ty::SemaphoreIntegrity)
		{
			Checker_BasicSemaphoreIntegrity(BB, currentInst);
		}
		else
		{
			std::cerr << "Undefined Checker" << std::endl;
		}
		
	}

	return true;
}



bool Checker::checkerTyDetermination()
{
	if(m_ArgumentPass->getArgument()->getAnalysisTy() 
			== IPA::Argument::analysisTy::MemoryAllocationC)
	{
		setCheckerTy(Checker::checker_ty::MemoryAllocationC);	
	}
	else if(m_ArgumentPass->getArgument()->getAnalysisTy() 
			== IPA::Argument::analysisTy::FilePointerAnalysisC)
	{
		setCheckerTy(Checker::checker_ty::FilePointerAnalysisC);	
	}
	else if(m_ArgumentPass->getArgument()->getAnalysisTy() 
			== IPA::Argument::analysisTy::Deadlock)
	{
		setCheckerTy(Checker::checker_ty::Deadlock);	
	}
	else if(m_ArgumentPass->getArgument()->getAnalysisTy() 
			== IPA::Argument::analysisTy::SemaphoreIntegrity)
	{
		setCheckerTy(Checker::checker_ty::SemaphoreIntegrity);	
	}
	else if(m_ArgumentPass->getArgument()->getAnalysisTy() 
			== IPA::Argument::analysisTy::SharedVariables)
	{
		setCheckerTy(Checker::checker_ty::SharedVariables);	
	}
	else if(m_ArgumentPass->getArgument()->getAnalysisTy() 
			== IPA::Argument::analysisTy::SharedFunctions)
	{
		setCheckerTy(Checker::checker_ty::SharedFunctions);	
	}
	else 
	{
		setCheckerTy(Checker::checker_ty::UnknownCheckerTy);	
		return false;
	}

	return true;
}


/* checker set */





Checker::TraceData *Checker::SearchTraceVal(unsigned int iter_count){

	unsigned int count = 0;
	int found_flag = 0; 
	
	std::list<Checker::TraceData *>::iterator iter;

	for(iter = traceValState.begin();
			iter != traceValState.end(); iter++, count++){

		if(count == iter_count){
			found_flag = 1;
			break;
		}

	}

	if(found_flag == 0){

		return nullptr;
	}

	return (*iter);

}

/* Backward search: C lang is run sequentially. */
Checker::TraceData *Checker::SearchTraceVal(llvm::Value *cmp_val){

	std::list<TraceData *>::reverse_iterator iter;

	int found_flag = 0;

	for(iter = traceValState.rbegin(); 
			iter !=traceValState.rend(); iter++){

		if( (*iter)->traceVal == cmp_val){
			found_flag = 1;
			break;
		}

	}

	if(found_flag == 0){
		return nullptr;
	}

	return (*iter);
}


bool Checker::ShowTraceData(){

	int i = 1;
	for(auto iter = traceValState.begin();
			iter != traceValState.end(); iter++, i++){

		TraceData *current = *iter;

		std::cout << "[" << std::setw(2) << i << "] ";
		std::cout << "(" << current->traceVal << "/";
		std::cout << std::setw(2) << *(current->checker_state_flag) <<"/";
		std::cout << current->f->getName().str() <<  "/"; 
		std::cout << current->bug_location_flag << "/";
		std::cout << current->unique_number << ") ";

		if(i != 0 && i %3 == 0){
			std::cout << std::endl;
		}

	}
	std::cout << std::endl;
	return true;
}

bool Checker::ShowPassingReturnVal(){

	int i = 1;
	for(auto iter = PassingReturnVal.begin();
			iter != PassingReturnVal.end(); iter++, i++){

		TraceData *current = *iter;

		std::cout << "ShowPassingReturnVal" << std::endl;
		std::cout << "[" << std::setw(2) << i << "] ";
		std::cout << "(" << current->traceVal << "/";
		std::cout << std::setw(2) << *(current->checker_state_flag) <<"/";
		std::cout << current->f->getName().str() <<  "/"; 
		std::cout << current->bug_location_flag << ") ";

		if(i != 0 && i %3 == 0){
			std::cout << std::endl;
		}

	}
	std::cout << std::endl;
	return true;
}

bool Checker::deleteTraceValwithReturn(llvm::Function *fid){

	
	std::list<TraceData *>::iterator iter;

	iter = traceValState.begin();
	while(iter != traceValState.end()){
		
		TraceData *current = (*iter);
		/* 
		   	if checker_state_flag is Not normal, 
		 	leave the traceVal.
		*/
		if(current->f == fid  
			&& !(*(current->checker_state_flag) > 0
				|| *(current->checker_state_flag) < 0)		
				
			){
#ifdef DEBUG_CHECKER				
			std::cout << "[TraceData DEL]: ";
			std::cout << "(" << current->traceVal << "/";
			std::cout << *(current->checker_state_flag) <<"/";
			std::cout << current->f->getName().str() << ")";
			std::cout << std::endl;
#endif
			traceValState.erase(iter++);
		}
		else{
			++iter;
		}
		
	}
	return true;
}

bool Checker::setBugLocationFlag(llvm::Function *fid){

	
	std::list<TraceData *>::iterator iter;

	iter = traceValState.begin();
	while(iter != traceValState.end()){
		
		TraceData *current = (*iter);
		/* 
		   	if checker_state_flag is Not normal, 
		 	leave the traceVal.
		*/
		if(current->f == fid  
			&& (*(current->checker_state_flag) > 0)
//			&& (*(current->checker_state_flag) > 0
//				|| *(current->checker_state_flag) < 0)		
				
			){
			current->bug_location_flag = 1;
#ifdef DEBUG_CHECKER				
			std::cout << "[SetBugLocationFlag]: ";
			std::cout << "(" << current->traceVal << "/";
			std::cout << *(current->checker_state_flag) <<"/";
			std::cout << current->f->getName().str() << ")";
			std::cout << std::endl;
#endif
		}
		
		++iter;
	}
	return true;
}


bool Checker::check4(wBasicBlock *BB, wInstruction *Inst){
	

	return true;
}


