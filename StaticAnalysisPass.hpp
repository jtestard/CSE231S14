// Every analysis file must extend the StaticAnalysisPass class.

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"

using namespace llvm;
using namespace std;

namespace {
  struct StaticAnalysisPass : public ModulePass {
      static char ID;
      virtual static Flow
      StaticAnalysisPass() : ModulePass(ID) {}
      virtual bool runOnModule(Module &M) ();
      virtual void runWorklist(Module &M);
      
      /*
       * Metadata for the input to the flow function is included in the instruction object.
       *
       */
      virtual void flowFunction(Instruction &inst);
      virtual void join(BasicBlock &b1,BasicBlock &b2);
      virtual void split();
      //Flow output for each instruction should be printable in JSON. This function
      //should on be called by the print(raw_ostream &OS, const Module*) from LLVM.
      virtual void printJSON();
    //void print(raw_ostream &OS, const Module*) const ();
}
