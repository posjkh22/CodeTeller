OUTPUT 			= code_analysis
SRC				=./src
LIB				=./lib
INCLUDE			=./include
TESTCASE		= ./TestCases
TESTCLASS 		= testcase8
TEST			= testcase8-1
TARGET_OPTION 	= --target=Linux
THREAD_OPTION1	= --thread=Unithread
THREAD_OPTION2	= --thread=Multithread
ANALYSIS_OPTION	= --analysis=MemoryAllocationC

test:
	make -C ./TestCases

run: all
	./$(OUTPUT) $(TESTCASE)/$(TESTCLASS)/$(TEST).bc $(TARGET_OPTION) $(THREAD_OPTION2) $(ANALYSIS_OPTION)

run2: 
	./$(OUTPUT) $(TESTCASE)/$(TESTCLASS)/$(TEST).bc $(TARGET_OPTION) $(THREAD_OPTION2) $(ANALYSIS_OPTION)


link: 
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
	-o $(OUTPUT) \
	-ljsoncpp \
	`llvm-config --cxxflags --libs --ldflags --system-libs`

all: clean main Support ArgumentPass SymbolManager TaskManager Task IRcodeData IRcodeTextDataSet wFunction wBasicBlock wInstruction path pathImpl checker BugReport
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

clean:
	rm -f $(OUTPUT) test_out $(LIB)/*.o Analysis*

result_clean:
	rm -f Analysis*

