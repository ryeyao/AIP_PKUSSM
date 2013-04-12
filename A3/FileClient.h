/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/08
 */

#ifndef FILECLIENT_H
#define FILECLIENT_H

#define MAX_FILE_NAME_LEN 32
#define MESSAGE_LEN sizeof(Message)
#define MAX_FILE_SIZE 4096

#define MAX_COMMAND_LEN 32

#define SOCKETIO_BUFFER_SIZE 512

typedef enum Command {
    PUT,
    GET,
    LS,
    CD,
    ERROR,
    QUIT,
    CONNECT
}Command;

typedef struct Message {
    Command command; // command index
    char fname[MAX_FILE_NAME_LEN];
    int data_size; // file data if type is trans 
               // error string size if type is error
               // -1 if type is command
}Message;

#endif
