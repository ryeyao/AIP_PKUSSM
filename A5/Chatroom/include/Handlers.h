/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/12
 */

#ifndef HANDLERS_H
#define HANDLERS_H

#include "MessagePoll.h"
#include "Message.h"
#define SOCKETIO_BUFFER_SIZE 512

extern MessagePoll<Message*> *msg_strategy;

extern int get_handler (Message*, int);
extern int put_handler (Message*, int);
extern int quit_handler (Message*, int);
extern void loop_dispatch ();
extern int conn_handler (Message*, int);
extern int waitmsg_handler (Message*, int);
extern int sendmsg_handler (Message*, int);

#endif
