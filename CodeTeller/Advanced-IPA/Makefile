OUTPUT 			= code_analysis
SRC				=./src
CHECKER_SRC		=./Checker
CHECKER_LIB		=./lib
LIB				=./lib
INCLUDE			=./include
TESTCASE		= ./TestCases
TESTCLASS 		= testcase9
TEST			= testcase9-1
TARGET_OPTION 	= --target=Linux
THREAD_OPTION1	= --thread=Unithread
THREAD_OPTION2	= --thread=Multithread
ANALYSIS_OPTION0	= --analysis=FilePointerAnalysisC
ANALYSIS_OPTION1	= --analysis=MemoryAllocationC
ANALYSIS_OPTION2	= --analysis=SemaphoreIntegrity

test:
	make -C ./TestCases

run: all
	./$(OUTPUT) $(TESTCASE)/$(TESTCLASS)/$(TEST).bc $(TARGET_OPTION) $(THREAD_OPTION1) $(ANALYSIS_OPTIOIN2)

run2: 
	./$(OUTPUT) $(TESTCASE)/$(TESTCLASS)/$(TEST).bc $(TARGET_OPTION) $(THREAD_OPTION1) $(ANALYSIS_OPTION2)


R1:
	./$(OUTPUT) $(TESTCASE)/testcase4/testcase4-4.bc $(TARGET_OPTION) $(THREAD_OPTION1) $(ANALYSIS_OPTION0)

R2:
	./$(OUTPUT) $(TESTCASE)/testcase8/testcase8-1.bc $(TARGET_OPTION) $(THREAD_OPTION2) $(ANALYSIS_OPTION1)

R3:
	./$(OUTPUT) $(TESTCASE)/testcase9/testcase9-2.bc $(TARGET_OPTION) $(THREAD_OPTION2) $(ANALYSIS_OPTION2)


CheckerList: c1 c2 c3 \
		FilePointerAnalysisC \
		MemoryAllocationC \
		Semaphore 

c1:
	clang++ -std=c++11 \
	-c $(CHECKER_SRC)/Checker1.cpp \
	-o $(LIB)/Checker1.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`
c2:
	clang++ -std=c++11 \
	-c $(CHECKER_SRC)/Checker2.cpp \
	-o $(LIB)/Checker2.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`
c3:
	clang++ -std=c++11 \
	-c $(CHECKER_SRC)/Checker3.cpp \
	-o $(LIB)/Checker3.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`
FilePointerAnalysisC:	
	clang++ -std=c++11 \
	-c $(CHECKER_SRC)/FilePointerAnalysisC.cpp \
	-o $(LIB)/FilePointerAnalysisC.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`
MemoryAllocationC:	
	clang++ -std=c++11 \
	-c $(CHECKER_SRC)/MemoryAllocationC.cpp \
	-o $(LIB)/MemoryAllocationC.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`
Semaphore:	
	clang++ -std=c++11 \
	-c $(CHECKER_SRC)/Semaphore.cpp \
	-o $(LIB)/Semaphore.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`



	

link: result_clean
	clang++ -std=c++11 \
	$(LIB)/main.o \
	$(LIB)/IRcodeData.o \
	$(LIB)/ArgumentPass.o \
	$(LIB)/SymbolManager.o \
	$(LIB)/TaskManager.o \
	$(LIB)/Task.o \
	$(LIB)/IRcodeTextDataSet.o \
	$(LIB)/wFunction.o \
	$(LIB)/wBasicBlock.o \
	$(LIB)/wInstruction.o \
	$(LIB)/Support.o \
	$(LIB)/Path.o \
	$(LIB)/PathImpl.o \
	$(LIB)/checker.o \
	$(LIB)/BugReport.o \
	$(LIB)/Visualize.o \
	$(CHECKER_LIB)/FilePointerAnalysisC.o \
	$(CHECKER_LIB)/MemoryAllocationC.o \
	$(CHECKER_LIB)/Semaphore.o \
	$(CHECKER_LIB)/Checker1.o \
	$(CHECKER_LIB)/Checker2.o \
	$(CHECKER_LIB)/Checker3.o \
	-o $(OUTPUT) \
	-ljsoncpp \
	`llvm-config --cxxflags --libs --ldflags --system-libs`

