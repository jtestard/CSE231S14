
//
//  StaticAnalysisPass.cpp
//  
//
//  Created by Costas Zarifis on 22/05/2014.
//
//

/*
 * Note : use the errs() instead of std::cout in this file to output to the console (if your name is not mike and you don't have a fancy debugger that
 * took hours to install :).
 */
#include "ConstantPropAnalysis.h"

#define ADD 8 //This is the opcode for the add instruction
/*
 * For basic static analysis, flow is just "assigned to top", which just means the basic string from the Flow general class will be top.
 * This method is expected to do much more when overloaded.
 */
Flow* ConstantPropAnalysis::executeFlowFunction(Flow *in, Instruction* inst){
	//errs() << "Instruction Opcode : " << inst->getOpcode() << ", get name : " << inst->getOpcodeName() << "\n";
	ConstantPropAnalysisFlow* inFlow = static_cast<ConstantPropAnalysisFlow*>(in);
	ConstantPropAnalysisFlow * output;
	switch(inst->getOpcode()) {

	case ADD : //store instruction
		output = executeAddInst(inFlow,inst);
		break;
	default:
		output = new ConstantPropAnalysisFlow(inFlow);
		break;
	}
	//errs() << "Instruction : " << *inst << ", Flow value : " << output->jsonString() << "\n";
	return output;
}


ConstantPropAnalysisFlow* ConstantPropAnalysis::executeAddInst(ConstantPropAnalysisFlow* in, Instruction* instruction) {

	errs() << "Got in here? \n";
	ConstantPropAnalysisFlow* f = new ConstantPropAnalysisFlow(in);
	Value *leftOperand=instruction->getOperand(0);
	Value *rightOperand=instruction->getOperand(1);
	map<string, float >value;


	Value *K = instruction;
	string regName =  K->getName();
	errs() << "Instruction : " << regName << " left "<< leftOperand->getName()<<" right " << rightOperand->getName()<<"\n";

	// Checking if left operand is a constant
	if(ConstantInt *CILeft=dyn_cast<ConstantInt>(leftOperand)){
	        if(ConstantInt *CIRight=dyn_cast<ConstantInt>(rightOperand)){
	          int leftVal = CILeft->getZExtValue();
	          int rightVal = CIRight->getZExtValue();
	          int resVal = leftVal + rightVal;
	          ConstantPropAnalysisFlow* ff = new ConstantPropAnalysisFlow();
	          errs() << leftVal << " "<< rightVal << "\n";
	          errs() << "outcome: " << resVal<< "\n";
	          value[K->getName()] = resVal;
	          ff->value = value;
	          ConstantPropAnalysisFlow* tmp = static_cast<ConstantPropAnalysisFlow*>(ff->join(f));
	          delete ff;
	          delete f;
	          f = tmp;
	          //OutputE.setValue(K, LatticeValuePtr(new ConstPropLatticeValue(cvalue)));
	        }
	        else{
	          /*if(InputE.hasKey(secondOperand)){
				//LatticeValuePtr LVP = InputE.getContent(secondOperand);
	            //ConstPropLatticeValue CPL = cast<ConstPropLatticeValue>(*LVP) ;
	            int cvalue2 = CPL.getValue();
	            int cvalue1 = CI1->getZExtValue();
	            int cvalue = cvalue1 + cvalue2;

	            errs() << "something is wrong 1\n";
	            //OutputE.setValue(K, LatticeValuePtr(new ConstPropLatticeValue(cvalue)));
	          }
	          else break;
	          */

	        	errs() << "something is wrong 1\n";
	        }
	      }
	      else{
	        /*if(ConstantInt *CI2=dyn_cast<ConstantInt>(secondOperand)){
	          int cvalue2 = CI2->getZExtValue();
	          if(InputE.hasKey(firstOperand)){
				LatticeValuePtr LVP = InputE.getContent(firstOperand);
	            ConstPropLatticeValue CPL = cast<ConstPropLatticeValue>(*LVP);
	            int cvalue1 = CPL.getValue();
	            int cvalue = cvalue1 + cvalue2;
	            OutputE.setValue(K, LatticeValuePtr(new ConstPropLatticeValue(cvalue)));
	          }
	          else break;
	        }
	        else{
	          if(InputE.hasKey(secondOperand) && InputE.hasKey(firstOperand)){
				LatticeValuePtr LVP1 = InputE.getContent(firstOperand);
				LatticeValuePtr LVP2 = InputE.getContent(secondOperand);
	            ConstPropLatticeValue CPL1 = cast<ConstPropLatticeValue>(*LVP1);
	            ConstPropLatticeValue CPL2 = cast<ConstPropLatticeValue>(*LVP2);
	            int cvalue2 = CPL2.getValue();
	            int cvalue1 = CPL1.getValue();
	            int cvalue = cvalue1 + cvalue2;
	            OutputE.setValue(K, LatticeValuePtr(new ConstPropLatticeValue(cvalue)));
	          }
	          else break;

	        }
	      }
	      break;
	      */

	    	  errs() << "something is wrong 2\n";
	    }




	//StoreInst* store = static_cast<StoreInst*>(instruction);
	//Check if right operand is a pointer


	//if (store->getOperand(1)->getType()->isPointerTy()) {
		//Check if left & right operand names are non empty.
		//if (store->getOperand(0)->getName()!="" && store->getOperand(1)->getName()!="") {
			//ConstantPropAnalysisFlow* f = new ConstantPropAnalysisFlow();
			//set<string> s;

			//s.insert(store->getOperand(0)->getName());


			//value["b"] = 3.6;
			//f->value = value;
			//ConstantPropAnalysisFlow* ff = static_cast<ConstantPropAnalysisFlow*>(f->join(in));
			//delete f;
			//return ff;
		//}
	//}
	return f;
}

Flow* ConstantPropAnalysis::initialize(){
	return new ConstantPropAnalysisFlow(ConstantPropAnalysisFlow::BOTTOM);
}

ConstantPropAnalysis::ConstantPropAnalysis(Function &F) : StaticAnalysis() {
	this->top = new ConstantPropAnalysisFlow(ConstantPropAnalysisFlow::TOP);//Should be changed by subclasses of Flow to an instance of the subclass
	this->bottom = new ConstantPropAnalysisFlow(ConstantPropAnalysisFlow::BOTTOM);//Should be changed by subclasses of Flow to an instance of the subclass
	this->functionName = F.getName();
	buildCFG(F);
}
