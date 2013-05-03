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

#include "MessageSharedMemory.h"

#define SHM_SIZE 4096
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
    void* temp;
    temp = (void*) shmat (shmid, NULL, 0);

    if ((int)temp == -1) {
        perror ("shmat");
    }
    //*T::iterator it = (*msg).begin();
    //for (it = (*msg).begin(); it != (*msg).end(); ++it) {
    //    (*temp_msg)[it->first] = it->second;
    //}
    //(*temp_msg).insert ((*msg).begin(), (*msg).end());

    T* temp_msg;
    temp_msg = new(temp) T;

    int testf = msg.begin()->first;
    int tests = msg.begin()->second;
    //(*temp_msg)[(*msg).begin()->first] = (*msg).begin()->second;
    (*temp_msg)[testf] = tests;

    atomic_store_explicit(&lock, false, memory_order_release);
}

template <class T>
T MessageSharedMemory<T>::getMessage () {
    while (atomic_exchange_explicit(&lock, true, memory_order_acquire))
        ;

    int shmid;
    printf("1\n");
    if ( (shmid = shmget (key, SHM_SIZE, IPC_CREAT)) == -1) {
        perror ("shmget");
    }
    printf("2\n");
    //FIXME this may cause seg fault
    T* temp_msg;
    temp_msg = (T*) shmat (shmid, NULL, 0);
    printf("3\n");
    printf("4\n");

    atomic_store_explicit(&lock, false, memory_order_release);
    printf("5\n");
    return *temp_msg;
}
