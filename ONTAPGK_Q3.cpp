#include <iostream.h>
#include <stdio.h>
#include <math.h>

struct ps{
 int tu;
 int mau;	
};

void input(ps &pso){
	printf("Nhap tu so: ");
	scanf("%d", &pso.tu);
	x:
	printf("Nhap mau so: ");
	scanf("%d", &pso.mau);
	if(pso.mau == 0){
		printf("Mau so phai khac 0!!!");
		goto x;
	}
}

void output(ps pso){
	if (pso.tu==pso.mau)
		printf("1");
	else
		printf("Phan so = %d/%d", pso.tu, pso.mau);
}

int ucln(int a, int b){
	a = abs(a);
 	b = abs(b);
	while(b!=a){
		if(a>b)
			a=a-b;
		if(b>a)
			b=b-a;
	}
	return a;
}

void toiGian(ps &pso){
	int u = ucln(pso.tu, pso.mau);
	pso.tu = pso.tu / u;
	pso.mau = pso.mau / u;
}

ps tong(ps psoA, ps psoB){
	ps psoTong;
	int mauChung = psoA.mau * psoB.mau;
	
	psoTong.tu = (psoA.tu*psoB.mau + psoB.tu*psoA.mau);
	psoTong.mau = mauChung;
	
	// toi gian
	toiGian(psoTong);
	return psoTong;
}

int main(int argc, char *argv[])
{
	ps pso1, pso2;
	cout<<"Nhap phan so thu 1: "<<"\n";
	input(pso1);
	cout<<"Nhap phan so thu 2: "<<"\n";
	input(pso2);
	//printf("%d", ucln(3,11));
	
	//toiGian(pso1);
	//output(pso1);
	// tinh tong
	cout<<"Tong 2 phan so: ";
	output(tong(pso1, pso2));
	return 0;
}