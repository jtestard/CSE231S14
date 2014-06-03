//#include <stdio.h>

int main() {
	// simple statementsi
	int  a = 2;
	int  b = 3;
	int *c = &a;
	c = &b; //testing X = &Y -- should have c -> b
	float d = 0.5f;
	float* e = &d;
	float* f;
	float* g;
	float** h;
	float i = 5.3f;
	f = e;
	g = f; //testing X = Y -- should have f->d, g->d
	h = &f;
	e = &i;
	*h = e; //testing *X = Y f->i

	// simple branch
	if (a==3) {
		e = &d;
	} else {
		f = &d;
	}
	float **j;
	//simple loop
	for (int k = 0 ; k < 10 ; k++) {
		if (k==5)
			j = &e;
		if (k==9)
			j = &f;
	}
	return 0;
}
