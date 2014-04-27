#include <stdio.h>
#include <string>
#include <iostream>
#include <map>

using namespace std;

std::map<std::string,int> branchTakenMap;
std::map<std::string,int> branchFoundMap;
std::map<std::string,int> instructionMap;

void branchTaken(char* function) {
	std::string fn(function);
	if (branchTakenMap[fn]) {
		branchTakenMap[fn] += 1;
	} else {
		branchTakenMap[fn] = 1;
	}
	//printf("function : %s",fn.c_str());
}

void branchFound(char * function) {
	std::string fn(function);
	if (branchFoundMap[fn]) {
		branchFoundMap[fn] += 1;
	} else {
		branchFoundMap[fn] = 1;
	}
	//printf("function : %s",fn.c_str());
}

void print() { // na grapso statement na tupono oti dn vrethikan branches
	for (std::map<string,int>::iterator it=branchFoundMap.begin() ; it!=branchFoundMap.end() ; it++) {
		string fn = it->first;
		printf("FunctionName\tBias\tTaken\tFound\n");
		if (branchFoundMap[fn] && branchTakenMap[fn]) {
			float f = ((float) (branchFoundMap[fn]) / (float) (branchTakenMap[fn]));
			printf("%s\t%f\t%d\t%d\n",fn.c_str(),f,branchFoundMap[fn],branchTakenMap[fn]);
		}
	}
	//for (std::map<std::string,int>::const_iterator it=instructionMap.begin() ; it!=instructionMap.end() ; it++) {
	//	printf("%s\t\t%d\n",it->first.c_str(),it->second);
	//}
}

