
#include <stdio.h>
#include <map>


int count = 0;

std::map<char*,int> instructionMap;
void counting(char* s){
//	if (instructionMap[s]) {
//		instructionMap[s]++;
//	} else {
//		instructionMap[s] = 1;
//	}
//	count++;
//	printf("%s\n",s);
}
void print() {
	for (std::map<char*,int>::const_iterator it=instructionMap.begin() ; it!=instructionMap.end() ; it++) {
		printf("%s\t\t%d\n",it->first,it->second);
	}
	printf("TOTAL %d\n", count);
}
