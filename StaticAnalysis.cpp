//
//  StaticAnalysisPass.cpp
//  
//
//  Created by Jules Testard on 22/05/2014.
//
//

#include "StaticAnalysis.hpp"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include <map>
#include <string>

using namespace llvm;
using namespace std;

//class StaticAnalysis {
//    virtual void runWorklist(Module &M){
//        //Worklist algorithm code
//    }
//    virtual void flowFunction(Instruction &inst) {
//        // Flow Inset = inst.getSet();
//        // if (inst.opcode == 'blah') {
//        //      flowFunctionBlah()
//        // } else if ...
//        //
//    }
//}
