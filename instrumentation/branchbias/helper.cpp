#include <stdio.h>
#include <string>
#include <map>

//using namespace std;

map<string,int> branchTakenMap;
map<string,int> branchFoundMap;

void branchTaken(char* function) {
	string fn(function);
	if (branchTakenMap[fn]) {
		branchTakenMap[fn] += 1;
	} else {
		branchTakenMap[fn] = 0;
	}
}

void branchFound(char * function) {
	string fn(function);
	if (branchFoundMap[fn]) {
		branchFoundMap[fn] += 1;
	} else {
		branchFoundMap[fn] = 0;
	}
}

void print() {
//	for (map<string,int>const_iterator it=branchFoundMap.begin() ; it!=branchFoundMap.end() ; it++) {
//		string fn(it->first());
//		printf("FunctionName\tBias\tTaken\tFound");
//		if (branchFoundMap[fn] && branchTakenMap[fn]) {
//			float f = ((float) (branchFoundMap[fn]) / (float) (branchTakenMap[fn]));
//			printf("%s\t%f\t%d\t%d",fn.c_str(),f,branchFoundMap[fn],branchTakenMap[fn]);
//		}
//	}
}

