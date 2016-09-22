#include <iostream.h>
#include <stdio.h>
int main()
{
	FILE *fout = fopen("DSLKD.cpp","rt");
	if(fout==NULL)
		cout<<"Couldn't open!";
	else{
		//fprintf(stdout ,fout);
		//char data[1000];
		//fread(data,256,256,fout);
		//char ch = getc(fout);
		//cout<<ch;
		char data[256];
		//fread(data,1,256,fout);
		while(fout!=NULL){
			fread(data,1,256,fout);
			cout<<data;
		}
		
		fclose(fout);
	}
		
	
	return 0;
}