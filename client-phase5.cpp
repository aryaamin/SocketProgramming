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
#include <openssl/md5.h>
#include <sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/md5.h>
#include <sys/wait.h>
#include <signal.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <openssl/md5.h>
#include <sha.h>
#define BACKLOG 100 // how many pending connections queue will hold
#define MAX_LINE 4096

using namespace std;

void sigchld_handler(int s){
    while(wait(NULL) > 0);
}
ssize_t total=0;
void sendfile(FILE *fp, int sockfd) 
{
    int n; 
    char sendline[MAX_LINE] = {0}; 
    while ((n = fread(sendline, sizeof(char), MAX_LINE, fp)) > 0) 
    {
        total+=n;
        if (n != MAX_LINE && ferror(fp))
        {
            perror("Read File Error");
            exit(1);
        }
        
        if (send(sockfd, sendline, n, 0) == -1)
        {
            perror("Can't send file");
            exit(1);
        }
        memset(sendline, 0, MAX_LINE);
    }
}

void writefile(int sockfd, FILE *fp, bool b1)
{
    ssize_t n;
    char buff[MAX_LINE] = {0};
    while ((n = recv(sockfd, buff, MAX_LINE, 0)) > 0) 
    {
        if(!b1){
            continue;
        }
        total+=n;
        if (n == -1)
        {
            perror("Receive File Error");
            exit(1);
        }
        
        if (fwrite(buff, sizeof(char), n, fp) != n)
        {
            perror("Write File Error");
            exit(1);
        }
        memset(buff, 0, MAX_LINE);
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

void receiving(int server_fd, vector<string> filesihave, int myport, int myuniqueid, vector<string> filesiwant,int neighbours, vector<string> &filesfound, char* pathtofile, vector<int> &portoffilesfound, vector<int> &uqidoffilesfound, vector<string> &hashofff,vector<string> &depthofff, vector<int> neighbourport)
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
    tv.tv_sec = 30;
    char *filename33;

    while (1)
    {
        k++;
        // cout<<"1"<<endl;
        ready_sockets = current_sockets;
        // int recport;
        // int recport1;
        // int recport2;
        // int recport3;
        // int recport4;
        // int recport5;
        // static fd_set empty;     // initialized to 0 -> empty
        // cout<<memcmp(&current_sockets, &empty, sizeof(fd_set))<<endl;
        // cout<<memcmp(&ready_sockets, &empty, sizeof(fd_set))<<endl;
        // cout<<"FDSET: "<<FD_ISSET(i, &ready_sockets)<<endl; 


        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, &tv) < 0)
        {
            perror("Error");
            cout<<"Ary11a";
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
                    // recport = s->sin_port;
                    // recport1 = ntohs(s->sin_port);
                    // recport2 = htons(s->sin_port);
                    // recport3 = address.sin_port;
                    // recport4 = ntohs(address.sin_port);
                    // recport5 = htons(address.sin_port);
                        // inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
                    

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

                    // cout<<"buff:  "<<buffstr<<endl;

                    // if(buffstr[buffstr.size()-1] == '^'){
                    //     //not found
                    //     cout<<"!6"<<endl;
                    //     portofr = stoi(buffstr.substr(0,buffstr.size()-1));
                    //     count11++;
                    // }
                    // else if(buffstr.find('#') != std::string::npos){
                    //     //found
                    //     cout<<"!!!"<<endl;
                    //     filesofr = stoi(buffstr.substr(0,buffstr.size()-2));
                    //     // count11++;
                    // }
                    else if(buffstr[0]=='G' && buffstr[1]=='G' && buffstr[2]=='G'){
                        // cout<<"FFFFFFF"<<endl;
                        // filesofr++;
                        // size_t pos11 = buffstr.find("@");
                        // string filefd = buffstr.substr(0,pos11);
                        // // cout<<filefd<<endl;
                        // filesfound.push_back(filefd);
                        // depthofff.push_back(buffstr.substr(buffstr.size()-2,buffstr.size()-1));
                        buffstr = buffstr.substr(3,buffstr.size());
                        // size_t pos19 = buffstr.find("F");
                        // buffstr = buffstr.substr(0,pos19+1);
                        // cout<<"&&&&  "<<buffstr<<endl;

                        FILE *fp;
                         
                          size_t pos14 = buffstr.find("F")+1;
                          size_t pos15,pos13;
                          if(buffstr[pos14-3] == '.' && buffstr[pos14-4] == '@'){

                            pos13 = buffstr.find('$');
                            pos15 = buffstr.find('@');

                            string kk("Downloaded/");
                            string fff = pathtofile+kk;
                            mkdir(const_cast<char*>(fff.c_str()),0777);
                            string fff11 = pathtofile+kk+buffstr.substr(0,pos13);
                            filename33 = const_cast<char*>(fff11.c_str());
                            const char* filename = filename33;
                            cout<<fff<<endl;
                          // char buffer[1000];
                         
                            
                           
                            std::ifstream file(filename, std::ifstream::binary);
                            MD5_CTX md5Context;
                            MD5_Init(&md5Context);
                            char buf[1024 * 16];
                            while (file.good()) {
                                file.read(buf, sizeof(buf));
                                MD5_Update(&md5Context, buf, file.gcount());
                            }
                            unsigned char result[MD5_DIGEST_LENGTH];
                            MD5_Final(result, &md5Context);
                            std::stringstream md5string;
                            md5string << std::hex << std::setfill('0');
                            for (const auto &byte: result)
                                md5string << std::setw(2) << (int)byte;

                            if(filesfound.size() == 0){
                                fp = fopen(filename, "w");
                                fprintf(fp, "%s", buffstr.substr(pos14,buffstr.size()).c_str());
                                writefile(i, fp, true);
                                fclose(fp);
                                std::ifstream file(filename, std::ifstream::binary);
                                MD5_CTX md5Context;
                                MD5_Init(&md5Context);
                                char buf[1024 * 16];
                                while (file.good()) {
                                    file.read(buf, sizeof(buf));
                                    MD5_Update(&md5Context, buf, file.gcount());
                                }
                                unsigned char result[MD5_DIGEST_LENGTH];
                                MD5_Final(result, &md5Context);
                                std::stringstream md5string;
                                md5string << std::hex << std::setfill('0');
                                for (const auto &byte: result)
                                    md5string << std::setw(2) << (int)byte;

                                sleep(2);
                                hashofff.push_back(md5string.str());
                                filesfound.push_back(buffstr.substr(0,pos13));
                                // cout<<"AAAA:  "<<buffstr.substr(0,pos14-1)<<endl;
                                depthofff.push_back(buffstr.substr(0,pos14-1).substr(buffstr.substr(0,pos14-1).size()-1,buffstr.substr(0,pos14-1).size()));
                                // cout<<"DDD: "<<buffstr.substr(0,pos14-1).substr(buffstr.substr(0,pos14-1).size()-1,buffstr.substr(0,pos14-1).size())<<endl;
                                cout<<"!4"<<endl;
                                uqidoffilesfound.push_back(stoi(buffstr.substr(0,pos15).substr(pos13+1,buffstr.substr(0,pos15).size())));
                            }
                            else{
                                int c=0;
                                for(int p=0; p<filesfound.size(); p++){
                                    if(filesfound[p] == buffstr.substr(0,pos13)){
                                        cout<<"!3"<<endl;
                                        if(uqidoffilesfound[p] > stoi(buffstr.substr(0,pos15).substr(pos13+1,buffstr.substr(0,pos15).size()))){
                                            fp = fopen(filename, "w");
                                            fprintf(fp, "%s", buffstr.substr(pos14,buffstr.size()).c_str());
                                            writefile(i, fp, true);
                                            fclose(fp);
                                            std::ifstream file(filename, std::ifstream::binary);
                            MD5_CTX md5Context;
                            MD5_Init(&md5Context);
                            char buf[1024 * 16];
                            while (file.good()) {
                                file.read(buf, sizeof(buf));
                                MD5_Update(&md5Context, buf, file.gcount());
                            }
                            unsigned char result[MD5_DIGEST_LENGTH];
                            MD5_Final(result, &md5Context);
                            std::stringstream md5string;
                            md5string << std::hex << std::setfill('0');
                            for (const auto &byte: result)
                                md5string << std::setw(2) << (int)byte;

                                            hashofff[p] = md5string.str();
                                            filesfound[p] = buffstr.substr(0,pos13);
                                            depthofff[p] = buffstr.substr(0,pos14-1).substr(buffstr.substr(0,pos14-1).size()-1,buffstr.substr(0,pos14-1).size());
                                            uqidoffilesfound[p] = stoi(buffstr.substr(0,pos15).substr(pos13+1,buffstr.substr(0,pos15).size()));
                                        }
                                        else{
                                            fp = fopen(filename, "w");
                                            fprintf(fp, "%s", buffstr.substr(pos14,buffstr.size()).c_str());
                                            writefile(i, fp, false);
                                            fclose(fp);
                                        }
                                    }
                                    else{
                                        c++;
                                    }
                                }
                                if(c == filesfound.size()){
                                    fp = fopen(filename, "w");
                                    fprintf(fp, "%s", buffstr.substr(pos14,buffstr.size()).c_str());
                                    writefile(i, fp, true);
                                    fclose(fp);
                                    sleep(2);
                                    hashofff.push_back(md5string.str());
                                    filesfound.push_back(buffstr.substr(0,pos13));
                                    depthofff.push_back(buffstr.substr(0,pos14-1).substr(buffstr.substr(0,pos14-1).size()-1,buffstr.substr(0,pos14-1).size()));
                                    cout<<"!1"<<endl;
                                    uqidoffilesfound.push_back(stoi(buffstr.substr(0,pos15).substr(pos13+1,buffstr.substr(0,pos15).size())));
                                    cout<<"!2"<<endl;
                                }



                            }
                          }
                    }
                    else if(buffstr[buffstr.size()-1] == '(' && buffstr[buffstr.size()-3] == ')'){
                        size_t pos11 = buffstr.find("$");
                        string buffstr2 = buffstr.substr(0,pos11);
                        cout<<buffstr2<<endl;
                        cout<<"LLLLL: "<<buffstr.substr(pos11+1,buffstr.size()).substr(0,buffstr.substr(pos11+1,buffstr.size()).size()-3)<<endl;
                        portofr = stoi(buffstr.substr(pos11+1,buffstr.size()).substr(0,buffstr.substr(pos11+1,buffstr.size()).size()-3));
                        
                        for(int j=0; j<filesihave.size(); j++){
                        // cout<<"*******"<<filesihave[j]<<"********"<<endl;
                            if(filesihave[j] == buffstr2){
                            
                                string hhh = "GGG" + buffstr2 + "$" + to_string(myuniqueid) + "@.2F";
       
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

                                cout<<"SENT3 "<<hhh<<" to "<<portofr<<endl;

                                FILE *fp;
                                string filepath1(filesihave[j]);
                                string filepath2 = pathtofile + filepath1;
                                char *filename = const_cast<char*>(filepath2.c_str());
                                uint32_t sz;
                                int32_t sent_bytes;
                                fp = fopen(filename, "r");
                                if (fp == NULL) {
                                    perror("[-]Error in reading file.");
                                    exit(1);
                                }

                                int n;
                                char data[5000] = {0};
                                
                                sendfile(fp, sockfdsendfiles);
                            
                                close(sockfdsendfiles);

                            }
                        }
                    }

                    else if(buffstr[0]=='*' && buffstr[1]=='*' && buffstr[2]=='*'){
                        int c1=0;
                        buffstr = buffstr.substr(3,buffstr.size());
                        cout<<buffstr<<endl;
                        size_t pos = buffstr.find("+");
                        portofr = stoi(buffstr.substr(pos,buffstr.size()));
                        buffstr = buffstr.substr(0,pos);
                        // sleep(5);
                        // cout<<"Received req of file "<<buffstr<<" from "<<portofr<<endl;
                        for(int j=0; j<filesihave.size(); j++){
                        // cout<<"*******"<<filesihave[j]<<"********"<<endl;
                            if(filesihave[j] == buffstr){
                            
                                string hhh = "GGG" + buffstr + "$" + to_string(myuniqueid) + "@.1F";
       
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
                                cout<<"SENT 1"<<hhh<<" to "<<portofr<<endl;

                                FILE *fp;
                                string filepath1(filesihave[j]);
                                string filepath2 = pathtofile + filepath1;
                                char *filename = const_cast<char*>(filepath2.c_str());
                                uint32_t sz;
                                int32_t sent_bytes;
                                fp = fopen(filename, "r");
                                if (fp == NULL) {
                                    perror("[-]Error in reading file.");
                                    exit(1);
                                }

                                int n;
                                char data[5000] = {0};
                                
                                sendfile(fp, sockfdsendfiles);

                                close(sockfdsendfiles);
                            }
                            else{
                                c1++;
                            }
                        }
                        cout<<c1<<endl;
                        if(c1 == filesihave.size()){

                            for(int i=0; i<neighbours; i++){
                                // string myportinfo = to_string(portofr)+"^";
                                // cout<<"NP: "<<neighbourport[i]<<endl;
                                // sending(neighbourport[i], myportinfo);
                                // string myfilesinfo = to_string(myfiles)+".#";
                                // sending(neighbourport[i], myfilesinfo);
                                sending(neighbourport[i], buffstr+"$" + to_string(portofr)+").(");
                                cout<<"SENT 2"<<buffstr+"$" + to_string(myuniqueid)+").("<<" to "<<neighbourport[i]<<endl;

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

    namespace fs = std::filesystem;


    string path = argv[2];

    
    for (const auto & entry : fs::directory_iterator(path))
        {
            size_t pos = entry.path().u8string().find("/");
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
    cout<<"!66"<<endl;

    for(int i=0; i<neighbours; i++){
        neighbourid.push_back(stoi(textlbl[2].substr(0, textlbl[2].find(" "))));
        textlbl[2].erase(0, textlbl[2].find(" ") + 1);
        neighbourport.push_back(stoi(textlbl[2].substr(0, textlbl[2].find(" "))));
        textlbl[2].erase(0, textlbl[2].find(" ") + 1);
    }
    cout<<"!1gsrg"<<endl;

    myfiles = stoi(textlbl[3]);

    for(int i=4; i<(myfiles+4); i++){
        filename.push_back(textlbl[i]);
        cout<<textlbl[i]<<endl;
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

    // cout<<myport<<" "<<neighbourport[0]<<endl;

    // if ((sockfdsend = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    //     perror("socket");
    //     exit(1);
    // }

    
    // cout<<"Press 1 to send"<<endl;

    // int num;
    // cin>>num;
    // cout<<"MYUNQID: "<<myuniqueid<<endl<<endl;
    int num;
    cin>>num;

    

        for(int i=0; i<neighbours; i++){
            // string myportinfo = to_string(myport)+"^";
            // sending(neighbourport[i], myportinfo);
            // string myfilesinfo = to_string(myfiles)+".#";
            // sending(neighbourport[i], myfilesinfo);


            for(int j=0; j<myfiles; j++){
                
                if(int(filename[j][filename[j].size()-1]) == 13){
                        filename[j].pop_back();
                }
                string hhh = "***"+filename[j]+"+"+to_string(myport);
                sending(neighbourport[i], hhh);
                cout<<"SENT I want "<<filename[j]<<" to "<<neighbourport[i]<<endl;
            }
        }

        
    

    bool filefoundornot[filename.size()] = {false};
    int foundloc[filename.size()];
    int depth11[filename.size()]={0};
    for(int i=0; i<filename.size(); i++){
        foundloc[i] = 100000;
    }
    vector<string> depthofff;
    vector<string> filesfound;
    vector<string> hashofff;
    vector<int> portoffilesfound;
    vector<int> uqidoffilesfound;


    receiving(sockfdrec, filesihave, myport, myuniqueid, filename, neighbours, filesfound, argv[2], portoffilesfound, uqidoffilesfound,hashofff,depthofff, neighbourport);

    for(int l=0; l<filename.size(); l++){
        // cout<<foundloc[l]<<endl<<endl;
                            for(int j=0; j<filesfound.size(); j++){
                                // cout<<filesfound[j]<<endl;
                                // cout<<filesfound.size()<<endl;
                                // cout<<uqidoffilesfound.size()<<endl;
                                size_t pos12 = filesfound[j].size();
                                int coungg=0;
                                for(int h=0; h<filename[l].size(); h++){
                                    if(filename[l][h] == filesfound[j].substr(0,pos12)[h]){
                                        coungg++;
                                    }
                                }
      
                                if(int(filename[l][filename[l].size()-1]) == 13){
                                    filename[l].pop_back();
                                }
                            
                                if(int(filesfound[j][filesfound[j].size()-1]) == 13){
                                    filesfound[j].pop_back();
                                }
  
                                if(filename[l].size() <= coungg){
                                    filefoundornot[l] = true;
                                    cout<<"Found "<<filename[l]<<" at "<<uqidoffilesfound[j]<<" with MD5 "<<hashofff[j]<<" at depth "<<depthofff[j]<<endl;
                                    // cout<<foundloc[l]<<endl;
                                    // foundloc[l] = min(foundloc[l],stoi(filesfound[j].substr(pos12+1,filesfound[j].size())));
                                    // cout<<foundloc[l]<<endl;
                                    // cout<<stoi(filesfound[j].substr(pos12+1,filesfound[j].size()))<<endl;
                                }
                            }
                        }
    for(int l=0; l<filename.size(); l++){
        if(filefoundornot[l]){
            // cout<<"Found "<<filename[l]<<" at "<<uqidoffilesfound[l]<<endl;
        }
        else{
            cout<<"Not Found "<<filename[l]<<" at 0 with 0 at depth 0"<<endl;
    
        }
    }

    return 0;
}

