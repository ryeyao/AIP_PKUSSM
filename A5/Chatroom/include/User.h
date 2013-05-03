/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/12
 */

#ifndef USER_H
#define USER_H

//#include <sys/types.h>
//#include <sys/socket.h>
#include <netinet/in.h>
#define MAX_USER_NAME_LEN 16

typedef struct User {

    int id;
    int connfd;
    sockaddr_in addr;
    char name[MAX_USER_NAME_LEN];

}User;

#endif

