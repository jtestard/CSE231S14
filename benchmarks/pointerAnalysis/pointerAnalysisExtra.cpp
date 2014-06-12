//#include <stdio.h>
#define NULL 0

struct Node;

typedef struct Node {
	char value;
	char* name;
} Node;

int main() {
	// simple statementsi
	int  a = 2.0f;
	int *b = & a;
	int *c = b+1;
	Node n;
	Node *n_ptr = &n;
	char d = 'b';
	n.name = &d;
	return 0;
}
