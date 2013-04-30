/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/12
 */

#ifndef HANDLERS_H
#define HANDLERS_H

#include "Server.h"

extern int get_handler (Message*, int);
extern int put_handler (Message*, int);
extern int quit_handler (Message*, int);
extern int chat_handler (Message*, int);

#endif
