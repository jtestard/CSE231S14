//
//  StaticAnalysisPass.cpp
//  
//
//  Created by Jules Testard on 22/05/2014.
//
//

#include "StaticAnalysisPass.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include <map>
#include <string>

using namespace llvm;
using namespace std;

namespace {
    // This implementation is common to all
    class StaticAnalysis : public ModulePass {
        
        StaticAnalysisPass() : ModulePass(ID) {}
        
        virtual void runWorklist(M);
        
        virtual void runWorklist(Module &M) {
            //here goes the worklist aglorithm
        }
    }

char StaticAnalysisPass::ID = 0;

