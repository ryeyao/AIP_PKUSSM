/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/29
 */

#ifndef MESSAGEPIPE_H
#define MESSAGEPIPE_H

#include <MessagePoll.h>
#include <atomic>
using namespace std;

template <class T>
class MessagePipe: public MessagePoll<T> {

    public:
        //MessagePipe ();
        MessagePipe (char* fifoname);
        ~MessagePipe ();
        void putMessage (T msg);
        T getMessage ();

    private:
        char* fifoname;
        atomic<bool> lock;
};

#endif
