#include <stdio.h>
#include <string>
#include <iostream>
#include <map>

using namespace std;

std::map<std::string,int> branchTakenMap;
std::map<std::string,int> branchFoundMap;
std::map<std::string,int> instructionMap;

void branchTaken(char* function) {
	string fn(function);
	if (branchTakenMap[fn]) {
		branchTakenMap[fn] += 1;
	} else {
		branchTakenMap[fn] = 1;
	}
	//printf("function : %s",fn.c_str());
}

void branchFound(char * function) {
	string fn(function);
	if (branchFoundMap[fn]) {
		branchFoundMap[fn] += 1;
	} else {
		branchFoundMap[fn] = 1;
	}
	//printf("function : %s",fn.c_str());
}

void print() {
	if (!branchFoundMap.empty()){
		printf("FunctionName\tBias\tTaken\tFound\n");
		for (std::map<string,int>::iterator it=branchFoundMap.begin() ; it!=branchFoundMap.end() ; it++) {
			string fn = it->first;
			if (branchFoundMap[fn] && branchTakenMap[fn]) {
				float f = ( (float) (branchTakenMap[fn])/ (float) (branchFoundMap[fn]));
				printf("%s\t%f\t%d\t%d\n",fn.c_str(),f,branchTakenMap[fn],branchFoundMap[fn]);
			}
		}
	}
	else{
		printf("No branches found!");
	}
	//for (std::map<std::string,int>::const_iterator it=instructionMap.begin() ; it!=instructionMap.end() ; it++) {
	//	printf("%s\t\t%d\n",it->first.c_str(),it->second);
	//}
}

