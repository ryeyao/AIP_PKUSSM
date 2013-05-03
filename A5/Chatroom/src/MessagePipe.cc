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
    if (mkfifo (fifoname, 0777) < 0) {
        perror("mkfifo");
    }
    lock = ATOMIC_VAR_INIT(false);
};

template <class T>
MessagePipe<T>::~MessagePipe () {
};

template <class T>
void MessagePipe<T>::putMessage (T msg) {
    while (atomic_exchange_explicit(&lock, true, memory_order_acquire))
        ;

    int fd = 0;
    fd = open (fifoname, O_WRONLY);

    int total_size = sizeof(T) + msg->data_size;
    char* buffer = new char[total_size];

    memcpy (buffer, msg, sizeof(T)); // Write header
    memcpy (buffer + sizeof(T), msg->data_ptr, msg->data_size); // Write body

    write (fd, buffer, total_size); 

    close (fd);
    delete[] buffer;
    atomic_store_explicit(&lock, false, memory_order_release);
};

template <class T>
T MessagePipe<T>::getMessage () {
    while (atomic_exchange_explicit(&lock, true, memory_order_acquire))
        ;
    int fd = 0;
    fd = open (fifoname, O_RDONLY);

    T msg;
    // read header
    read (fd, msg, sizeof(T));
    // read body into memory
    char* buffer = new char[msg->data_size];
    read (fd, buffer, msg->data_size);
    msg->data_ptr = buffer;

    close (fd);
    atomic_store_explicit(&lock, false, memory_order_release);
    return msg;
};
