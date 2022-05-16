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
// #include <CkCrypt2.h>
#define BACKLOG 100000 // how many pending connections queue will hold
#define MAX_LINE 4096


using namespace std;
void printMD5(unsigned char* md, long size = MD5_DIGEST_LENGTH) {
    for (int i=0; i<size; i++) {
        cout<< hex << setw(2) << setfill('0') << (int) md[i];
    }
}
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

void receiving(int server_fd, vector<string> filesihave, int myport, int myuniqueid, vector<string> filesiwant,int neighbours, vector<string> &filesfound, char* pathtofile, vector<int> &portoffilesfound, vector<int> &uqidoffilesfound, vector<string> &hashofff, vector<string> &ans1)
{
    struct sockaddr_in address;
    int valread;
    char buffer[4096];
    int addrlen = sizeof(address);
    fd_set current_sockets, ready_sockets;

    //Initialize my current set
    FD_ZERO(&current_sockets);
    FD_SET(server_fd, &current_sockets);
    int k = 0;
    int portofr,filesofr;
    // int portpos=0;
    int count11 = 0;
    time_t current_time;
    time_t current_time11;
    current_time = time(NULL);
    struct timeval tv;
    tv.tv_sec = 20;
    size_t pos13;
    char *filename33;
    int count55 = 0;

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
                    // cout<<"VALREAD: "<<valread<<endl;
                    buffer[valread] = '\0';
                    string buffstr(buffer);
                    //13 is ascii of carriage return which is coming in end in some cases
                    if(int(buffer[buffstr.size()-1]) == 13){
                        buffstr.pop_back();
                    } 

                    // cout<<"BUFF:  "<<buffstr<<endl;

                    // if(buffstr[buffstr.size()-1] == '^'){
                    //     //found
                    //     cout<<"BUFF1: "<<buffstr<<endl;
                    //     portofr = stoi(buffstr.substr(0,buffstr.size()-1));
                    //     count11++;
                    
                    // }
                    // else if(buffstr.find('#') != std::string::npos){
                    //     //found
                    //     if(buffstr[buffstr.size()-1] == '#' && buffstr[buffstr.size()-2] == '.'){
                    //         filesofr = stoi(buffstr.substr(0,buffstr.size()-2));
                    //     // count11++;
                    //     }
                    // }
                    // else if(buffstr.substr(buffstr.size()-1,buffstr.size()) == "F"){
                    //     // cout<<"FFFFFFF"<<endl;
                    //     // filesofr++;
                    //     cout<<"BUFF2: "<<buffstr<<endl;
                    //     size_t pos11 = buffstr.find("@");
                    //     string filefd = buffstr.substr(0,pos11);
                    //     // cout<<filefd<<endl;
                    //     filesfound.push_back(filefd);
                    // }

                    if(buffstr[0] == '}' && buffstr[1] == '{'){


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
                        
                        string aa="Connected to "+id1+" with unique-ID "+uqid1+" on port "+to_string(portofr);
                        ans1.push_back(aa);


                        for(int j=0; j<filesihave.size(); j++){
                        // cout<<"*******"<<filesihave[j]<<"********"<<endl;
                        if(filesihave[j] == buffstr.substr(2,buffstr.size())){
                            // filesfound.push_back(buffstr.substr(2,buffstr.size()));
                            // portoffilesfound.push_back(portofr);
                            // uqidoffilesfound.push_back(myuniqueid);
                            string hhh = buffstr.substr(2,buffstr.size()) + "$" + to_string(myuniqueid) + "@.F";
                            

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

                    else{

                        // cout<<"BUFF4: "<<buffstr<<endl;

                    
                          FILE *fp;
                         
                          size_t pos14 = buffstr.find("F")+1;
                          size_t pos15;
                          if(buffstr[pos14-2] == '.' && buffstr[pos14-3] == '@'){

                            pos13 = buffstr.find('$');
                            pos15 = buffstr.find('@');

                            string kk("Downloaded/");
                            string fff = pathtofile+kk;
                            mkdir(const_cast<char*>(fff.c_str()),0777);
                            string fff11 = pathtofile+kk+buffstr.substr(0,pos13);
                            filename33 = const_cast<char*>(fff11.c_str());
                            const char* filename = filename33;
      
                          // char buffer[1000];
                         
                            
                           
                            // std::ifstream file(filename, std::ifstream::binary);
                            // MD5_CTX md5Context;
                            // MD5_Init(&md5Context);
                            // char buf[1024 * 16];
                            // while (file.good()) {
                            //     file.read(buf, sizeof(buf));
                            //     MD5_Update(&md5Context, buf, file.gcount());
                            // }
                            // unsigned char result[MD5_DIGEST_LENGTH];
                            // MD5_Final(result, &md5Context);
                            // std::stringstream md5string;
                            // md5string << std::hex << std::setfill('0');
                            // for (const auto &byte: result)
                            //     md5string << std::setw(2) << (int)byte;

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
                                uqidoffilesfound.push_back(stoi(buffstr.substr(0,pos15).substr(pos13+1,buffstr.substr(0,pos15).size())));
                            }
                            else{
                                int c=0;
                                for(int p=0; p<filesfound.size(); p++){
                                    if(filesfound[p] == buffstr.substr(0,pos13)){
                                        if(uqidoffilesfound[p] > stoi(buffstr.substr(0,pos15).substr(pos13+1,buffstr.substr(0,pos15).size()))){
                                            fp = fopen(filename, "w");
                                            fprintf(fp, "%s", buffstr.substr(pos14,buffstr.size()).c_str());
                                            writefile(i, fp, true);
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
                                            fclose(fp);
                                            hashofff[p] = md5string.str();
                                            filesfound[p] = buffstr.substr(0,pos13);
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
                                    fclose(fp);
                                    sleep(2);
                                    hashofff.push_back(md5string.str());
                                    filesfound.push_back(buffstr.substr(0,pos13));
                                    uqidoffilesfound.push_back(stoi(buffstr.substr(0,pos15).substr(pos13+1,buffstr.substr(0,pos15).size())));

                                }



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

    vector<string> ans1;



    

    for(int i=0; i<neighbours; i++){
        neighbourid.push_back(stoi(textlbl[2].substr(0, textlbl[2].find(" "))));
        textlbl[2].erase(0, textlbl[2].find(" ") + 1);
        neighbourport.push_back(stoi(textlbl[2].substr(0, textlbl[2].find(" "))));
        textlbl[2].erase(0, textlbl[2].find(" ") + 1);
    }

    myfiles = stoi(textlbl[3]);

    for(int i=4; i<(myfiles+4); i++){
        filename.push_back(textlbl[i]);
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


        for(int i=0; i<neighbours; i++){
            // string myportinfo = to_string(myport)+"^";
            // sending(neighbourport[i], myportinfo);
            // string myfilesinfo = to_string(myfiles)+".#";
            // sending(neighbourport[i], myfilesinfo);

            for(int j=0; j<myfiles; j++){
                if(int(filename[j][filename[j].size()-1]) == 13){
                        filename[j].pop_back();
                } 
                string hhh = "}{"+filename[j]+"-"+to_string(myport)+"+"+to_string(myuniqueid)+"*"+to_string(myid);;
                sending(neighbourport[i], hhh);
            }
            sleep(5);
        }
    sort(filesihave.begin(), filesihave.end());
    for(int i=0; i<filesihave.size(); i++){
        cout<<filesihave[i]<<endl;
    }

    if(neighbours==0){
        for(int i=0; i<filename.size(); i++){
            string aa = "Found "+filename[i]+" at 0 with MD5 0 at depth 0";
            ans1.push_back(aa);
        }
        sort(ans1.begin(), ans1.end());
        for(int i=0; i<ans1.size(); i++){
            cout<<ans1[i]<<endl;
        }
        return 0;
    }


    bool filefoundornot[filename.size()] = {false};
    int foundloc[filename.size()];
    for(int i=0; i<filename.size(); i++){
        foundloc[i] = 100000;
    }
    vector<string> filesfound;
    vector<string> hashofff;
    vector<int> portoffilesfound;
    vector<int> uqidoffilesfound;
    vector<string> ans;
    



    receiving(sockfdrec, filesihave, myport, myuniqueid, filename, neighbours, filesfound, argv[2], portoffilesfound, uqidoffilesfound,hashofff, ans);


    for(int l=0; l<filename.size(); l++){

                            for(int j=0; j<filesfound.size(); j++){

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
                                    string aa="Found "+filename[l]+" at "+to_string(uqidoffilesfound[j])+" with MD5 "+hashofff[j]+" at depth 1";
                                    ans1.push_back(aa);

                                }
                            }
                        }
    for(int l=0; l<filename.size(); l++){
        if(filefoundornot[l]){
        }
        else{
            string aa = "Not Found "+filename[l]+" at 0 with 0 at depth 0";
            ans1.push_back(aa);
    
        }
    }
    sort(ans.begin(), ans.end());
    sort(ans1.begin(), ans1.end());
    for(int i=0; i<ans.size()-1; i++){
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

