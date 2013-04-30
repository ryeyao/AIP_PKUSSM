/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/29
 */

#ifndef MESSAGESHAREDMEMORY_H
#define MESSAGESHAREDMEMORY_H

#include <MessagePoll.h>

template <class T>
class MessageSharedMemory: public MessagePoll<T> {
    public:
        MessageSharedMemory ();
        ~MessageSharedMemory ();
        void putMessage (T msg);
        T getMessage ();
        
};

#endif
