///*
// * Flow.cpp
// *
// *  Created on: 2014-05-29
// *      Author: jtestard
// */
//
//
//#include "ConstantPropAnalysisFlow.h"
//
///**
// * For the Constant Propagation Analysis, just compare the two maps contained in the flow.
// */
//bool ConstantpropAnalysisFlow::equals(Flow& other) {
//
////	ConstantpropAnalysisFlow f = (ConstantpropAnalysisFlow)rhs;
////		this->basic = f.basic;
////		this->value = f.value;
//
//	ConstantpropAnalysisFlow othercp = (ConstantpropAnalysisFlow)other;
//
//	return this->value.size() == othercp.value.size()
//			&& std::equal(this->value.begin(), this->value.end(),
//							othercp.value.begin());
//
//
//	//return lhs.size() == rhs.size()
//	//        && std::equal(lhs.begin(), lhs.end(),
//	//                      rhs.begin());
//
//
////	PointerAnalysisFlow* other = (PointerAnalysisFlow*)otherSuper;
////	if (other->value.size()!=this->value.size())
////		return false;
////	for (map<string, vector<string> >::const_iterator itThis = this->value.begin(), itOther = other.value.begin() ;
////			itThis != this->value.end() ; itThis++, itOther++) {
////			string varThis = itThis->first;
////			string varOther = itOther->first;
////			vector<string> vecThis = itThis->second;
////			vector<string> vecOther = itOther->second;
////			if (vecThis.size()!=vecOther.size())
////				return false;
////			for (int i = 0 ; i < vecThis.size() ; i++) {
////				if (vecThis[i]!=vecOther[i])
////					return false;
////			}
////	}
//	return true;
//}
//
//string ConstantpropAnalysisFlow::jsonString(){
//	return "\"" + basic + "\"";
//}
//
//
//void ConstantpropAnalysisFlow::copy(Flow &rhs){
//	ConstantpropAnalysisFlow f = (ConstantpropAnalysisFlow)rhs;
//	this->basic = f.basic;
//	this->value = f.value;
//}
//
//ConstantpropAnalysisFlow::ConstantpropAnalysisFlow() : Flow() { }
//
//ConstantpropAnalysisFlow::ConstantpropAnalysisFlow(string input) : Flow(input){}
//
//ConstantpropAnalysisFlow::ConstantpropAnalysisFlow(Flow &flow) : Flow(flow.basic){}
//
//
////Most basic join operation possible.
//Flow ConstantpropAnalysisFlow::join(Flow &other){
//	//join bottom-bottom gives you bottom. Anything else gives you top.
//
//	ConstantpropAnalysisFlow othercp = (ConstantpropAnalysisFlow)other;
//	if (this->basic==BOTTOM && other.basic==BOTTOM)
//		return Flow(BOTTOM);
//	else if (this->basic == BOTTOM)
//		return othercp;
//	else if (othercp.basic == BOTTOM)
//		return (*this);
//	else if (this->basic==TOP | other.basic==TOP)
//		return Flow(TOP);
//	else {
//		//ConstantpropAnalysisFlow currF = new ConstantpropAnalysisFlow("Other");
//		//currF.value
//		//for (std::map<string, float>::iterator it = this->value.begin(); it!=this->value.begin();it++){
//			// do something!
//
//		return null;
//
//		}
//
//
//
//
//	}
//
//}
//
//ConstantpropAnalysisFlow::~ConstantpropAnalysisFlow(){
//	//Nothing for basic static analysis
//}
