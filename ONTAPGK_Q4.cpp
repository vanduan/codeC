#include <iostream.h>
#include <math.h>

// Q1
int abc(int n){
	return (int)pow(2,n);
}
// Q2
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
// begin Q3  
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
// end Q3 ///
//


int main()
{
	int luaChon;
	while(true){
		cout<<"\n"<<"Bai Kiem Tra Thuc Hanh: "<<endl;
		cout<<"1. Q1"<<endl;
		cout<<"2. Q2"<<endl;
		cout<<"3. Q3"<<endl;
		cout<<"4. Thoat"<<endl;
		cout<<"Nhap 1 so (1, 2, 3 hoac 4) de thuc hien Test: ";
		cin>>luaChon;
		switch(luaChon){
			case 4:
				cout<<"Good bye!!!";
				exit(0);
			case 1:
				int n;
				cout<<"Nhap so n: ";cin>>n;
				cout<<"Ket qua: "<<abc(n);
				break;
			case 2:
				int Q[100], soPT;
				input(Q, soPT);
				output(Q, soPT);
				cout<<"\n"<<"Tich cac phan tu: "<<T_rec(Q, soPT)<<"\n";
				break;
			case 3:
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
				break;
			default:
				cout<<"May bi mu akkkkkk!!!";
		}
	}	
}