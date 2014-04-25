#include <stdio.h>
#include <map>
#include <string>
#include <sstream>
#include <vector>

//using namespace std;

void print();

int count = 0;
std::map<std::string,int> instructionMap;

void counting(char* s){
	//printf("I am in!\n");
	std::string in(s);
    std::string str(in);
    std::string buf; // Have a buffer string
    std::stringstream ss(str); // Insert the string into a stream
    std::vector<std::string> tokens; // Create vector to hold our words
    while (ss >> buf)
        tokens.push_back(buf);
    for(std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
    	if (instructionMap[*it]) {
    		instructionMap[*it]++;
    	} else {
    		instructionMap[*it] = 1;
    	}
    	count++;
    }
}

void print() {
	for (std::map<std::string,int>::const_iterator it=instructionMap.begin() ; it!=instructionMap.end() ; it++) {
		printf("%s\t\t%d\n",it->first.c_str(),it->second);
	}
	printf("TOTAL\t\t%d\n", count);
}