all: clean \
		main \
		Support \
		ArgumentPass \
		SymbolManager \
		TaskManager \
		Task \
		IRcodeData \
		IRcodeTextDataSet \
		wFunction \
		wBasicBlock \
		wInstruction \
		path \
		pathImpl \
		checker \
		BugReport \
		Visualize \
		CheckerList 
	clang++ -std=c++11 \
	$(LIB)/main.o \
	$(LIB)/IRcodeData.o \
	$(LIB)/ArgumentPass.o \
	$(LIB)/SymbolManager.o \
	$(LIB)/TaskManager.o \
	$(LIB)/Task.o \
	$(LIB)/IRcodeTextDataSet.o \
	$(LIB)/wFunction.o \
	$(LIB)/wBasicBlock.o \
	$(LIB)/wInstruction.o \
	$(LIB)/Support.o \
	$(LIB)/Path.o \
	$(LIB)/PathImpl.o \
	$(LIB)/checker.o \
	$(LIB)/BugReport.o \
	$(LIB)/Visualize.o \
	$(CHECKER_LIB)/MemoryAllocationC.o \
	$(CHECKER_LIB)/Semaphore.o \
	$(CHECKER_LIB)/Checker1.o \
	$(CHECKER_LIB)/Checker2.o \
	$(CHECKER_LIB)/Checker3.o \
	-o $(OUTPUT) \
	-ljsoncpp \
	`llvm-config --cxxflags --libs --ldflags --system-libs`
	
main:
	clang++ -std=c++11 -c main.cpp  \
	-o $(LIB)/main.o \
	`llvm-config --cxxflags` \
	-I$(INCLUDE)

Support:
	clang++ -std=c++11 \
	-c $(SRC)/Support.cpp \
	-o $(LIB)/Support.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`

ArgumentPass:
	clang++ -std=c++11 \
	-c $(SRC)/ArgumentPass.cpp \
	-o $(LIB)/ArgumentPass.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`

SymbolManager:
	clang++ -std=c++11 \
	-c $(SRC)/SymbolManager.cpp \
	-o $(LIB)/SymbolManager.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`

Task:
	clang++ -std=c++11 \
	-c $(SRC)/Task.cpp \
	-o $(LIB)/Task.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`

TaskManager:
	clang++ -std=c++11 \
	-c $(SRC)/TaskManager.cpp \
	-o $(LIB)/TaskManager.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`

IRcodeData:
	clang++ -std=c++11 \
	-c $(SRC)/IRcodeData.cpp \
	-o $(LIB)/IRcodeData.o \
	`llvm-config --cxxflags` \
	-I$(INCLUDE)

IRcodeTextDataSet:
	clang++ -std=c++11 \
	-c $(SRC)/IRcodeTextDataSet.cpp \
	-o $(LIB)/IRcodeTextDataSet.o \
	`llvm-config --cxxflags` \
	-I$(INCLUDE)

wFunction:
	clang++ -std=c++11 \
	-c $(SRC)/wFunction.cpp \
	-o $(LIB)/wFunction.o \
	`llvm-config --cxxflags` \
	-I$(INCLUDE)
	
wBasicBlock:
	clang++ -std=c++11 \
	-c $(SRC)/wBasicBlock.cpp \
	-o $(LIB)/wBasicBlock.o \
	`llvm-config --cxxflags` \
	-I$(INCLUDE)

wInstruction:
	clang++ -std=c++11 \
	-c $(SRC)/wInstruction.cpp \
	-o $(LIB)/wInstruction.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`

path:
	clang++ -std=c++11 \
	-c $(SRC)/Path.cpp \
	-o $(LIB)/Path.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`

pathImpl:
	clang++ -std=c++11 \
	-c $(SRC)/PathImpl.cpp \
	-o $(LIB)/PathImpl.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`

checker:
	clang++ -std=c++11 \
	-c $(SRC)/checker.cpp \
	-o $(LIB)/checker.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`

BugReport:
	clang++ -std=c++11 \
	-c $(SRC)/BugReport.cpp \
	-o $(LIB)/BugReport.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`

Visualize:
	clang++ -std=c++11 \
	-c $(SRC)/Visualize.cpp \
	-o $(LIB)/Visualize.o \
	-I$(INCLUDE) \
	`llvm-config --cxxflags`
	
clean:
	rm -f $(OUTPUT) test_out $(LIB)/*.o Analysis*

result_clean:
	rm -f Analysis* ./dat/*

