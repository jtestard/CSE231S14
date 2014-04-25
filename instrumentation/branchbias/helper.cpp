#include <stdio.h>
#include <string>
#include <map>

std::map<std::string,int> branchTakenMap;
std::map<std::string,int> branchFoundMap;
std::map<std::string,int> instructionMap;

void branchTaken(char* function) {
	std::string fn(function);
	if (branchTakenMap[fn]) {
		branchTakenMap[fn] += 1;
	} else {
		branchTakenMap[fn] = 0;
	}
	printf("function : %s",fn.c_str());
}

void branchFound(char * function) {
	std::string fn(function);
	if (branchFoundMap[fn]) {
		branchFoundMap[fn] += 1;
	} else {
		branchFoundMap[fn] = 0;
	}
	printf("function : %s",fn.c_str());
}

void print() {
//	for (std::map<std::string,int>::const_iterator it=branchFoundMap.begin() ; it!=branchFoundMap.end() ; it++) {
//		std::string fn(it->first());
//		printf("FunctionName\tBias\tTaken\tFound");
//		if (branchFoundMap[fn] && branchTakenMap[fn]) {
//			float f = ((float) (branchFoundMap[fn]) / (float) (branchTakenMap[fn]));
//			printf("%s\t%f\t%d\t%d",fn,f,branchFoundMap[fn],branchTakenMap[fn]);
//		}
//	}
//	for (std::map<std::string,int>::const_iterator it=instructionMap.begin() ; it!=instructionMap.end() ; it++) {
//		printf("%s\t\t%d\n",it->first.c_str(),it->second);
//	}
}

