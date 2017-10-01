/*
	Author: vanduan95.dvp@gmail.com
	Date: 9/2017

	Use socket to download file from internet
	HTTP protocol only
	
	OS: Windows (C-Free 5)
	Command to compile: gcc dofile.cpp -o dofile -lws2_32 -lstdc++
*/

#define _WIN32_WINNT 0x0501

#include <winsock.h>
#include <string>
#include <iterator>
#include <fstream>
#include <iostream>
using namespace std;

#define SIZE 10240

bool conn2host();
void closeconn();
void link2host();
void link2path();
bool download_file();
string get_filename(string PATHFILE);

SOCKET soc; 
int PORT;
string PATHFILE, HOST, URL;

int main(int argc, char *argv[])
{
	if(argc !=2 ){
		std::cout<<"Usage: "<<argv[0]<<" <url>"<<std::endl;
		exit(1);
	}
	
	URL = argv[1];
	link2host();
	link2path();
	if(PATHFILE == ""){
		std::cout<<"Path file is null"<<std::endl;
		exit(1);
	}
	
 	// connect to host
	if(conn2host()){ 
		// success
		std::cout<<"Connect successful"<<std::endl;
		
		if(download_file()){
			
		}
		else{
			
		}
	}
	
	closeconn(); // close socket
	return 0;
}

// CONNECT2HOST - Connect to server
bool conn2host(){
	WSADATA w;
	SOCKADDR_IN ser; //Socket address information
	
	// starting up the Winsock API ver.2
	int error = WSAStartup(0x0202, &w);

	if (error){
		// what happen!
		std::cout<<"ERROR "<<WSAGetLastError()<<": Call WSAStartup()"<<std::endl;
		return false;	
	}
	// check winsoc version 2
	if (w.wVersion != 0x0202) {
		std::cout<<"ERROR "<<WSAGetLastError()<<": winsock version"<<std::endl;
		WSACleanup();
		return false;
	}
	
	// use DNS to get IP address
	struct hostent *lh;
	lh = gethostbyname(HOST.c_str());
	if(lh == NULL){
		std::cout<<"ERROR "<<WSAGetLastError()<<": No such host name: "<<HOST<<std::endl;
		closeconn();
		exit(1);
	}
	else if (lh->h_length != sizeof(in_addr)){
 		std::cout<<"ERROR "<<WSAGetLastError()<<": gethostbyname did not return IPv4 addresses" <<std::endl;
 		closeconn();
		exit(1);
 	}
 	else{
 		// we have server IP
		ser.sin_addr.s_addr = *(u_long *) lh->h_addr_list[0];
    	std::cout<<"Server IP"<<": "<<inet_ntoa(ser.sin_addr)<<std::endl;
	 }
	
	ser.sin_family = AF_INET; // address family Internet (IPv4)
	ser.sin_port = htons(PORT); // port to connect

	soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // create socket
	
	if (soc == INVALID_SOCKET){
		// cannot create socket
		std::cout<<"ERROR "<<WSAGetLastError()<<": invalib socket"<<std::endl;
		return false;
	}

	if (connect(soc, (SOCKADDR *)&ser, sizeof(ser)) == SOCKET_ERROR){
		// cannot connect
		std::cout<<"ERROR "<<WSAGetLastError()<<": Connect socket"<<std::endl;
		return false;	
	}
	// success
	return true;
}

bool download_file(){
	string message = "GET " + PATHFILE +" HTTP/1.1\r\nHost: "+HOST+"\r\nUser-Agent: MyApp\r\n\r\n";
	std::cout<<message<<std::endl; 
	
	if(send(soc ,message.c_str(), message.length(), 0) < 0)
    {	// GET pathfile
        std::cout<<"ERROR "<<WSAGetLastError()<<": GET failed"<<std::endl;
        return false;
    }
    
    char data[SIZE];
    int total_len = 0;
    ofstream myfile;
    myfile.open(get_filename(PATHFILE).c_str(), ios::binary);
    if(!myfile.is_open()){
    	std::cout<<WSAGetLastError()<<": Unable open "<<get_filename(PATHFILE)<<std::endl;
    	return false;	
    }
    
    bool isHeader = true;
	while(true){
    	int recv_len = recv(soc, data, sizeof(data), 0);
    	if(recv_len == 0){
    		// disconnect
	    	break;
	    }
	    if(isHeader){
	    	// if data content header HTTP 
	    	// remove it
	    	int indexdata = 0;
    		for(int i = 0; i < recv_len; i++){
		    	if(data[i] == 0x0d && data[i+1] == 0x0a && data[i+2] == 0x0d && data[i+3] == 0x0a){
	    			indexdata = i+4;
	    			break;
	    		}
		    }
		    char newdata[recv_len - indexdata];
		    int index = 0;
		    for(int i = indexdata; i < recv_len; i++){
    			newdata[index] = data[i];
    			index++;
    		}
		    myfile.write(newdata, recv_len);	
		    isHeader = false;
    	}
    	else{
	    	myfile.write(data, recv_len);
	    }
	    total_len += recv_len;
	    std::cout<<"Received "<<total_len<<" bytes"<<std::endl;
    }
    myfile.close();
    
	return true;
}

// return filename
string get_filename(string PATHFILE){
	return ".\\" + PATHFILE.substr(PATHFILE.find_last_of("/"));
}

// CLOSECONNECTION - shutdown socket and close connection
void closeconn() {
	if (soc) 
		closesocket(soc);
	WSACleanup(); //Clean up Winsock
}

// get host from url
void link2host(){
	if(URL.find("http://") == 0){
		// HTTP
		PORT = 80;
		HOST = URL.substr(7);
		HOST = HOST.substr(0, HOST.find("/"));
	}
	else if(URL.find("https://") == 0){
		// HTTPS
		std::cout<<"Don't support HTTPS "<<std::endl;
		closeconn();
		exit(1);
	}
	else {
		// HTTP
		PORT = 80;
		HOST = URL.substr(0, URL.find("/"));
	}
}

// get pathfile from url
void link2path(){
	PATHFILE = URL.substr(URL.find(HOST)+HOST.length());
}
