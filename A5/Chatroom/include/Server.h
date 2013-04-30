/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/08
 */

#ifndef FILESERVER_H
#define FILESERVER_H

#include "Message.h"

#define SOCKETIO_BUFFER_SIZE 512

void parse_msg(Message, char*);
int handle_msg(Message*, int);
void* clithread(void*);
void listen(int);
void add_handler(Command, void*);

#endif
