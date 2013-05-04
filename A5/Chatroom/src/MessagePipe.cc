/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/29
 */

#include <MessagePipe.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

template <class T>
MessagePipe<T>::MessagePipe (char* fifoname) {

    this->fifoname = fifoname;
    if (mkfifo (fifoname, 0600) < 0) {
        perror("mkfifo");
    }
    lock = ATOMIC_VAR_INIT(false);
};

template <class T>
MessagePipe<T>::~MessagePipe () {
};

template <class T>
void MessagePipe<T>::putMessage (T msg) {

    int fd = 0;
    //printf("before write open\n");
    if ((fd = open (fifoname, O_WRONLY)) < 0) {
        perror("write open fifo");
        atomic_store_explicit(&lock, false, memory_order_release);
        return;
    }

    int total_size = sizeof(T) + msg.data_size;
    char* buffer = new char[total_size];

    memcpy (buffer, &msg, sizeof(T)); // Write header
    memcpy (buffer + sizeof(T), msg.data_ptr, msg.data_size); // Write body

    while (atomic_exchange_explicit(&lock, true, memory_order_acquire))
        ;
    //printf("total size is %d\n", total_size);
    write (fd, buffer, total_size); 
    //printf("write buffer %s\n", msg.data_ptr);

    close (fd);
    delete[] buffer;
    atomic_store_explicit(&lock, false, memory_order_release);
};

template <class T>
T MessagePipe<T>::getMessage () {
    int fd = 0;
    //printf("before read open\n");
    if ((fd = open (fifoname, O_RDONLY)) < 0) {
        perror("read open fifo");
        atomic_store_explicit(&lock, false, memory_order_release);
        T msg;
        msg.to = msg.from = 0;
        return msg;
    }

    T msg;
    // read header
    while (atomic_exchange_explicit(&lock, true, memory_order_acquire))
        ;
    read (fd, &msg, sizeof(T));
    // read body into memory
    char* buffer = new char[msg.data_size];
    read (fd, buffer, msg.data_size);
    msg.data_ptr = buffer;
    //printf("read pipe %s\n", buffer);

    close (fd);
    atomic_store_explicit(&lock, false, memory_order_release);
    return msg;
};
