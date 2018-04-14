#ifndef WFUNCTION
#define WFUNCTION

#include <list>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instruction.h>
#include <iostream>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include "wBasicBlock.hpp"
#include "Path.hpp"
#include "IRcodeData.hpp"


using namespace llvm;

typedef std::unique_ptr<Module> IRmodule; 

typedef std::list<llvm::GlobalVariable* > GlobalVariableList;
typedef std::list<llvm::GlobalVariable* > StaticVariableList;

typedef std::list<wBasicBlock* > PathTy;

class Path;
class IRcodeData;


class wFunction 
{

public:
	typedef enum {
		VoidTy,
		NonVoidTy
	}FuncRetTy;

private:
	Function *func;
	FuncRetTy retTy;
	std::list<wBasicBlock *> wBBList;
	wFunction *callerFunc;
	wBasicBlock *CurrentBasicBlock;
	std::list<std::list<wBasicBlock *> *> PathsList;
	std::list<Path *> PATHSLIST;

private:
	GlobalVariableList m_gvl;
	StaticVariableList m_svl;
	IRmodule* m_IRmodule; 

public:
	GlobalVariableList& getGlobalVariableList();
	StaticVariableList& getStaticVariableList();

	/* preprocessing */
	bool detectGlobalVariables();
	
	bool showGlobalVariable();
	bool showStaticVariable();
	bool FindStaticVariable();
	bool FindGlobalVariable();

private:
	IRcodeData* m_IRcodeData;

public:


	wFunction(IRmodule* , IRcodeData* );

	bool determineFuncRetTy();
	bool setFuncRetTy(wFunction::FuncRetTy);
	wFunction::FuncRetTy getFuncRetTy();

	/* wrapping function */
	bool setwFunction(Function *);
	Function *getFunction();

	wFunction *getCallerFunc();
	bool setCallerFunc(wFunction *);

	bool pushBBList(wBasicBlock *);
	wBasicBlock *getCurrentBasicBlock();

	bool setCurrentBasicBlock(wBasicBlock *);
	std::list<wBasicBlock *> *getwBBList();
	wBasicBlock* SearchBasicBlock(BasicBlock *);
	
	/* cfg Impl */
	std::list<std::list<wBasicBlock *> *> *getPathsList();
	bool makePathsList(wBasicBlock *);
	bool IntrinsicFuncMakePathsList(wBasicBlock *);
	unsigned int getNumPaths();
	bool showPathsList();

	/* Path */
	std::list<Path *> *makePATHSLIST();
	std::list<Path *> *getPATHSLIST();


};

#endif
