/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/12
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <map>

#include "Handlers.h"
#include "User.h"
#include "MessageSharedMemory.h"
#include "MessageSharedMemory.cc"
using namespace std;

#ifdef MSG_STRATEGY_PIPE
#include "MessagePipe.h"
#include "MessagePipe.cc"
MessagePoll<Message*> *msg_strategy = new MessagePipe<Message*>("msgpipe");
#endif

#ifdef MSG_STRATEGY_QUEUE
#include "MessageQueue.h"
#include "MessageQueue.cc"
MessagePoll<Message*> *msg_strategy = new MessageQueue<Message*>();
#endif

#ifdef MSG_STRATEGY_SHARED_MEMORY
MessagePoll<Message*> *msg_strategy = new MessageSharedMemory<Message*>("msgshm");
#endif

MessagePoll<map<int, int>>* clilist_shm = new MessageSharedMemory<map<int, int>>("clilist_shm");
map<int, int> *client_list = new map<int, int>();

int get_handler (Message* msg, int connfd) {

    printf("Received request: GET %s.\n", msg->fname);
    FILE * fp = fopen(msg->fname, "r");

    Message resp_msg;
    if (fp == NULL) {
        perror("open file");
        char *error_str = "Open file error";
        resp_msg.command = ERROR;
        resp_msg.fname[0] = '\0';
        resp_msg.data_size = strlen(error_str) + 1;
        send(connfd, &resp_msg, MESSAGE_LEN, 0);
        return -1;
    }

    struct stat st;
    stat(msg->fname, &st);

    strcpy(resp_msg.fname, msg->fname);
    resp_msg.command = GET;
    resp_msg.data_size = st.st_size;

    cout<<"send file size"<<endl;
    if (send(connfd, &resp_msg, MESSAGE_LEN, 0) == -1) {
        perror("send");
        fclose(fp);
        return -1;
    }
    
    char* buff = (char*)malloc(st.st_size);
    fread(buff, sizeof(char), st.st_size, fp);

    cout<<"send file data"<<endl;
    
    int rem_num = resp_msg.data_size / SOCKETIO_BUFFER_SIZE;
    int i = 0;
    for (; i < rem_num; i++) {
        if (send(connfd, buff + i * SOCKETIO_BUFFER_SIZE, SOCKETIO_BUFFER_SIZE, 0) == -1) {
            perror("send");
            free(buff);
            fclose(fp);
            return -1;
        }
    }
    
    int rem_bytes = resp_msg.data_size % SOCKETIO_BUFFER_SIZE;
    if (send(connfd, buff + (i - 1) * SOCKETIO_BUFFER_SIZE, rem_bytes, 0) == -1) {
        perror("send");
        free(buff);
        fclose(fp);
        return -1;
    }

    printf("Sent file %s(%d bytes) successfully\n", resp_msg.fname, resp_msg.data_size);
    free(buff);
    fclose(fp);
    return 0;
}

int put_handler (Message* msg, int connfd) {

    printf("Received request: PUT %s.\n", msg->fname);
    printf("%d bytes\n", msg->data_size);
    FILE * fp = fopen(msg->fname, "w+");

    Message resp_msg;
    if (fp == NULL) {
        perror("Create file");
        char *error_str = "Create file error";
        resp_msg.command = ERROR;
        resp_msg.fname[0] = '\0';
        resp_msg.data_size = strlen(error_str) + 1;
        send(connfd, &resp_msg, MESSAGE_LEN, 0);
        return -1;
    }

    fwrite(msg->data_ptr, sizeof(char), msg->data_size, fp);
    // TODO check if write successfully

    //resp_msg.command = PUT;
    //strcpy(resp_msg.fname, msg->fname);
    //resp_msg.data_size = 0;
    //if (send(connfd, &resp_msg, MESSAGE_LEN, 0) == -1) {
    //    perror("send");
    //    free(buff);
    //    fclose(fp);
    //    return -1;
    //}

    printf("PUT file %s(%d bytes) successfully\n", msg->fname, msg->data_size);
    fclose(fp);
    return 0;
}

int quit_handler (Message* msg, int connfd) {
}

void loop_dispatch () {

    while (true) {

        printf("loopdispatch is getting clilist\n");
        Message *resp_msg;
        map<int, int> clilist;
        printf("hahaha\n");
        clilist = clilist_shm->getMessage ();
        printf("loopdispatch is getting message\n");
        resp_msg = msg_strategy->getMessage ();

        int buff_len = MESSAGE_LEN + resp_msg->data_size;
        char* buff = (char*)malloc (buff_len);
        memcpy (buff, resp_msg, MESSAGE_LEN);
        memcpy (buff + MESSAGE_LEN, resp_msg->data_ptr, resp_msg->data_size);

        int connfd;
        if (IS_BROADCAST(resp_msg->to)) {
            map<int, int>::iterator it = clilist.begin();
            for (; it != clilist.end(); ++it) {
                connfd = it->second;
                send(connfd, buff, buff_len, 0);
            }
        } else {
            connfd = clilist[resp_msg->to];
            send(connfd, buff, buff_len, 0);
        }

    
        free (buff);
        if (resp_msg->data_ptr != NULL) {
            free (resp_msg->data_ptr);
        }
    }
}

int conn_handler (Message* msg, int connfd) {
    
    printf("User %s[%d] is connected\n", msg->fname, msg->from);

    map<int, int> temp_list;
    temp_list[msg->from] = connfd;
    clilist_shm->putMessage (temp_list);

    return 0;
}

/**
 * Another process call this function to read message
 */
int waitmsg_handler (Message* msg, int connfd) {

    printf("[SERVER] User[%d] is waiting for message\n", msg->from);
    Message *resp_msg;
    resp_msg = msg_strategy->getMessage ();
    int buff_len = MESSAGE_LEN + resp_msg->data_size;
    char* buff = (char*)malloc (buff_len);

    memcpy (buff, resp_msg, MESSAGE_LEN);
    memcpy (buff + MESSAGE_LEN, resp_msg->data_ptr, resp_msg->data_size);
    send(connfd, buff, buff_len, 0);
    
    free (buff);
    if (resp_msg->data_ptr != NULL) {
        free (resp_msg->data_ptr);
    }
    return 0;
}

int sendmsg_handler (Message* msg, int connfd) {

    printf("[SERVER] User %s[%d] send message: %s\n", msg->fname, msg->from, msg->data_ptr);
    msg_strategy->putMessage (msg);

    printf("put done!\n");
    return 0;
}

