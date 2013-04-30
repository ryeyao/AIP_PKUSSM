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

template <typename T>
MessagePipe<T>::MessagePipe (char* fifoname) {

    if (mkfifo (fifoname, 0777) < 0) {
        perror("mkfifo");
    }
};

template <typename T>
MessagePipe<T>::~MessagePipe () {
};

template <typename T>
void MessagePipe<T>::putMessage (T msg) {
    int fd = 0;
    fd = open (fifoname, O_WRONLY);

    int total_size = sizeof(T) + msg->data_size;
    char* buffer = new char[total_size];

    strncpy (buffer, &msg, sizeof(T)); // Write header
    strncpy (buffer + sizeof(T), msg->data_ptr, msg->data_size); // Write body

    write (fd, buffer, total_size); 

    close (fd);
    delete[] buffer;
};

template <typename T>
T MessagePipe<T>::getMessage () {
    int fd = 0;
    fd = open (fifoname, O_RDONLY);

    T msg;
    // read header
    read (fd, &msg, sizeof(T));
    // read body into memory
    char* buffer = new char[msg->data_size];
    read (fd, buffer, msg->data_size);
    msg->data_ptr = buffer;

    close (fd);
};
