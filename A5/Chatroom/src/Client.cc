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

char my_name[MAX_FILE_NAME_LEN];
int my_id = -1;
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

int comm_quit(int sockfd) {

}

int comm_conn(int sockfd) {
    printf ("| Connecting...\n");
    Message msg;

    msg.command = CONN;
    strncpy (msg.fname, my_name, MAX_FILE_NAME_LEN);
    //sprintf (msg.fname, "%d", id);
    msg.data_size = 0;
    msg.data_ptr = NULL;
    msg.from = my_id;
    msg.to = my_id;

    if (send(sockfd, &msg, MESSAGE_LEN, 0) == -1) {
        perror("conn");
        return -1;
    }

    printf ("| Connected!\n");
    printf ("| \n");
    printf ("=================================================\n");
    printf ("| Hello %s, you have been connected to the server\n", my_name);
    printf ("| usage:\n");
    printf ("| \tsend|sd <id> <message>\n");
    printf ("| e.g\n");
    printf ("| console> send 1 Hi jack!\n");
    printf ("| [%s] says to [1]: Hi jack!\n", my_name);
    printf ("=================================================\n");
    return 0;
}

int comm_sendmsg (int sockfd, int id, char* content) {

    Message msg;

    msg.command = SENDMSG;
    strncpy (msg.fname, my_name, MAX_FILE_NAME_LEN);
    msg.data_size = strlen (content);
    msg.data_ptr = content;
    msg.from = my_id;
    msg.to = id;

    /*if (!strcmp (name, "all")) {
        msg.to = 0;
    } else {
        msg.to = 1;
    }
    */
    
    int buff_len = MESSAGE_LEN + msg.data_size;
    char *buff = (char*) malloc(buff_len);
    memcpy (buff, &msg, MESSAGE_LEN);
    memcpy (buff + MESSAGE_LEN, msg.data_ptr, msg.data_size);
    if (send(sockfd, buff, buff_len, 0) == -1) {
        perror("send");
        free(buff);
        return -1;
    }

    //printf("PUT file %s(%d bytes) successfully\n", msg.fname, st.st_size);
    //MESSAGE_DIS(my_name, msg.fname, msg.data_ptr);
    free(buff);
    return 0;
}

void* comm_waitmsg (void* sockfd) {

    int connfd = (int)sockfd;
    //printf("\ndebug: waiting message\n");
    while (true) {
        Message recv_msg;
        int number_bytes = 0;
        if ((number_bytes = recv(connfd, &recv_msg, MESSAGE_LEN, 0)) <= 0){
            perror("Server is down\nrecv");
            exit(0);
        }
        //printf("recv %d bytes\n", number_bytes);

        char* buff = (char*) malloc(recv_msg.data_size);
        if ((number_bytes = recv(connfd, buff, recv_msg.data_size, 0)) == -1){
            perror("recv");
            exit(1);
        }
        recv_msg.data_ptr = buff;
        char sender[MAX_FILE_NAME_LEN];
        sprintf(sender, "%s[id=%d]", recv_msg.fname, recv_msg.from);
        if (recv_msg.to == 0) {
            MESSAGE_DIS(sender, "all", recv_msg.data_ptr);
        } else {
            MESSAGE_DIS(sender, my_name, recv_msg.data_ptr);
        }
    }
}

int console(int sockfd) {


    cout<< "=================================================" <<endl;
    cout<< "| Please enter your nick name: ";
    cin>> my_name;
    cout<< "| " <<endl;
    cout<< "| Please enter your userid(0~255): ";
    cin>> my_id;
    cout<< "| " <<endl;
    comm_conn (sockfd);
    
    // Create read thread
    pthread_t pt;
    if (pthread_create(&pt, NULL, comm_waitmsg, (void*)sockfd) != 0) {
        perror("pthread_create");
        return -1;
    }

    char console_comm[MAX_COMMAND_LEN];
    char fname[MAX_FILE_NAME_LEN];
    while(true) {

        cout<<"console>";
        cin>>console_comm;

        if (strlen(console_comm) == 4 
                && !strcmp(console_comm, "quit")
                || !strcmp(console_comm, "q")) {

            comm_quit(sockfd);
            break;

        } else if (strlen(console_comm) == 3
                && !strcmp(console_comm, "put")) {

            cin>>fname; 
            comm_put(sockfd, fname);  

        } else if (strlen(console_comm) == 3
                && !strcmp(console_comm, "get")) {

            cin>>fname;
            comm_get(sockfd, fname);  

        } else if (strlen(console_comm) >= 2
                && (!strcmp(console_comm, "send")
                    || !strcmp(console_comm, "sd"))) {
            //char name[MAX_FILE_NAME_LEN];
            int toid = 0;
            char msg_content[MAX_DATA_SIZE];
            cin>>toid;
            gets (msg_content);
            comm_sendmsg (sockfd, toid, msg_content);
        } else {
            cout<<endl;
            continue;
        }
        cout<<endl;
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
      printf("Usage:Client <address> <port> \n");
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
