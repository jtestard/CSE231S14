#ifndef VARIABLE
#define VARIABLE
#include <string>

using namespace std;

class Variable {
private :
	string name;
	Variable();
public :
	Variable(string name);
	~Variable();
	string GetName();
};
#endif
