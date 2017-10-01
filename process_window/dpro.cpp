/*
	Author: vanduan95.dvp@gmail.com
	Date: 01/10/2017

	Funcion:
		- List process is running in windows
		- Kill process by ID
		- Run a process
	
	OS: Windows (C-Free 5)
*/
#include <unistd.h>
#include <iostream>
using namespace std;

void help(char *name);

int main(int argc, char **argv){
	if(argc == 1){
		
	}
	else if(argc == 3){
		int option;
		bool is;
		while((option = getopt(argc, argv, "l:k:r:h")) != -1){
			switch(option){
				case 'k':
					std::cout<<"kill"<<std::endl;
					break;
				case 'r':
					std::cout<<"run"<<std::endl;
					break;
				case 'h':
					std::cout<<"help"<<std::endl;
					break;
				default:
					help(argv[0]);
			}
		}
		return 0;
	}
	else{
		help(argv[0]);
		return 1;
	}
	
}

void help(char *name){
		std::cout<<std::endl;
		std::cout<<"Usage: "<<name<<" [-k PID] | [-r program name]"<<std::endl;
		std::cout<<std::endl;
		std::cout<<"\tNo args\t\tList process running"<<std::endl;
		std::cout<<"\t-k PID\t\tKill process with process ID"<<std::endl;
		std::cout<<"\t-r PNAME\tStart program"<<std::endl;
	
}