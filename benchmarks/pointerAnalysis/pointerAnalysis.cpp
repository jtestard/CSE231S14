//#include <stdio.h>

int main() {
	// simple statements
	int  a = 2;
	int* b = &a;
	int* c = &a;
	int  d = 3;
	// simple branch
	if (a==3) {
		int *e = &d; 
	} else {
		int *f = &d;	
	}
	//simple loop
	for (int i = 0 ; i < 10 ; i++) {
		d = d+i;	
	}
	return 0;
}
