/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/29
 */

#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "MessageSharedMemory.h"
#include "User.h"

#define SHM_SIZE 32*sizeof(User)
#define HAVE_MSGHDR_MSG_CONTROL
template <class T>
struct msghdr MessageSharedMemory<T>::write_fd (void *ptr, size_t nbytes, int sendfd) {

    struct msghdr msg;
    struct iovec iov[1];

#ifdef HAVE_MSGHDR_MSG_CONTROL
    union {
        struct cmsghdr cm;
        char control[CMSG_SPACE(sizeof(int))];
    } control_un;
    struct cmsghdr *cmptr;

    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un.control);
    bzero(msg.msg_control, sizeof(control_un.control));

    cmptr = CMSG_FIRSTHDR(&msg);
    cmptr->cmsg_len = CMSG_LEN(sizeof(int));
    cmptr->cmsg_level = SOL_SOCKET;
    cmptr->cmsg_type = SCM_RIGHTS;
    *((int*) CMSG_DATA(cmptr)) = sendfd;
#else
    msg.msg_accrights = (caddr_t) &sendfd;
    msg.msg_accrightslen = sizeof (int);
#endif
    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    iov[0].iov_base = ptr;
    iov[0].iov_len = nbytes;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    
    return msg;
}
template <class T>
void MessageSharedMemory<T>::read_fd (struct msghdr* fdhdr, void *ptr, size_t nbytes, int *recvfd) {
    struct msghdr msg = *fdhdr;
    struct iovec iov[1];
    ssize_t n;

#ifdef HAVE_MSGHDR_MSG_CONTROL
    union {
        struct cmsghdr cm;
        char control[CMSG_SPACE(sizeof(int))];
    } control_un;
    struct cmsghdr *cmptr;

    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof (control_un.control);
#else
    int newfd;

    msg.msg_accrights = (caddr_t) &newfd;
    msg.msg_accrightslen = sizeof(int);
#endif

    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    iov[0].iov_base = ptr;
    iov[0].iov_len = nbytes;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

#ifdef HAVE_MSGHDR_MSG_CONTROL
    if ((cmptr = CMSG_FIRSTHDR(&msg)) != NULL && cmptr->cmsg_len == CMSG_LEN(sizeof(int))) {
        if (cmptr->cmsg_level != SOL_SOCKET)
            perror ("control level != SOL_SOCKET");
        if (cmptr->cmsg_type != SCM_RIGHTS)
            perror ("control type != SCM_RIGHTS");
        *recvfd = *((int*) CMSG_DATA(cmptr));
    } else {
        *recvfd = -1;
    }
#else
    if (msg.msg_accrightslen == sizeof(int))
        *recvfd = newfd;
    else
        *recvfd = -1;
#endif
    
}

template <class T>
MessageSharedMemory<T>::MessageSharedMemory (char* ipc_name) {

    int fd = open (ipc_name, O_CREAT, 0600); 
    close (fd);
    
    if ((key = ftok (ipc_name, 10)) != 0) {
        perror ("ftok");
    }
    lock = ATOMIC_VAR_INIT(false);
}

template <class T>
MessageSharedMemory<T>::~MessageSharedMemory () {
}

template <class T>
void MessageSharedMemory<T>::putMessage (T msg) {
    while (atomic_exchange_explicit(&lock, true, memory_order_acquire))
        ;

    int shmid;
    if ( (shmid = shmget(key, SHM_SIZE, IPC_EXCL | 0600)) == -1) {
        perror ("shmget");
    }
    T temp_msg;
    temp_msg = (T) shmat (shmid, NULL, 0);

    if ((int)temp_msg == -1) {
        perror ("shmat");
    }
    char c = '\0';
    msg->fdhdr = write_fd(&c, 1, msg->connfd);
    memcpy (temp_msg, msg, SHM_SIZE);

    atomic_store_explicit(&lock, false, memory_order_release);
}

template <class T>
T MessageSharedMemory<T>::getMessage () {
    while (atomic_exchange_explicit(&lock, true, memory_order_acquire))
        ;

    int shmid;
    if ( (shmid = shmget (key, SHM_SIZE, IPC_CREAT)) == -1) {
        perror ("shmget");
    }
    //FIXME this may cause seg fault
    T temp_msg;
    temp_msg = (T) shmat (shmid, NULL, 0);
    char c = 0;
    int connfd = 0;
    read_fd(&temp_msg->fdhdr, &c, 1, &connfd);
    if (connfd > 0) {
        temp_msg->connfd = connfd;
    }

    atomic_store_explicit(&lock, false, memory_order_release);
    return temp_msg;
}

