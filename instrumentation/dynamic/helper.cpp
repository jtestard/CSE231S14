
#include <stdio.h>
#include <map>


int count = 0;

void counting(char* s){
//	if (instructionMap[s]) {
//		instructionMap[s]++;
//	} else {
//		instructionMap[s] = 1;
//	}
	//printf("Instructions : %s", s);
	count++;
}

//void counting(){
//	count++;
//}

void print() {
//	for (std::map<char*,int>::const_iterator it=instructionMap.begin() ; it!=instructionMap.end() ; it++) {
//		printf("%s\t\t%d\n",it->first,it->second);
//	}
	printf("TOTAL %d\n", count);
}
