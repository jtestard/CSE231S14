#include "../Variable.h"
#include <string>
#include <iostream>

using namespace std;
void testVariable();

int main(void) {
	testVariable();
}

void testVariable(){
	string example = "example";
	Variable v(example);
	cout << "I created a new variable with name " << v.GetName() << endl;
}
