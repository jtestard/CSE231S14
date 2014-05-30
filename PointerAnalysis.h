/*
 * PointerAnalysis.h
 *
 *  Created on: 2014-05-30
 *      Author: jtestard
 */

#ifndef POINTERANALYSIS_H_
#define POINTERANALYSIS_H_
#include "StaticAnalysis.h"

class PointerAnalysis : public StaticAnalysis {
	//Need to be filled in.
public:
	PointerAnalysis(Function &F);
	virtual ~PointerAnalysis();

protected:
	virtual void executeFlowFunction(Flow &in, Instruction &inst, Flow &out);
};

#endif /* POINTERANALYSIS_H_ */
