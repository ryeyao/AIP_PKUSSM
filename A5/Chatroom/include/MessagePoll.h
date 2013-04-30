/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/29
 */
#ifndef MESSAGEPOLL_H
#define MESSAGEPOLL_H

template <class T>
class MessagePoll {
    public:
        MessagePoll ();
        virtual ~MessagePoll ();
        virtual void putMessage (T msg, char* data_buf) = 0;
        virtual T getMessage () = 0;

};

#endif
