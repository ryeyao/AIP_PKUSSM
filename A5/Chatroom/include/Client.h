/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/08
 */

#ifndef FILECLIENT_H
#define FILECLIENT_H

#include "Message.h"

#define MAX_COMMAND_LEN 32

#define SOCKETIO_BUFFER_SIZE 512

#define MESSAGE_DIS(from, to, msg) printf("\n=================================================\n[%s] says to [%s]: %s\n=================================================\n[", from,  to, msg)

void sighandler (int);
int comm_get (int, char*);
int comm_put (int, char*);
int comm_quit (int);
int comm_conn (int);
void* comm_waitmsg (void*);
int comm_sendmsg (int, int, char*);
int console (int);
#endif
