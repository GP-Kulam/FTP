#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fstream>
#include <dirent.h>
#define port 7777
#define SIZE 1024
using namespace std;
void displayAllFiles(){
    DIR *di;
    char *ptr1,*ptr2;
    int retn;
    struct dirent *dir;
    di = opendir("."); //specify the directory name
    cout << "================================================" << endl;
    cout << "   Choose a file you want to upload " << endl;
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
    write_res_file.open(file_name, ios::out);
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
void getAllFilesNames(int sock_fd, char *req){
    send(sock_fd, req, sizeof(req), 0);
    char files_names[1024];
    recv(sock_fd, files_names, sizeof(files_names), 0);
    cout << "================================================" << endl;
    cout << "   The files available for download " << endl;
    cout << "================================================" << endl;
    int i = 0;
    int num = 1;
    cout << "[" << num << "] ";
    while(files_names[i] != '\0'){
        if(files_names[i] == ' ') {
            num++;
            cout << "\t";
            cout << "[" << num << "] ";
        }
        cout << files_names[i];
        i++;
    }
    cout << endl;
}
void download(int client_sock_fd, char *req){
    char s[SIZE];
    string temp;
    cout << "Enter the file name you want to download: ";
    cin >> temp;
    int i = 0;
    for(auto c: temp){
        s[i++] = c;
    }
    s[i] = '\0';
    send(client_sock_fd, s, sizeof(s), 0);
    write_file(s, client_sock_fd);
}
void uploadFile(int sock_fd, char *req){
    displayAllFiles();
    string temp;
    cout << "Enter the file name you want to upload: ";
    cin >> temp;
    int i = 0;
    char up_file_name[20];
    for(auto c: temp){
        up_file_name[i++] = c;
    }
    up_file_name[i] = '\0';
    send(sock_fd, up_file_name, sizeof(up_file_name), 0);
    send_file(up_file_name, sock_fd);
}
int main(){
    int client_sock_fd;
    struct sockaddr_in client_addr;
    client_sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client_sock_fd, (struct sockaddr *)&client_addr, sizeof(client_addr));
    cout << "Connected" << endl;

    // char s[SIZE];
    // cout << "Enter the file name you want to download: ";
    // cin.getline(s, SIZE);
    // send(client_sock_fd, s, sizeof(s), 0);
    // write_file(s, client_sock_fd);
    char choice;
    cout << "===================================================================" << endl;
    cout << "Which operation you want to do?" << endl;
    cout << "(1) Show Files\t (2) Upload a File\t (3) Download a File" << endl;
    cout << "===================================================================" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    switch(choice){
        case '1': 
        system("clear");
        getAllFilesNames(client_sock_fd, &choice);
        break;
        case '2': 
        system("clear");
        send(client_sock_fd, &choice, sizeof(choice), 0);
        uploadFile(client_sock_fd, &choice);
        break;
        case '3': 
        system("clear");
        send(client_sock_fd, &choice, sizeof(choice), 0);
        download(client_sock_fd, &choice);
        displayAllFiles();
        default: return 0;
    }
    return 0;
}