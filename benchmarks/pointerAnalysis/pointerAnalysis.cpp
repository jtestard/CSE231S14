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
//	if (a==3) {
//		e = &d;
//	} else {
//		f = &d;
//	}
//	float **g;
//	//simple loop
//	for (int i = 0 ; i < 10 ; i++) {
//		if (i==5)
//			g = &e;
//		if (i==9)
//			g = &f;
//	}
//	return 0;
}
