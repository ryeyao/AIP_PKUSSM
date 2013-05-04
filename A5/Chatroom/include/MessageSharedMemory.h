/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/29
 */

#ifndef MESSAGESHAREDMEMORY_H
#define MESSAGESHAREDMEMORY_H

#include <MessagePoll.h>
#include <atomic>
using namespace std;

template <class T>
class MessageSharedMemory: public MessagePoll<T> {
    public:
        MessageSharedMemory (char* ipc_name);
        ~MessageSharedMemory ();
        void putMessage (T msg);
        T getMessage ();

    private:
        key_t key;
        atomic<bool> lock;
        struct msghdr write_fd (void *ptr, size_t nbytes, int sendfd);
        void read_fd (struct msghdr* fdhdr, void *ptr, size_t nbytes, int *recvfd);
        
};

#endif
