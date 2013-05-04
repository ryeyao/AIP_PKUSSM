/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/08
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <iostream>
#include <map>

#include "Server.h"
#include "Handlers.h"
using namespace std;
#define BACKLOG 10

typedef int HDLR_FP(Message*, int);


map<Command, HDLR_FP*> hdlr_map;
int sockfd;

void sig_handler (int signo) {

    close (sockfd);
    //printf ("Server terminated.\n");
    SRVR_LOG("Server terminated.\n");
    exit (0);
}
/**
 * Put Message into a struct specified by macro e.g. MessageQueue, Pipe, SharedMemory
 */

int handle_msg(Message* msg, int connfd) {

    HDLR_FP* hdlr = hdlr_map[msg->command];
    return hdlr(msg, connfd);
}

int parse_msg (Message* msg, int connfd) {

    //printf("wait for command\n");
    SRVR_LOG ("wait for Command\n");
    // Parse header
    int number_bytes = recv(connfd, msg, MESSAGE_LEN, 0); 
    if (number_bytes <= 0) {
        perror("recv no data");
        return -1;
    }

    if (msg->data_size <= 0) {
        msg->data_size = 0;
        msg->data_ptr = NULL;
        return 0;
    }

    // Parse data
    char* buff = (char*)malloc(msg->data_size);
    int rem_num = msg->data_size / SOCKETIO_BUFFER_SIZE;
    int i = 0;
    for (; i < rem_num; i++) {
        if (recv(connfd, buff + i * SOCKETIO_BUFFER_SIZE, SOCKETIO_BUFFER_SIZE, 0) == -1) {
            perror("recv");
            free (buff);
            return -1;
        }
    }
    if (i == 0) {
        i = 1;
    }
    int rem_bytes = msg->data_size % SOCKETIO_BUFFER_SIZE;
    if (recv(connfd, buff + (i - 1) * SOCKETIO_BUFFER_SIZE, rem_bytes, 0) == -1) {
        perror("recv");
        free (buff);
        return -1;
    }
    msg->data_ptr = buff;
    return 0;
}

void* clithread (void* arg) {
    
    //printf("A client is connected.\n");
    SRVR_LOG("A client is connected.\n");
    int connfd = *((int *) arg);
    pthread_detach(pthread_self());

    while(true) {

        Message msg;
        if (parse_msg (&msg, connfd) != 0) {
            break;
        }

        if (handle_msg (&msg, connfd) != 0 ) {
            break;
        }
    }

    free(arg);
    close(connfd);
    pthread_exit(NULL);
}

void listen(int port) {

    int *connfd;

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

    printf("[SERVER] Server is listening on port %d\n", ntohs(tempaddr.sin_port));

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

    // Ftp service
    add_handler(GET, &get_handler);
    add_handler(PUT, &put_handler);

    // Chat service
    add_handler(CONN, &conn_handler);
    add_handler(SENDMSG, &sendmsg_handler);

    pthread_t pt;
    if (pthread_create(&pt, NULL, loop_dispatch, 0) != 0)  {
        perror("pthread_create");
        exit(0);
    }
    /*pid_t dispatch_child = fork();
    if (dispatch_child >= 0) {
        
        if (dispatch_child == 0) {
            loop_dispatch();
            exit(0);
        } else {
            //printf("dispatch process is started\n");
            SRVR_LOG("dispatch process is started\n");
        }
    } else {
        perror("loop_dispatch");
        exit(0);
    }
    */
    listen(port);
    //signal (SIGQUIT | SIGINT | SIGKILL | SIGHUP | SIGSTOP | SIGABRT | SIGTERM , sig_handler);
    signal (SIGINT, sig_handler);
}
