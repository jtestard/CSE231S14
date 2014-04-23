#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include <map>
#include <string>

using namespace llvm;
using namespace std;

namespace {
  struct DynamicInstructionCount : public ModulePass {
    static char ID;
    DynamicInstructionCount() : ModulePass(ID) {}

    map<string,int> instructionMap;

    virtual bool runOnModule(Module &M) {
    	for (Module::iterator m = M.begin(), e = M.end() ; e != m ; ++m) {
    			errs() << *m << "\n";
//			for (inst_iterator I = inst_begin(m), E = inst_end(m) ; I != E ; ++I) {
//					errs() << *I << "\n";
//				}
			}
    	return false;
    }

  };
}

char DynamicInstructionCount::ID = 0;
static RegisterPass<DynamicInstructionCount> X("dynamicInstructionCount", "DynamicInstructionCount Pass", false, false);
