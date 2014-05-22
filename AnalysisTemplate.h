#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include <map>
#include <string>

using namespace llvm;
using namespace std;

namespace {
  struct StaticAnalysisPass : public ModulePass {
    static char ID;
    StaticAnalysisPass() : ModulePass(ID) {}
    virtual bool runOnModule(Module &M) ();
    void print(raw_ostream &OS, const Module*) const ();
}

char StaticAnalysisPass::ID = 0;
