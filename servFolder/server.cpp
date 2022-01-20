#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fstream>
#include <dirent.h>
#include <vector>
#define port 7777
#define SIZE 1024
using namespace std;

bool file_exist(char *name)
{
    ifstream file(name);
    if(!file)            
        return false;    
    else                 
        return true;     
}
void displayAllFiles(){
    DIR *di;
    char *ptr1,*ptr2;
    int retn;
    struct dirent *dir;
    di = opendir("."); //specify the directory name
    cout << "================================================" << endl;
    cout << "   The files available for the download " << endl;
    cout << "================================================" << endl;
    if (di)
    {
        int i = 0;
        while ((dir = readdir(di)) != NULL)
        {
            ptr1=strtok(dir->d_name,".");
            ptr2=strtok(NULL,".");
            if(ptr2!=NULL)
            {
                retn=strcmp(ptr2,"txt");
                if(retn==0)
                {
                    i++;
                    cout << "[" << i << "]" << ptr1 << ".txt\t";
                }
            }
        }
        closedir(di);
    }
    cout << endl;
}
void write_file(string file_name, int sock_fd){
    char data[SIZE];
    recv(sock_fd, data, sizeof(data), 0);
    if(data[0] == '0') {
        cout << "Requested file does not exist on the server!" << endl;
        return;
    }
    cout << "Please wait downloading..." << endl;
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 100000; j++){
            for(int k = 0; k < 1000; k++){

            }
        }
        cout << "* " << flush;
    }
    cout << endl;
    ofstream write_res_file;
    write_res_file.open("res.txt", ios::out);
    write_res_file << data;
    cout << "File succesfully downloaded!" << endl;
}
void send_file(string req_file, int client_sock_fd){
    ifstream read_req_file;
    read_req_file.open(req_file, ios::in);
    char data[SIZE];
    read_req_file.getline(data, SIZE);
    send(client_sock_fd, data, sizeof(data), 0);
}
void sendAllFileNames(int sock_fd){
    DIR *di;
    char *ptr1,*ptr2;
    int retn;
    struct dirent *dir;
    di = opendir("."); //specify the directory name
    vector <string> files;
    if (di)
    {
        int i = 0;
        while ((dir = readdir(di)) != NULL)
        {
            ptr1=strtok(dir->d_name,".");
            ptr2=strtok(NULL,".");
            if(ptr2!=NULL)
            {
                retn=strcmp(ptr2,"txt");
                if(retn==0)
                {
                    string temp = string(ptr1) + ".txt";
                    files.push_back(temp);
                }
            }
        }
        closedir(di);
    }
    char file_names[1024];
    int k = 0;
    int vsize = files.size();
    int cnt = 0;
    for(auto f: files){
        cnt++;
        for(char c: f){
            file_names[k++] = c;
        }
        if(cnt != vsize)
        file_names[k++] = ' ';
    }
    send(sock_fd, file_names, sizeof(file_names), 0);
    cout << endl;
}
void response(int client_sock_fd){
    char req_file[10];
    recv(client_sock_fd, req_file, sizeof(req_file), 0);
    cout << "Request recieved for the file ";
    int i = 0;
    while(req_file[i] != '\0'){
        cout << req_file[i];
        i++;
    }
    cout << endl;
    if(file_exist(req_file)){
        send_file(req_file, client_sock_fd);
        cout << "File sent succesfully" << endl;
    } else {
        send(client_sock_fd, "0", sizeof("0"), 0);
        cout << "Requested file doesn't exist on this server!" << endl;
    }
}
int main(){
    int serv_sock_fd, client_sock_fd;
    struct sockaddr_in serv_addr, client_addr;
    serv_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    // displayAllFiles();
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    bind(serv_sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    listen(serv_sock_fd, 5);
    cout << "Server is listening at port " << port << endl;

    socklen_t client_len = sizeof(client_addr);
    client_sock_fd = accept(serv_sock_fd, (struct sockaddr *)&client_addr, &client_len);
    if(client_sock_fd < 0) cout << "Error" << endl;
    else cout << "Connected.." << endl;
    char req[1];
    recv(client_sock_fd, req, sizeof(req), 0);
    switch(req[0]){
        case '1': sendAllFileNames(client_sock_fd);
        break;
        case '3': response(client_sock_fd);
        break;
    }
    return 0;
}