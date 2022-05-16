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
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <bits/stdc++.h>
#define BACKLOG 10 // how many pending connections queue will hold

using namespace std;

void sigchld_handler(int s){
    while(wait(NULL) > 0);
}

void receiving(int server_fd, vector<string> filesihave, int myport, int myuniqueid, vector<string> filesiwant,int neighbours, vector<string> &filesfound, vector<string> &ans)
{
    struct sockaddr_in address;
    int valread;
    char buffer[2000];
    int addrlen = sizeof(address);
    fd_set current_sockets, ready_sockets;

    //Initialize my current set
    FD_ZERO(&current_sockets);
    FD_SET(server_fd, &current_sockets);
    int k = 0;
    int portofr,filesofr;
    // int portpos=0;
    int count11 = 0;
    // vector<string> filesfound;
    // bool filefoundornot[filesiwant.size()] = {false};
    // string foundloc[filesiwant.size()];
    time_t current_time;
    time_t current_time11;
    current_time = time(NULL);
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 500000;

    while (1)
    {

        k++;
        ready_sockets = current_sockets;

        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, &tv) < 0)
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
                    struct sockaddr_in *s = (struct sockaddr_in *)&address;

                    

                    FD_SET(client_socket, &current_sockets);
                }
                else
                {
                    valread = recv(i, buffer, sizeof(buffer), 0);
                    buffer[valread] = '\0';
                    string buffstr(buffer);
                    //13 is ascii of carriage return which is coming in end in some cases
                    if(int(buffer[buffstr.size()-1]) == 13){
                        buffstr.pop_back();
                    } 

                    // cout<<buffstr<<"$$$$"<<i<<endl;
                    // portpos = 4+filesihave.size()*count11;
                    // cout<<"BUFF: "<<buffstr<<endl;


                    if(buffstr.substr(buffstr.size()-1,buffstr.size()) == "F"){
       
                        size_t pos11 = buffstr.find("@");
                        string filefd = buffstr.substr(0,pos11);;
                        filesfound.push_back(filefd);
                    }


                    else if(buffstr[0] == '}' && buffstr[1] == '{'){

                        size_t pos10 = buffstr.find("-");
                        size_t pos11 = buffstr.find("*");
                        size_t pos12 = buffstr.find("+");
                        string uqid1, id1, port1;
                        uqid1 = buffstr.substr(pos12+1, buffstr.size());
                        size_t pos13 = uqid1.find("*");
                        uqid1 = uqid1.substr(0,pos13);
                        id1 = buffstr.substr(pos11+1,buffstr.size());
                        buffstr = buffstr.substr(0,pos12);
                        port1 = buffstr.substr(pos10+1,buffstr.size());
                        buffstr = buffstr.substr(0,pos10);
                        portofr = stoi(port1);
                        buffstr = buffstr.substr(2, buffstr.size());
                        // cout<<"DDD: "<<buffstr<<endl;
    
                        
                        string aa="Connected to "+id1+" with unique-ID "+uqid1+" on port "+to_string(portofr);
                        ans.push_back(aa);
                        for(int j=0; j<filesihave.size(); j++){
                        // cout<<"*******"<<filesihave[j]<<"********"<<endl;
                        if(filesihave[j] == buffstr){
                            string hhh = buffstr + "$" + to_string(myuniqueid) + "@.F";

                            int sockfdsendfiles;
                            struct sockaddr_in their_addr;
                            int numbytes;
                            const char *array = hhh.c_str();

                            if ((sockfdsendfiles = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                                perror("socket");
                                exit(1);
                            }

                            their_addr.sin_family = AF_INET; // host byte order
                            their_addr.sin_port = htons(portofr); // short, network byte order
                            their_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
                            bzero(&(their_addr.sin_zero), 8); // zero the rest of the struct
                        
                            // cout<<"Arya"<<endl;
                            if (connect(sockfdsendfiles, (struct sockaddr *)&their_addr,sizeof(struct sockaddr)) == -1) {
                                perror("connect");
                                exit(1);
                            }
                            if ((numbytes=send(sockfdsendfiles, array, hhh.size(), 0)) == -1) {
                                perror("recvfrom");
                                exit(1);
                            }

                            close(sockfdsendfiles);
                        }
                    }
                    }

                    FD_CLR(i, &current_sockets);
   
                }
            }
        }

        if (k == (FD_SETSIZE * 2))
            break;
    }
}

