#include <stdio.h>
#include <math.h>

int abc(int n){
	return (int)pow(2,n);
}
int abc123(int n){
	int S=1;
	for(int i=0; i<n; i++){
		S = S * 2; 
	}
	return S;
}

int main()
{
	int n;
	
	printf("%d\n", abc(3));
	printf("%d\n", abc123(3));
	return 0;
}