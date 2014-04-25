#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include <map>
#include <string>
#define HELPER_FUNCTIONS 160

using namespace llvm;
using namespace std;

namespace {
  struct DumpContent : public ModulePass {
    static char ID;
    DumpContent() : ModulePass(ID) {}

    map<string,int> instructionMap;

    virtual bool runOnModule(Module &M) {
		Module::iterator F = M.begin();
		for (int i = 0 ; i < HELPER_FUNCTIONS ; i++)
			F++;
        for (Module::iterator E = M.end(); F!= E; ++F) {
        	errs() << "Function : " << F->getName() << "\n";
        }
		Module::iterator m = M.begin();
		for (int i = 0 ; i < HELPER_FUNCTIONS ; i++)
			m++;
    	for (Module::iterator e = M.end() ; e != m ; ++m) {
			for (inst_iterator I = inst_begin(m), E = inst_end(m) ; I != E ; ++I) {
				errs() << *I << "\n";
			}
		}
    	return false;
    }
  };
}

char DumpContent::ID = 0;
static RegisterPass<DumpContent> X("dumpContent", "DumpContent Pass", false, false);
