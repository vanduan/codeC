/*
	Author: vanduan95.dvp@gmail.com
	Date: 9/2017
	Use socket to download file from internet
	HTTP protocol only
	
	OS: Windows (with C-Free 5)
	Command to compile: gcc dofile.cpp -o dofile -lws2_32 -lstdc++
*/

#define _WIN32_WINNT 0x0501
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

#include <winsock2.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>
#include <iterator>
#include <fstream>
#include <iostream>
using namespace std;

#define SIZE 1024000

void help(char *text);
bool conn2host();
void closeconn();
void link2host();
void link2path();
bool download_file();
bool download_file_https();
string get_filename(string PATHFILE);
void printProgress (double percentage);

SOCKET soc;
SSL *ssl;

int PORT;
string PATHFILE, HOST, URL;
string PROTOCOL;

int main(int argc, char *argv[])
{
	if(argc !=2 ){
		help(argv[0]);
		exit(1);
	}
	
	URL = argv[1];
	link2host();
	link2path();
	if(PATHFILE == "" || PATHFILE == "/"){
		std::cerr<<"Path file is null"<<std::endl;
		help(argv[0]);
		exit(1);
	}
	
 	// connect to host
	if(conn2host()){ 
		// success
		std::cout<<"Connect successful"<<std::endl<<std::endl;
		
		if(PROTOCOL	== "HTTP"){
			// download with HTTP
			download_file();
		}
		else if(PROTOCOL == "HTTPS"){
			download_file_https();
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
		std::cerr<<"ERROR "<<WSAGetLastError()<<": Call WSAStartup()"<<std::endl;
		return false;	
	}
	// check winsoc version 2
	if (w.wVersion != 0x0202) {
		std::cerr<<"ERROR "<<WSAGetLastError()<<": winsock version"<<std::endl;
		WSACleanup();
		return false;
	}
	
	// use DNS to get IP address
	struct hostent *lh;
	lh = gethostbyname(HOST.c_str());
	if(lh == NULL){
		std::cerr<<"ERROR "<<WSAGetLastError()<<": No such host name: "<<HOST<<std::endl;
		closeconn();
		exit(1);
	}
	else if (lh->h_length != sizeof(in_addr)){
 		std::cerr<<"ERROR "<<WSAGetLastError()<<": gethostbyname did not return IPv4 addresses" <<std::endl;
 		closeconn();
		exit(1);
 	}
 	else{
 		// we have server IP
		ser.sin_addr.s_addr = *(u_long *) lh->h_addr_list[0];
    	std::cout<<"Server IP"<<": "<<inet_ntoa(ser.sin_addr)<<" - Port"<<": "<<PORT<<std::endl<<std::endl;
	 }
	
	ser.sin_family = AF_INET; // address family Internet (IPv4)
	ser.sin_port = htons(PORT); // port to connect

	soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // create socket
	
	if (soc == INVALID_SOCKET){
		// cannot create socket
		std::cerr<<"ERROR "<<WSAGetLastError()<<": invalib socket"<<std::endl;
		return false;
	}

	if (connect(soc, (SOCKADDR *)&ser, sizeof(ser)) == SOCKET_ERROR){
		// cannot connect
		std::cerr<<"ERROR "<<WSAGetLastError()<<": Connect socket"<<std::endl;
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
        std::cerr<<"ERROR "<<WSAGetLastError()<<": GET failed"<<std::endl;
        return false;
    }
    
    char data[SIZE];
    int total_len = 0, data_len = -1;
    ofstream myfile;
    bool isHeader = true; //  flag: receive data content header
    
    // open file 
    myfile.open(get_filename(PATHFILE).c_str(), ios::binary);
    if(!myfile.is_open()){
    	// Unable open
    	std::cerr<<"FILE ERROR "<<WSAGetLastError()<<": Unable open "<<get_filename(PATHFILE)<<std::endl;
    	std::cerr<<"Please check your url!"<<std::endl;
    	return false;	
    }
    

	while(true){
		if(total_len == data_len){
			// disconnect when received full data
    		std::cout<<std::endl<<"Done: Save to file "<<get_filename(PATHFILE)<<std::endl;
    		break;
		}
		
    	int recv_len = recv(soc, data, sizeof(data), 0); // receive data
    	
		if(recv_len == 0){
    		// disconnect
	    	break;
	    }
	    
   	 	if(isHeader){
	    	/* check header
	    		404 Not Found
	    		403 Forbidden
	    		200 OK
	    	*/
	    	char *ok = "200 OK";
	    	if(strstr(data, ok) == NULL){ // what happen??
	    		char *notfound = "404 Not Found";
	    		char *forbidden = "403 Forbidden";
	    		
	    		// colse file and remove
				myfile.close();
	    		remove(get_filename(PATHFILE).c_str());
	    		
				if(strstr(data, notfound) != NULL){
	    			// Not Found
    		 		std::cerr<<"ERROR: 404 Not Found"<<std::endl;
        			return false;
		    	}
		    	else if(strstr(data, forbidden) != NULL){
		    		// Forbidden
	    			std::cerr<<"ERROR: 403 Forbidden"<<std::endl;
        			return false;
	    		}
	    		else{
	    			// what error???
	    			// print header to console
	    			std::cerr<<"ERROR: "<<std::endl;
		    		for(int i = 0; i < recv_len; i++){
		    			std::cout<<data[i];
				    	if(data[i] == 0x0d && data[i+1] == 0x0a && data[i+2] == 0x0d && data[i+3] == 0x0a){
			    			break;
			    		}
		    		}
        			return false;
		    	}
	    	}
	    	// OK
	    	// find index, start data file
	    	int indexdata = 0;
    		for(int i = 0; i < recv_len; i++){
		    	if(data[i] == 0x0d && data[i+1] == 0x0a && data[i+2] == 0x0d && data[i+3] == 0x0a){
	    			indexdata = i+4;
	    			break;
	    		}
		    }
		    
		    // copy data to new char array
		    char newdata[recv_len - indexdata];
		    int index = 0;
		    for(int i = indexdata; i < recv_len; i++){
    			newdata[index] = data[i];
    			index++;
    		}
    		
    		// get content-length (data len) from header
    		char *str = "Length: ";
    		char *content = strstr(data, str);
    		char size[32];
    		for(int i = 0; i < strlen(content); i++){
		    	if(content[i] == 0x20){
	    			for(int k = i+1; k < strlen(content); k++){
			    		if(content[k] == 0x0d){
		    				int index = 0;
							for(int h = i+1; h < k; h++){
								size[index] = content[h];
								index++;
							}
							break;
		    			}
	    			}
	    			break;
    			}
    		}
    		
		    data_len = atoi(size);
		    
    		// write data to file
		    myfile.write(newdata, recv_len - indexdata);	
		    isHeader = false;
		    
		    recv_len = recv_len - indexdata;
    	}
    	else{
	    	myfile.write(data, recv_len);
	    }
        total_len += recv_len;
        printProgress((double)total_len/data_len);
    }
    myfile.close();
    
	return true;
}

bool download_file_https(){
	//SSL_library_init();
	//SSLeay_add_ssl_algorithms();
	//SSL_load_error_strings();
	
	//SSL_METHOD *meth = TLSv1_2_client_method();
	
	//SSL_CTX *ctx = SSL_CTX_new(meth);
}

// return filename
string get_filename(string PATHFILE){
	char *query = "?";
	// content string query?
	// http://domain.name/path/file/filename.txt?query=zzz
	if(strstr(PATHFILE.c_str(), query) != NULL){
		PATHFILE = PATHFILE.substr(PATHFILE.find_last_of("/"));
		return ".\\" + PATHFILE.substr(0, PATHFILE.find("?"));
	}
	else
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
		PROTOCOL = "HTTP";
		PORT = 80;
		HOST = URL.substr(7);
		HOST = HOST.substr(0, HOST.find("/"));
	}
	else if(URL.find("https://") == 0){
		// HTTPS
		PROTOCOL = "HTTPS";
		PORT = 443;
		HOST = URL.substr(8);
		HOST = HOST.substr(0, HOST.find("/"));
	}
	else {
		// default is HTTP
		PROTOCOL = "HTTP";
		PORT = 80;
		HOST = URL.substr(0, URL.find("/"));
	}
	
	// port in url
	// http://domain.name:<port>/path/file/filename.txt?query=zzz
	if(HOST.find(":") != -1){
		string p = HOST.substr(HOST.find(":") + 1);
		PORT = atoi(p.c_str());
		HOST = HOST.substr(0, HOST.find(":"));
	}
}

// get pathfile from url
void link2path(){
	PATHFILE = URL.substr(URL.find(HOST)+HOST.length());
	// url content port number
	if(PATHFILE.find(":") < PATHFILE.find("/"))
		PATHFILE = PATHFILE.substr(PATHFILE.find("/"));
		
}

void help(char *text){
	std::cout<<std::endl<<"Usage: "<<text<<" <url>"<<std::endl<<std::endl;
	std::cout<<"URL:    http://domain.name/path/to/file.txt"<<std::endl;
	std::cout<<"        domain.name/path/to/file.txt"<<std::endl;
}

void printProgress (double percentage)
{
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush (stdout);
}