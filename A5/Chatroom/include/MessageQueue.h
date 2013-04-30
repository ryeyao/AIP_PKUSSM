/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/29
 */

#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <MessagePoll.h>

template <class T>
class MessageQueue: public MessagePoll<T> {
    public:
        MessageQueue ();
        ~MessageQueue ();
        void putMessage (T msg);
        T getMessage ();
        
};

#endif
