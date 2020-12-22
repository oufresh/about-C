#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int a;
	long b;
	long int c;
	long long d;
	void *p;
	int *pi;
	float f;
	double df;

	printf("sizeof(int) = %d\n", sizeof(a));
	printf("sizeof(long) = %d\n", sizeof(b));
	printf("sizeof(unsigned long) = %d\n", sizeof(unsigned long));
	printf("sizeof(long int) = %d\n", sizeof(c));
	printf("sizeof(long long) = %d\n", sizeof(d));
	printf("sizeof(void *) = %d\n", sizeof(p));
	printf("sizeof(int *) = %d\n", sizeof(pi));
	printf("sizeof(float) = %d\n", sizeof(f));
	printf("sizeof(double) = %d\n", sizeof(df));


	return 0;
}
