//https://sites.google.com/site/arnamoyswebsite/Welcome/updates-news/llvmpasstoinsertexternalfunctioncalltothebitcode
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"

using namespace llvm;
using namespace std;

namespace {
  struct DynamicInstructionCount : public ModulePass {
    static char ID;
    llvm::Function* hook;		//Required that you have a llvm Function to call

    DynamicInstructionCount() : ModulePass(ID) {hook = NULL;}

    virtual bool runOnModule(Module &M){
                Constant *hookFunc;
//                for (Module::global_iterator S = M.global_begin(), E = M.global_end() ; S != E ; ++S ) {
//                	errs() << "Variable : " << *S << "\n";
//                }
//                for (Module::named_metadata_iterator nmdi = M.named_metadata_begin(), nmde = M.named_metadata_end() ; nmdi != nmde ; ++nmdi) {
//                	errs() << "Named Metadata : " << nmdi << "\n";
//                }
//                for (Module::iterator F = M.begin(), E = M.end(); F!= E; ++F) {
//                	errs() << "Function : " << *F << "\n";
//                }
//                for (Module::alias_iterator ai = M.alias_begin(), ae = M.alias_end() ; ai != ae; ++ai) {
//                	errs() << "Alias" << *ai << "\n";
//                }
                hookFunc = M.getFunction("_Z5printv");
                if (hookFunc) {
					hook= cast<Function>(hookFunc);
					for(Module::iterator F = M.begin(), E = M.end(); F!= E; ++F) {
						for(Function::iterator BB = F->begin(), E = F->end(); BB != E; ++BB) {
							DynamicInstructionCount::runOnBasicBlock(BB);
						}
					}
                }
                return false;
            }

    virtual bool runOnBasicBlock(Function::iterator &BB) {
                for(BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE; ++BI) {
                	Instruction *CI = dyn_cast<Instruction>(BI);
                	Instruction *newInst = CallInst::Create(hook, "");
                	BB->getInstList().insert(CI, newInst);
                }
                return true;
    }
  };
}

char DynamicInstructionCount::ID = 0;
static RegisterPass<DynamicInstructionCount> X("dynamicInstructionCount", "DynamicInstructionCount World Pass", false, false);
