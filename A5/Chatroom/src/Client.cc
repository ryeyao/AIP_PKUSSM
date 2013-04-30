/* 
 Just show a concurrent multi-connection client example.
 Copyright (C) 2006, Li Suke, School of Software,
 Beijing University
 This  is free software; you can redistribute it and/or
 modify it freely.

 This software  is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <wait.h>

#define MAX_LEN 1024
#include"Client.h"

using namespace std;

void sighandler(int signo) 
{
  int status;
  pid_t pid;

  while((pid = waitpid(-1, &status, WNOHANG)) > 0 ) {
    printf("process %d terminated\n",pid);
   
  }
}

int comm_get(int sockfd, char* fname) {
    cout<<"will get "<<fname<<endl;

    Message msg, resp_msg;
    msg.command = GET;
    strcpy(msg.fname, fname);
    msg.data_size = 0;

    cout<<"send file name"<<endl;
    if (send(sockfd, &msg, MESSAGE_LEN, 0)== -1){
       perror("send");
       exit(1);
    }	
    int number_bytes = 0;
    cout<<"get file size"<<endl;
    if ((number_bytes = recv(sockfd, &resp_msg, MESSAGE_LEN, 0))==-1){
       perror("recv");
       exit(1);
    }
        printf("Receving file %s (%d bytes)\n", resp_msg.fname, resp_msg.data_size);
        char* filebuf = (char*) malloc(resp_msg.data_size);
        cout<<"get file data"<<endl;
        int rem_num = resp_msg.data_size / SOCKETIO_BUFFER_SIZE;
        int i = 0;
        for (; i < rem_num; i++) {
            if ((number_bytes = recv(sockfd, filebuf + i * SOCKETIO_BUFFER_SIZE, SOCKETIO_BUFFER_SIZE, 0)) == -1) {
                perror("recv");
                exit(1);
            }
        }
        int rem_bytes = resp_msg.data_size % SOCKETIO_BUFFER_SIZE;
        if ((number_bytes = recv(sockfd, filebuf + (i - 1) * SOCKETIO_BUFFER_SIZE, rem_bytes, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        FILE* fp = fopen(resp_msg.fname, "w+");
        fwrite(filebuf, sizeof(char), resp_msg.data_size, fp);
        free(filebuf);
        fclose(fp);
        
        printf("File received successfully!)\n");
    
}

int comm_put(int sockfd, char* fname) {
    cout<<"will put "<<fname<<endl;

    struct stat st;
    stat(fname, &st);

    Message msg;
    msg.command = PUT;
    strcpy(msg.fname, fname);
    msg.data_size = st.st_size;

    FILE* fp = fopen(fname, "r");

    if (send(sockfd, &msg, MESSAGE_LEN, 0)== -1){
       perror("send");
       exit(1);
    }	
    char* buff = (char*)malloc(st.st_size);
    fread(buff, sizeof(char), st.st_size, fp);
    
    int rem_num = st.st_size / SOCKETIO_BUFFER_SIZE;
    int i = 0;
    for (; i < rem_num; i++) {
        if (send(sockfd, buff + i * SOCKETIO_BUFFER_SIZE, SOCKETIO_BUFFER_SIZE, 0) == -1) {
            perror("send");
            free(buff);
            fclose(fp);
            return -1;
        }
    }

    int rem_bytes = st.st_size % SOCKETIO_BUFFER_SIZE;
    if (send(sockfd, buff + (i - 1) * SOCKETIO_BUFFER_SIZE, rem_bytes, 0) == -1) {
        perror("send");
        free(buff);
        fclose(fp);
        return -1;
    }

    printf("PUT file %s(%d bytes) successfully\n", msg.fname, st.st_size);
    return 0;
}

int comm_QUIT(int sockfd) {

}

int comm_CONNECT(int sockfd) {
}

int console(int sockfd) {

    char console_comm[MAX_COMMAND_LEN];
    char fname[MAX_FILE_NAME_LEN];
    while(true) {

        cout<<"ftp>";
        cin>>console_comm;

        if (strlen(console_comm) == 4 
                && !strcmp(console_comm, "quit")
                || !strcmp(console_comm, "q")) {

            comm_QUIT(sockfd);
            break;

        } else if (strlen(console_comm) == 3
                && !strcmp(console_comm, "put")) {

            cin>>fname; 
            comm_put(sockfd, fname);  

        } else if (strlen(console_comm) == 3
                && !strcmp(console_comm, "get")) {

            cin>>fname;
            comm_get(sockfd, fname);  

        }
    }
}
int main(int argc, char *argv[])
{
   int sockfd;
   int conn_ret;
   struct sockaddr_in servaddr;
   char sendbuf[30]; 
   int i;
   const char *hello= "hello world!";
   char recvbuf[MAX_LEN];
   ssize_t number_bytes;
   int status;

   if(argc != 3) {
      printf("Usage:readcli <address> <port> \n");
      return 0;
   }

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_port = htons(atoi(argv[2]));
   inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    
   if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
       perror("sock");
       exit(1);
   }

   conn_ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
   if(conn_ret == -1){
       perror("connect");
       exit(1);
   }
   console(sockfd);
  //   comm_get(sockfd);
    //snprintf(sendbuf, sizeof(sendbuf), "%s,client:%d",hello,i);
   close(sockfd);
   exit(0);
   
   return 0;
}
