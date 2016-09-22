// Author: Van Duan
// Date: 18/09/2016

#include <iostream.h>
#include<time.h>
#include<math.h>
// dinh nghia Node va List
typedef struct 	Node{
	int Data;
	Node *pNext;
};

typedef struct	List{
	Node *pHead, *pTail;
	int soPT; // so phan tu cua DSLKD
};

// GetNode ==> Tao Node moi
Node* GetNode(int x)
{	
	Node *p = new Node;
	if (p==NULL)
		{	cout<<"Khong du bo nho";
			return NULL;
		}
	p->Data=x;
	p->pNext=NULL;
	return p;
}
// khoi tao danh sach rong
void Init(List &L)
{
	L.pHead = L.pTail = NULL;
}
// them Node vao cuoi List
void addTail(List &L, Node *p)
{	
	if (L.pHead == NULL)  
		L.pHead = L.pTail = p;
	else
	{
		L.pTail->pNext = p;
		L.pTail = p;
	}	
}
// Tao List
void CreateListTail (List &L)
{
	srand(time(NULL));
	int x; 
 	cout<<"	Nhap so node n = "; 
 	cin>>L.soPT;
 	Node *newNode;
 	for (int i=0; i<L.soPT; i++)
  	{	
  		x = rand()%100;
		newNode = GetNode(x);
		addTail(L, newNode);	
  	}
}
//Tim kiem Node có khóa x trong DSLK
Node* Search (List L, int x)
{	Node* p = L.pHead;
	while (p!=NULL && p->Data!=x) p=p->pNext;
    return p;
}
// In danh sach
void PrintList (List L)
{	
	Node *p = L.pHead;
	cout<<"	";
	while(p!=NULL){
		cout<<p->Data<<" ";
		p=p->pNext;	
	}
	cout<<endl<<"	";
}
// menu chon lua chuc nang
int menu(){
	int choise;
	cout<<"	+====================================+"<<endl;
	cout<<"	|      DANH SACH LIEN KET DON        |"<<endl;
	cout<<"	|------------------------------------|"<<endl;
	cout<<"	| 1. Khoi tao danh sach              |"<<endl;
	cout<<"	| 2. In danh sach                    |"<<endl;
	cout<<"	| 3-6. Them node moi sau node        |"<<endl;
	cout<<"	|         3. co khoa lon nhat        |"<<endl;
	cout<<"	|         4. co khoa nho nhat        |"<<endl;
	cout<<"	|         5. co khoa la SNT dau tien |"<<endl;
	cout<<"	|         6. co khoa la SNT lon nhat |"<<endl;
	cout<<"	| 0. Thoat                           |"<<endl;
	cout<<"	+====================================+"<<endl;
	cout<<"	Lua chon: ";
	cin>>choise;
	cout<<""<<endl;
	return choise;
}
// kiem tra DSLKD
bool checkList(List L){
	if(L.pHead == NULL){  /*List NULL*/	
		cout<<"	Danh sach chua duoc khoi tao"<<endl;
		cout<<"	";
		system("pause");
		return true;
	}
	return false; // List not NULL
}

