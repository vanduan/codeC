#include <iostream.h>
#include <stdlib.h>
#include <math.h>
void input(int Q[], int &n){
	cout<<"Nhap so phan tu n = "; 
	cin>>n;
	srand( time(NULL) );
	for(int i=0; i<n; i++){
		Q[i] = 1 + rand()%((int)pow(10, 3) - 1 + 1);
	}
}

void output(int Q[], int n){
	for(int i=0; i<n; i++)
		cout<<Q[i]<<" ";
}

long T_rec(int Q[], int n){
	if(n==0)
		return 1;
	return Q[n-1]*T_rec(Q, n-1);
}

int main()
{
	int Q[100], n;
	input(Q, n);
	output(Q, n);
	cout<<"\n"<<"Tich cac phan tu: "<<T_rec(Q, n)<<"\n";
	return 0;
}