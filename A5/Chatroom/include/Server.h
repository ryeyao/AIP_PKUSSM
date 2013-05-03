/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/08
 */

#ifndef FILESERVER_H
#define FILESERVER_H

#define SRVR_LOG(arg) printf("[SERVER] %s", arg)
#include "Message.h"

void parse_msg(Message, char*);
int handle_msg(Message*, int);
void* clithread(void*);
void listen(int);
void add_handler(Command, void*);

#endif
