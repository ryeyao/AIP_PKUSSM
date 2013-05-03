/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/29
 */
#ifndef COMMAND_H
#define COMMAND_H

typedef enum Command {
    PUT,
    GET,
    LS,
    CD,
    ERROR,
    QUIT,
    CONN,
    WAITMSG,
    SENDMSG
} Command;

#endif
