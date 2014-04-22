

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include <map>
#include <string>

using namespace llvm;
using namespace std;

namespace {
  struct StaticInstructionCount : public FunctionPass {
    static char ID;

    StaticInstructionCount() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
    	map<string,int> instructionMap;
    	for (inst_iterator I = inst_begin(F), E = inst_end(F) ; I != E ; ++I) {
    		string opcode = I->getOpcodeName();
    		if (instructionMap[opcode]) {
    			instructionMap[opcode]++;
    		} else {
    			instructionMap[opcode] = 1;
    		}
    	}
    	int total = 0;
    	for (map<string,int>::iterator it=instructionMap.begin() ; it!=instructionMap.end() ; it++) {
    		errs() << it->first << "\t" << it->second << "\n";
    		total += it->second;
    	}
    	errs() << "TOTAL\t" << total << "\n";
    	return false;
    }
  };
}

char StaticInstructionCount::ID = 0;
static RegisterPass<StaticInstructionCount> X("staticInstructionCount", "StaticInstructionCount World Pass", false, false);
