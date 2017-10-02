/*
	Author: vanduan95.dvp@gmail.com
	Date: 01/10/2017

	Funcion:
		- List process is running in windows
		- Kill process by ID
		- Run a process
	
	OS: Windows (with C-Free 5)
	Command to compile: gcc dpro.cpp -o dpro -lws2_32 -lstdc++
*/
	
#include <unistd.h>
#include <windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
using namespace std;

void help(char *name);
void list_process();
void kill_pid(int pid);
void run(char *name);

int main(int argc, char **argv){
	if(argc == 1){
		list_process();
	}
	else if(argc == 3){ // get option
		int option;
		bool is;
		while((option = getopt(argc, argv, "k:r:")) != -1){
			switch(option){
				case 'k':
					kill_pid(atoi(optarg));
					break;
				case 'r':
					run(optarg);
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

void run(char *name){
	STARTUPINFO infor={0};
	PROCESS_INFORMATION pInfor;
	
	if(!CreateProcess(NULL,  	// No module name (use command line)
			        name,      	// Command line
			        NULL,       // Process handle not inheritable
			        NULL,       // Thread handle not inheritable 
			        FALSE,      // Set handle inheritance to FALSE
			        0,          // No creation flags
			        NULL,       // Use parent's environment block
			        NULL,       // Use parent's starting directory 
			        &infor,     // Pointer to STARTUPINFO structure
			        &pInfor )   // Pointer to PROCESS_INFORMATION structure
		){
		std::cerr<<"ERROR "<<WSAGetLastError()<<": CreateProcess failed "<<name<<std::endl;
		exit(1);
	}
	// Wait until child process exits.
    //WaitForSingleObject(pInfor.hProcess, INFINITE);

    // Close process and thread handles. 
    //CloseHandle(pInfor.hProcess);
    //CloseHandle(pInfor.hThread);
}

void kill_pid(int pid){
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	
	// take a snapshot of all process in the system
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE){
		std::cerr<<"ERROR "<<WSAGetLastError()<<": CreateToolhelp32Snapshot of process"<<std::endl;
		exit(1);
	}
	
	// set the size of the struct before using it
	pe32.dwSize = sizeof(PROCESSENTRY32);
	
	// retrieve information about the first process
	// and exit if unsuccessful
	if(!Process32First(hProcessSnap, &pe32)){
		std::cerr<<"ERROR "<<WSAGetLastError()<<": Process32First"<<std::endl;
		CloseHandle(hProcessSnap); // clean the snapshot object
		exit(1);
	}
	
	// walk the snapshot of process
	// and compare PID each process with PID (to kill)
	do{
		if(pe32.th32ProcessID == pid){
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD) pe32.th32ProcessID);
            if (hProcess == NULL)
            {
            	std::cerr<<"ERROR "<<WSAGetLastError()<<": OpenProcess"<<std::endl;
				CloseHandle(hProcessSnap); // clean the snapshot object
            }
            else{
                TerminateProcess(hProcess, 9); // send termination signal
                CloseHandle(hProcess);
                std::cout<<"SUCCESS: Sent termination signal to the process with PID "<<pid<<std::endl;
                exit(0);
            }
		}
	}
	while(Process32Next(hProcessSnap, &pe32));
	std::cout<<"ERROR: No process with ID "<<pid<<std::endl;
	CloseHandle(hProcessSnap);
}

void list_process(){
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	
	// take a snapshot of all process in the system
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE){
		std::cerr<<"ERROR "<<WSAGetLastError()<<": CreateToolhelp32Snapshot of process"<<std::endl;
		exit(1);
	}
	
	// set the size of the struct before using it
	pe32.dwSize = sizeof(PROCESSENTRY32);
	
	// retrieve information about the first process
	// and exit if unsuccessful
	if(!Process32First(hProcessSnap, &pe32)){
		std::cerr<<"ERROR "<<WSAGetLastError()<<": Process32First"<<std::endl;
		CloseHandle(hProcessSnap); // clean the snapshot object
		exit(1);
	}
	
	// walk the snapshot of process
	// and display information about each process in turm
	printf("\t%-20s\t%8s\n","Image Name", "PID");
	std::cout<<"\t--------------------\t--------"<<std::endl;
	do{
		printf("\t%-20s\t%8d\n",pe32.szExeFile, pe32.th32ProcessID);
	}
	while(Process32Next(hProcessSnap, &pe32));
	CloseHandle(hProcessSnap);
}

void help(char *name){
	std::cout<<std::endl;
	std::cout<<"Usage: "<<name<<" [-k PID] | [-r pname]"<<std::endl;
	std::cout<<std::endl;
	std::cout<<"\tNo args\t\tList process running"<<std::endl;
	std::cout<<"\t-k PID\t\tKill process with process ID"<<std::endl;
	std::cout<<"\t-r PNAME\tStart program"<<std::endl;
	
}