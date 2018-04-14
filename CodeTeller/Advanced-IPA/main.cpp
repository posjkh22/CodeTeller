#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <utility>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Bitcode/BitcodeReader.h>

#include "IRcodeData.hpp"
#include "IRcodeTextDataSet.hpp"
#include "wFunction.hpp"
#include "wBasicBlock.hpp"
#include "wInstruction.hpp"
#include "Support.hpp"
#include "Path.hpp"
#include "PathImpl.hpp"
#include "checker.hpp"
#include "ArgumentPass.hpp"
#include "TaskManager.hpp"
#include "BugReport.hpp"
#include "SymbolManager.hpp"
#include "Visualize.hpp"

/* 2018-01-25 */

/*

    [solved] problem: GPL
	[solved] problem: split;
	[solved] problem: delete argument (function) 

	problem: C code linking
	probelm: Intrincs (llvm.memcpy)
	problem: other ptr(return or argument) case: malloc & free
	problem: real malloc/ real free

	return -> function ret type -> if ret something, pusch argulist, (stack) get, update.

	unique number:

	problem: C lang is squential. -> revserse iterator  or delete inserted value

*/


const char OSJSON[]="./Json/OperatingSystemList.json";
const char TFUNCJSON[]="./Json/TargetFunctionList.json";

using namespace llvm;

/* To easily pass varaibles */
GlobalVariable *comp_gv;

//std::unique_ptr<Module> m; 

int main(int argc, char *argv[]) {


	/* Symbol Manager */

	IPA::SymbolManager sm(new IPA::PathUncountedFunction,
				new IPA::ThreadSupportOS);
	
	sm.Process(OSJSON, TFUNCJSON);

	sm.DEBUG_showPUFL();

	/* Process Argument */
	IPA::ArgumentPass arg;
	arg.processArguPass(argc, argv);
	arg.showArgument();

	std::cout << "--------------Argu analysis done----------------" << std::endl;

	/* IRParser */
	StringRef filename = argv[argc-1];
	LLVMContext context;
	SMDiagnostic error;
	std::unique_ptr<Module> m = parseIRFile(filename, error, context);

	
	std::cout << "-------------IR Parse analysis done-------------" << std::endl;

	/* Wrapper */
	IRcodeData IRcode(m, sm);

	/* Global Variable */

	//IRcode.ShowGlobalVariables(m);

	//comp_gv = IRcode.popGlobalVariable(m, 0);

	/* Function */
	IRcode.Preprocess1();
	IRcode.Preprocess2();
	IRcode.Preprocess3();
	
	IRcode.Preprocess0();
	
	std::cout << "---------------Pre-analysis done----------------" << std::endl;
	
/*
	IRcode.DebugWrapperModule();	
*/

/*	
	IRcode.ShowPathsInEachFunc();
*/

	/* TaskManager */
	IPA::TaskManager tm(&arg, &IRcode);
	tm.Process();
	
	std::cout << "---------------Task-analysis done---------------" << std::endl;

	/* Analysis Manager */
	/*
	IPA::AnalysisManager am(&tm);
	am.run();
	*/

	/* Graph */
	IPA::Visualize v1(
			new IPA::Graphviz(), 
			"./dat/SharedResourcesList.dat",
			"./dat/TaskSharedResources.dot");
	v1.Setup();
	v1.run();		

	IPA::Visualize v2(
			new IPA::Graphviz(), 
			"./dat/TaskFunctionList.dat",
			"./dat/TaskFunction.dot");
	v2.Setup();
	v2.run();		
	
	std::cout << "------------------Visualize done----------------" << std::endl;
	
	for(unsigned int i = 0; i < tm.getTaskNum(); i++)
	{

		IPA::Task* t = tm.getTask();	
		
		PathListTy* gpl = t->getPathList();

		PathList m_PathList(gpl);
		//m_PathList.ShowPathList();
	
	
		IPA::BugReport brp;

		Checker checker(&brp, &arg, t);
		checker.CheckerRunsOnPathList(&m_PathList);
		
	}


	return 0;
}
