#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <bits/stdc++.h>
#define BACKLOG 10 // how many pending connections queue will hold

using namespace std;
    

void sigchld_handler(int s){
    while(wait(NULL) > 0);
}
string convertToString(char* a)
{
    string s = a;
    return s;
}
void receiving(int server_fd, int neighbours, int &yy, vector<string> &ans)
{
    struct sockaddr_in address;
    int valread;
    char buffer[2000] = {0};
    int addrlen = sizeof(address);
    fd_set current_sockets, ready_sockets;
    struct timeval tv;
    tv.tv_sec = 10;

    //Initialize my current set
    FD_ZERO(&current_sockets);
    FD_SET(server_fd, &current_sockets);
    int k = 0;
    while (1)
    {
        k++;
        ready_sockets = current_sockets;

        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
        {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &ready_sockets))
            {

                if (i == server_fd)
                {
                    int client_socket;

                    if ((client_socket = accept(server_fd, (struct sockaddr *)&address,
                                                (socklen_t *)&addrlen)) < 0)
                    {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    FD_SET(client_socket, &current_sockets);
                }
                else
                {
                    valread = recv(i, buffer, sizeof(buffer), 0);
                    yy++;
                    ans.push_back(convertToString(buffer));
                    FD_CLR(i, &current_sockets);
                    if(yy == neighbours){
                        return;
                    }
                }
            }
        }

        if (k == (FD_SETSIZE * 2))
            break;
    }
}

void sending(int senderport, string hhh, int yy, int neighbours){

        int sockfdsend;
        struct sockaddr_in their_addr;
        int numbytes;
        const char *array = hhh.c_str();

        if ((sockfdsend = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(1);
        }

        their_addr.sin_family = AF_INET; // host byte order
        their_addr.sin_port = htons(senderport); // short, network byte order
        their_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
        bzero(&(their_addr.sin_zero), 8); // zero the rest of the struct
    
        while(1){
            if(yy == neighbours){return;}
            if (connect(sockfdsend, (struct sockaddr *)&their_addr,sizeof(struct sockaddr)) == -1) {
                // perror("connect");
                continue;
                exit(1);
            }
            break;
        }
        if ((numbytes=send(sockfdsend, array, hhh.size(), 0)) == -1) {
            perror("recvfrom");
            exit(1);
        }

        close(sockfdsend);
    }

int main(int argc, char *argv[]) {

    
	int myid;
	int myport;
	int myuniqueid;
	int neighbours;
	vector<int> neighbourid;
	vector<int> neighbourport;
	int myfiles;
	vector<string> filename;
    vector<string> filesihave;
    int yy=0;

    namespace fs = std::filesystem;


    string path = argv[2];

    
    for (const auto & entry : fs::directory_iterator(path))
        {
            size_t pos = entry.path().u8string().find("/");
            pos = entry.path().u8string().find("/", pos + 1);
            pos = entry.path().u8string().find("/", pos + 1);
            pos = entry.path().u8string().find("/", pos + 1);
            pos = entry.path().u8string().find("/", pos + 1);
            filesihave.push_back(entry.path().u8string().substr(pos+1, string::npos));

        }


	string myText;
	vector<string> textlbl;
	ifstream MyReadFile(argv[1]);
	int i=0;

	while (getline (MyReadFile, myText)) {
	  textlbl.push_back(myText);
	}

	MyReadFile.close();
	// cout.flush();

	myid = stoi(textlbl[0].substr(0, textlbl[0].find(" ")));
	textlbl[0].erase(0, textlbl[0].find(" ") + 1);
	myport = stoi(textlbl[0].substr(0, textlbl[0].find(" ")));
	textlbl[0].erase(0, textlbl[0].find(" ") + 1);
	myuniqueid=stoi(textlbl[0]);

	neighbours = stoi(textlbl[1]);



	

	myfiles = stoi(textlbl[3]);

	for(int i=4; i<(myfiles+4); i++){
		filename.push_back(textlbl[i]);
	}
    sort(filesihave.begin(), filesihave.end());

    for(int i=0; i<filesihave.size(); i++){
        if(filesihave[i] != "Downloaded"){
            cout<<filesihave[i]<<endl;
        }
    }

    if(neighbours == 0){
        return 0;
    }


    for(int i=0; i<neighbours; i++){
        neighbourid.push_back(stoi(textlbl[2].substr(0, textlbl[2].find(" "))));
        textlbl[2].erase(0, textlbl[2].find(" ") + 1);
        neighbourport.push_back(stoi(textlbl[2].substr(0, textlbl[2].find(" "))));
        textlbl[2].erase(0, textlbl[2].find(" ") + 1);
    }






	int sockfdrec; // listen on sock_fd, new connection on new_fd
    struct sockaddr_in my_addr; // my address information
    // struct sockaddr_in their_addr; // connector’s address information
    // unsigned int sin_size;
    struct sigaction sa;
    int yes=1;
	char buf[100];
	struct hostent *he;

    if ((sockfdrec = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET; // host byte order
    my_addr.sin_port = htons(myport); // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    bzero(&(my_addr.sin_zero), 8); // zero the rest of the struct

    if (bind(sockfdrec, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }
    if (listen(sockfdrec, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    // cout<<myport<<" "<<neighbourport[0]<<endl;

    // if ((sockfdsend = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    //     perror("socket");
    //     exit(1);
    // }

    
    // cout<<"Press 1 to send"<<endl;

    // int num;
    // cin>>num;
    vector<string> ans;
    string hhh = "Connected to "+to_string(myid)+" with unique-ID "+to_string(myuniqueid)+" on port "+to_string(myport);


    for(int i=0; i<neighbours; i++){
        sending(neighbourport[i], hhh, yy, neighbours);
    }


	receiving(sockfdrec, neighbours, yy, ans);

    sort(ans.begin(),ans.end());
    for(int i=0; i<ans.size(); i++){
        cout<<ans[i]<<endl;
    }



    return 0;
}