// tim Node co khoa lon nhat
Node* findMax(List L){
	Node *nodeMax = L.pHead;
	Node *p=L.pHead;
	int max=0;
	while(p!=NULL){
		if(p->Data > max){
			max = p->Data;
			nodeMax = p;	
		}
		p = p->pNext;
	}
	return nodeMax;
}
// them Node sau node co khoa lon nhat
void addNodeAfterMax(List &L){
	int data;
	cout<<endl<<"	Nhap gia tri cho node can them: "; cin>>data;
	Node *newNode = GetNode(data);
	Node *nodeMax = findMax(L);
	cout<<endl<<"	Node max: "<<nodeMax->Data<<endl<<endl;
	if(nodeMax==L.pTail){
		L.pTail->pNext=newNode;
		L.pTail=newNode;
	}
	else{
		Node *temp = nodeMax->pNext;
		nodeMax->pNext=newNode;
		newNode->pNext = temp;
	}
}
// tim Node co khoa nho nhat
Node* findMin(List L){
	Node *nodeMin = L.pHead;
	Node *p=L.pHead;
	int min=p->Data;
	while(p!=NULL){
		if(p->Data < min){
			min = p->Data;
			nodeMin = p;	
		}
		p = p->pNext;
	}
	return nodeMin;
}
// them Node sau node co khoa nho nhat
void addNodeAfterMin(List &L){
	int data;
	cout<<endl<<"	Nhap gia tri cho node can them: "; cin>>data;
	Node *newNode = GetNode(data);
	Node *nodeMin = findMin(L);
	cout<<endl<<"	Node min: "<<nodeMin->Data<<endl<<endl;
	Node *temp = nodeMin->pNext;
	nodeMin->pNext=newNode;
	newNode->pNext = temp;
}
// kiem tra SNT
bool checkSNT(int n)
{
	if (n < 2)
		return false;
	else{
		int temp = sqrt((float)n);
		for (int i = 2; i <= temp; i ++)
			if (n%i==0)
				return false;
	}
	return true;
}
// tim Node co khoa la SNT dau tien
Node* findSNT(List L){
	Node *p=L.pHead;
	while(p!=NULL){
		if(checkSNT(p->Data))
			return p;
		p=p->pNext;
	}
	return NULL;
}
// them Node sau node co khoa la SNT dau tien
void addNodeAfterSNT(List &L){
	Node *nodeSNT = findSNT(L);
	if (nodeSNT!=NULL){
		int data;
		cout<<endl<<"	Nhap gia tri cho node can them: "; cin>>data;
		Node *newNode = GetNode(data);
		cout<<endl<<"	Node SNT: "<<nodeSNT->Data<<endl<<endl;
		if(nodeSNT == L.pTail){
			L.pTail->pNext=newNode;
			L.pTail=newNode;
		}
		else{
			Node *temp = nodeSNT->pNext;
			nodeSNT->pNext=newNode;
			newNode->pNext = temp;
		}
	}
	else{
		cout<<endl<<"	Danh sach khong co so nguyen to"<<endl<<"	";
		system("pause");
	}
}
// tim Node co khoa la SNT lon nhat
Node* findSNTMax(List L){
	int sntMax = -1;
	Node *nodeSNTMax = NULL;
	Node *p=L.pHead;
	while(p!=NULL){
		if(checkSNT(p->Data) && p->Data > sntMax){
			sntMax = p->Data;
			nodeSNTMax = p;	
		}
		p=p->pNext;
	}
	return nodeSNTMax;
}
// them Node sau node co khoa la SNT lon nhat
void addNodeAfterSNTMax(List &L){
	Node *nodeSNTMax = findSNTMax(L);
	if (nodeSNTMax!=NULL){
		int data;
		cout<<endl<<"	Nhap gia tri cho node can them: "; cin>>data;
		Node *newNode = GetNode(data);
		cout<<endl<<"	Node SNT lon nhat: "<<nodeSNTMax->Data<<endl<<endl;
		if(nodeSNTMax == L.pTail){
			L.pTail->pNext=newNode;
			L.pTail=newNode;
		}
		else{
			Node *temp = nodeSNTMax->pNext;
			nodeSNTMax->pNext=newNode;
			newNode->pNext = temp;
		}
	}
	else{
		cout<<endl<<"	Danh sach khong co so nguyen to"<<endl<<"	";
		system("pause");
	}
}
main(int argc, char *argv[])
{
 	List L;
	while(true){
		int luaChon = menu();
		switch(luaChon){
			case 0:
				exit(0);
			case 1:
				Init(L);
				CreateListTail(L);
				break;
			case 2:
				if(!checkList(L)){
					PrintList(L);
					system("pause");
				}
				break;
			case 3:
				if(!checkList(L)){
					cout<<"	Danh sach truoc:"<<endl;
					PrintList(L);
					addNodeAfterMax(L);
					cout<<"	Danh sach sau:"<<endl;
					PrintList(L);
					system("pause");
				}
				break;
			case 4:
				if(!checkList(L)){
					cout<<"	Danh sach truoc:"<<endl;
					PrintList(L);
					addNodeAfterMin(L);
					cout<<"	Danh sach sau:"<<endl;
					PrintList(L);
					system("pause");
				}
				break;
			case 5:
				if(!checkList(L)){
					cout<<"	Danh sach truoc:"<<endl;
					PrintList(L);
					addNodeAfterSNT(L);
					cout<<"	Danh sach sau:"<<endl;
					PrintList(L);
					system("pause");
				}
				break;
			case 6:
				if(!checkList(L)){
					cout<<"	Danh sach truoc:"<<endl;
					PrintList(L);
					addNodeAfterSNTMax(L);
					cout<<"	Danh sach sau:"<<endl;
					PrintList(L);
					system("pause");
				}
				break;
		}
		system("cls");
	}
	
}