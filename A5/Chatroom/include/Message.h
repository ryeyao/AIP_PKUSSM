/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/29
 */
#ifndef MESSAGE_H
#define MESSAGE_H

#include "Command.h"

#define MAX_FILE_NAME_LEN 32
#define MESSAGE_LEN sizeof(Message)
#define MAX_FILE_SIZE 4096
#define MAX_DATA_SIZE 512

#define IS_BROADCAST(id) id?false:true
typedef struct Message {
    Command command; // command index
    char fname[MAX_FILE_NAME_LEN];
    int data_size; // file data if type is trans 
               // error string size if type is error
               // -1 if type is command
    char* data_ptr;
    unsigned int from;
    unsigned int to;
}Message;

#endif
