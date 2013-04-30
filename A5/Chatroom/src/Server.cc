/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/08
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<iostream>
#include<map>

#include "Server.h"
#include "Handlers.h"

#define BACKLOG 10
typedef int HDLR_FP(Message*, int);


using namespace std;
map<Command, HDLR_FP*> hdlr_map;

/**
 * Put Message into a struct specified by macro e.g. MessageQueue, Pipe, SharedMemory
 */

int handle_msg(Message* msg, int connfd) {

    HDLR_FP* hdlr = hdlr_map[msg->command];
    return hdlr(msg, connfd);
}

void* clithread (void* arg) {
    
    printf("A client is connected.\n");
    int connfd = *((int *) arg);
    pthread_detach(pthread_self());

    while(true) {

        printf("wait for command\n");
        Message msg;
        int number_bytes = recv(connfd, &msg, MESSAGE_LEN, 0); 
        if (number_bytes <= 0) {
            perror("recv");
            free(arg);
            close(connfd);
            pthread_exit(NULL);
        }
        cout<<"command received "<<number_bytes<<" bytes"<<endl;

        if (handle_msg(&msg, connfd) == -1 ) {
            break;
        }
    }

    free(arg);
    close(connfd);
    pthread_exit(NULL);
}

void listen(int port) {

    int *connfd;
    int sockfd;

    struct sockaddr_in servaddr;
    struct sockaddr_in tempaddr;
    struct sockaddr_in cliaddr;

    socklen_t clilen;
    socklen_t templen;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    templen = sizeof(struct sockaddr);
    if (getsockname(sockfd, (struct sockaddr *)&tempaddr, &templen) == -1) {
        perror("getsockname");
        exit(1);
    }

    printf("Server is listening on port %d\n", ntohs(tempaddr.sin_port));

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    for (;;) {
        pthread_t pt;
        clilen = sizeof(cliaddr);
        connfd = (int *) malloc(sizeof(int));
        if ((*connfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen)) == -1) {
            if (errno == EINTR) {
                perror("accept");
                continue;
            } else {
                exit(1);
            }
        }

        if (pthread_create(&pt, NULL, clithread, connfd) != 0) {
            perror("pthread_create");
            break;
        }
    }
}

void add_handler (Command cmd, HDLR_FP* handler) {
    hdlr_map[cmd] = handler;
}

int main(int argc, char** argv) {
    char usage[] = "usage: Server -p <port>";
    if (argc < 1) {
        perror(usage);
        exit(1);
    }

    int port = 0;

    int i = 1;
    for (; i < argc; i++) {

        if (!strcmp("-p", argv[i])) {
            port = atoi(argv[i+1]);
            i++;
            continue;
        }
    }

    add_handler(GET, &get_handler);
    add_handler(PUT, &put_handler);
    add_handler(CHAT, &chat_handler);
    listen(port);

    
}
