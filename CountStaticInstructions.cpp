

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include <map>
#include <string>

using namespace llvm;
using namespace std;

namespace {
  struct StaticInstructionCount : public ModulePass {
    static char ID;
    StaticInstructionCount() : ModulePass(ID) {}

    map<string,int> instructionMap;

    virtual bool runOnModule(Module &M) {
    	for (Module::iterator m = M.begin(), e = M.end() ; e != m ; ++m) {
			for (inst_iterator I = inst_begin(m), E = inst_end(m) ; I != E ; ++I) {
					string opcode = I->getOpcodeName();
					if (instructionMap[opcode]) {
						instructionMap[opcode]++;
					} else {
						instructionMap[opcode] = 1;
					}
				}
			}
    	return false;
    }

    void print(raw_ostream &OS, const Module*) const {
    	int total = 0;
    	for (map<string,int>::const_iterator it=instructionMap.begin() ; it!=instructionMap.end() ; it++) {
    		OS << it->first << "\t\t" << it->second << "\n";
    		total += it->second;
    	}
    	OS << "TOTAL\t\t" << total << "\n";
  	}
  };
}

char StaticInstructionCount::ID = 0;
static RegisterPass<StaticInstructionCount> X("staticInstructionCount", "StaticInstructionCount Pass", false, false);
