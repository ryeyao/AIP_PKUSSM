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


#include "Handlers.h"
using namespace std;

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

    char* buff = (char*)malloc(msg->data_size);
    int rem_num = msg->data_size / SOCKETIO_BUFFER_SIZE;
    int i = 0;
    for (; i < rem_num; i++) {
        if (recv(connfd, buff + i * SOCKETIO_BUFFER_SIZE, SOCKETIO_BUFFER_SIZE, 0) == -1) {
            perror("recv");
            fclose(fp);
            return -1;
        }
    }
    int rem_bytes = resp_msg.data_size % SOCKETIO_BUFFER_SIZE;
    if (recv(connfd, buff + (i - 1) * SOCKETIO_BUFFER_SIZE, rem_bytes, 0) == -1) {
        perror("recv");
        fclose(fp);
        return -1;
    }
    
    fwrite(buff, sizeof(char), msg->data_size, fp);
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
    free(buff);
    fclose(fp);
    return 0;
}

int quit_handler (Message* msg, int) {
}

int chat_handler (Message* msg, int) {
}

