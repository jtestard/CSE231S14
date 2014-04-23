
#include <stdio.h>
#include <map>
#include <string>


int count = 0;
std::map<std::string,int> instructionMap;

//void counting(std::string &s){
//	if (instructionMap[s]) {
//		instructionMap[s]++;
//	} else {
//		instructionMap[s] = 1;
//	}
//	count++;
//}

void counting(){
	count++;
}

void print() {
	printf("Count : %d\n", count);
}