void sending(int senderport, string hhh){

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
    vector<string> ans;
    vector<string> ans1;

    namespace fs = std::filesystem;


    string path = argv[2];

    
    for (const auto & entry : fs::directory_iterator(path))
        {
            size_t pos = entry.path().u8string().find("/");
            pos = entry.path().u8string().find("/", pos + 1);
            pos = entry.path().u8string().find("/", pos + 1);
            pos = entry.path().u8string().find("/", pos + 1);
            pos = entry.path().u8string().find("/", pos + 1);
            if(entry.path().u8string().substr(pos+1, string::npos) != "Downloaded"){
                filesihave.push_back(entry.path().u8string().substr(pos+1, string::npos));
            }
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

    myfiles = stoi(textlbl[3]);

    for(int i=4; i<(myfiles+4); i++){
        filename.push_back(textlbl[i]);
        // cout<<textlbl[i]<<endl;
    }


    neighbours = stoi(textlbl[1]);

    sort(filesihave.begin(), filesihave.end());
    for(int i=0; i<filesihave.size(); i++){
        cout<<filesihave[i]<<endl;
        
    }

    if(neighbours==0){
        for(int i=0; i<filename.size(); i++){
            if(int(filename[i][filename[i].size()-1]) == 13){
                                    filename[i].pop_back();
                                }
            string aa = "Found "+filename[i]+" at 0 with MD5 0 at depth 0";
            ans1.push_back(aa);
        }
        sort(ans1.begin(), ans1.end());
        for(int i=0; i<ans1.size(); i++){
            cout<<ans1[i]<<endl;
        }
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
    unsigned int sin_size;
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


   
    for(int i=0; i<neighbours; i++){
        // string myportinfo = to_string(myport)+"^";
        // sending(neighbourport[i], myportinfo);
        // string myfilesinfo = to_string(myfiles)+".#";
        // sending(neighbourport[i], myfilesinfo);

        for(int j=0; j<myfiles; j++){
            if(int(filename[j][filename[j].size()-1]) == 13){
                        filename[j].pop_back();
            } 
            string hhh = "}{"+filename[j]+"-"+to_string(myport)+"+"+to_string(myuniqueid)+"*"+to_string(myid);
            sending(neighbourport[i], hhh);
        }
    }


    bool filefoundornot[filename.size()] = {false};
    int foundloc[filename.size()];
    
    for(int i=0; i<filename.size(); i++){
    foundloc[i] = 100000;
    }
    vector<string> filesfound;

    



    receiving(sockfdrec, filesihave, myport, myuniqueid, filename, neighbours, filesfound, ans);

    for(int l=0; l<filename.size(); l++){
                            for(int j=0; j<filesfound.size(); j++){
                                size_t pos12 = filesfound[j].find("$");
                                int coungg=0;
                                for(int h=0; h<filename[l].size(); h++){
                                    if(filename[l][h] == filesfound[j].substr(0,pos12)[h]){
                                        coungg++;
                                    }
                                }
                                if(int(filename[l][filename[l].size()-1]) == 13){
                                    filename[l].pop_back();
                                }
                                if(int(filesfound[j][filesfound[l].size()-1]) == 13){
                                    filesfound[j].pop_back();
                                }
                                if(filename[l].size() <= coungg){
                                    filefoundornot[l] = true;
                    
                                    foundloc[l] = min(foundloc[l],stoi(filesfound[j].substr(pos12+1,filesfound[j].size())));
                               
                                  
                                }
                            }
                        }
    for(int l=0; l<filename.size(); l++){
        if(filefoundornot[l]){
            string aa = "Found "+filename[l]+" at "+to_string(foundloc[l])+" with MD5 0 at depth 1";
            ans1.push_back(aa);
        }
        else{
            string aa = "Found "+filename[l]+" at 0 with MD5 0 at depth 0";
            ans1.push_back(aa);
        }
    }

    sort(ans.begin(), ans.end());
    sort(ans1.begin(), ans1.end());

    for(int i=0; i<(ans.size()-1); i++){
        if(ans[i]!=ans[i+1]){
            cout<<ans[i]<<endl;
        }
    }
    cout<<ans[ans.size()-1]<<endl;
    for(int i=0; i<ans1.size(); i++){
        cout<<ans1[i]<<endl;
    }

    return 0;
}

