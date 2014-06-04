//#include <stdio.h>
#define NULL 0

int main() {
	// simple statementsi
	float  a = 2.0f;
	float  b = 3.0f;
	float *c = &a; // c->a
	//testing X = &Y
	c = &b; // c -> a,b
	float d = 0.5f;
	float* e = &d; // c -> a,b ; e -> d
	float* f;
	float* g;
	float** h;
	float **j;
	float *l = &b; // c -> a,b ; e -> d; l -> b;
	//testing X = Y
	f = e; // c -> a,b ; e -> d; l -> b; f ->d
	g = f; //c-> a,b ; e -> d; l -> b; f ->d ; g->d
	h = &f; //c-> a,b ; e -> d; l -> b; f ->d ; g->d; h->f
	e = &a; //c-> a,b ; e -> a,d; l -> b; f ->d ; g->d ; h->f
	//testing *X = Y
	*h = e; //c-> a,b ; e -> a,d; l -> b; f ->a,d ; g->d ; h->f ;
	j = &l; //c-> a,b ; e -> a,d; l -> b; f ->a,d ; g->d ; h->f ; j -> l
	//Testing X = *Y
	f = *j; //c-> a,b ; e -> a,d; l -> b; f ->a,b,d ; g->d ; h->f ; j -> l
	e = *j; //c-> a,b ; e -> a,b,d; l -> b; f ->a,b,d ; g->d ; h->f ; j -> l
	c = NULL; //e -> a,b,d; l -> b; f ->a,b,d ; g->d ; h->f ; j -> l
	// simple branch
	if (a==3) {
		e = &d;
	} else {
		f = &d;
	}
	//simple loop
	for (int k = 0 ; k < 10 ; k++) {
		if (k==5)
			j = &e;
		if (k==9)
			j = &f;
	}
	return 0;
}
