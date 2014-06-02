//#include "llvm/Pass.h"
//#include "llvm/IR/Module.h"
//#include "llvm/Support/raw_ostream.h"
//#include "llvm/Support/InstIterator.h"
//#include "llvm/Analysis/ConstantFolding.h"
//
//#include <map>
//#include <set>
//#include <string>
//#include <sstream>
//
//
//using namespace llvm;
//using namespace std;
//
//namespace {
//  struct ConstantPropagation : public FunctionPass {
//    static char ID;
//    ConstantPropagation() :  FunctionPass(ID) {}
//
//    map<string,int> instructionMap;
//    int NumInstKilled ;
//    std::stringstream ss;
//    virtual bool runOnModule(Module &M) {
//    	NumInstKilled = 0;
//    	for (Module::iterator m = M.begin(), e = M.end() ; e != m ; ++m) {
//
//			for (inst_iterator I = inst_begin(m), E = inst_end(m) ; I != E ; ++I) {
//
//					string opcode = I->getOpcodeName();
//					if (instructionMap[opcode]) {
//						instructionMap[opcode]++;
//					} else {
//						instructionMap[opcode] = 1;
//					}
//				}
//			}
//    	return false;
//    }
//
//    virtual bool runOnFunction(Function &F) {
//    	  std::set<Instruction*> WorkList;
//    	  for(inst_iterator i = inst_begin(F), e = inst_end(F); i != e; ++i) {
//    	      WorkList.insert(&*i);
//    	  }
//    	  bool Changed = false;
//    	  //ss<<" this is a ss: "<<"yay"<<" \n";
//    	  while (!WorkList.empty()) {
//    	    Instruction *I = *WorkList.begin();
//    	    WorkList.erase(WorkList.begin());    // Get an element from the worklist...
//
//    	    //errs()<<" command: "<< *I<<" \n";
//    	    std::string str;
//    	    raw_string_ostream rso(str);
//    	    I->print(rso);
//    	    ss<<" command: "<< str<<" \n";
//
//    	    //if (!I->use_empty())                 // Don't muck with dead instructions...
//    	      if (Constant *C = ConstantFoldInstruction(I)) {
//    	    	errs()<<"has constant \n";
//    	    	ss<<"\thas constant \n";
//    	        // Add all of the users of this instruction to the worklist, they might
//    	        // be constant propagatable now...
//    	        for (Value::use_iterator UI = I->use_begin(), UE = I->use_end();
//    	             UI != UE; ++UI){
//    	          WorkList.insert(cast<Instruction>(*UI));
//    	          errs()<<"Inserting: "<<cast<Instruction>(*UI)<< "\n";
//    	          ss<<"\tInserting: "<<cast<Instruction>(*UI)<< "\n";
//
//    	        }
//    	        // Replace all of the uses of a variable with uses of the constant.
//    	        I->replaceAllUsesWith(C);
//
//    	        // Remove the dead instruction.
//    	        WorkList.erase(I);
//    	        I->eraseFromParent();
//
//    	        // We made a change to the function...
//    	        Changed = true;
//    	        ++NumInstKilled;
//    	      }
//    	  }
//    	  return Changed;
//    }
//
//
//    void print(raw_ostream &OS, const Module*) const {
//    	//int total = 0;
//    	//for (map<string,int>::const_iterator it=instructionMap.begin() ; it!=instructionMap.end() ; it++) {
//    	//	OS << it->first << "\t\t" << it->second << "\n";
//    	//	total += it->second;
//    	//}
//    	//OS << "TOTAL\t\t" << total << "\n";
//    	OS<<ss.str()<<" \n";
//  	}
//  };
//}
//
//char ConstantPropagation::ID = 0;
//static RegisterPass<ConstantPropagation> X("ConstantPropagation", "ConstantPropagation Pass", false, false);
