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
#include"FileServer.h"

void sighandler(int signo) 
{
  int status;
  pid_t pid;

  while((pid = waitpid(-1, &status, WNOHANG)) > 0 ) {
    printf("process %d terminated\n",pid);
   
  }
}

int comm_get(int sockfd) {
    Message msg, resp_msg;
    msg.type = 0;
    msg.command = GET;
    strcpy(msg.fname, "hello");
    msg.data_size = 0;

    if (send(sockfd, &msg, sizeof(Message), 0)== -1){
       perror("send");
       exit(1);
    }	
    int number_bytes = 0;
    if ((number_bytes = recv(sockfd, &resp_msg, MESSAGE_LEN, 0))==-1){
       perror("recv");
       exit(1);
    }else{
        printf("Receving file %s (%d bytes)\n", resp_msg.fname, resp_msg.data_size);
        char* filebuf = (char*) malloc(resp_msg.data_size);
        if ((number_bytes = recv(sockfd, filebuf, resp_msg.data_size, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        FILE* fp = fopen(resp_msg.fname, "w+");
        fwrite(filebuf, sizeof(char), resp_msg.data_size, fp);
        free(filebuf);
        fclose(fp);
        
        printf("File received successfully!)\n");
    }
}

int comm_put(int sockfd) {
    char* fname = "put_hello";
    struct stat st;
    stat(fname, &st);

    Message msg, resp_msg;
    msg.type = 0;
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
    if (send(sockfd, buff, st.st_size, 0) == -1) {
        perror("send");
        free(buff);
        fclose(fp);
        return -1;
    }

    printf("PUT file %s(%d bytes) successfully\n", resp_msg.fname, st.st_size);
    return 0;
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
   pid_t pid;
   signal(SIGCHLD,sighandler);

   if(argc != 3) {
      printf("Usage:readcli <address> <port> \n");
      return 0;
   }

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_port = htons(atoi(argv[2]));
   inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

   for (i=0; i<1; i++) {

     if((pid = fork()) == 0) {
    
       if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
          perror("sock");
          exit(1);
       }

       conn_ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
       if(conn_ret == -1){
          perror("connect");
          exit(1);
       }
        comm_put(sockfd);  
  //      comm_get(sockfd);
       //snprintf(sendbuf, sizeof(sendbuf), "%s,client:%d",hello,i);
       close(sockfd);
       exit(0);
     }
   
   }
   return 0;
}
