USAGE
================
Project Page on github: http://github.com/ryeyao/HLIPA_PKUSSM
## Chatroom (alpha)
Based on the code of file server/client in assignment 3.
### Usage
#### First compile the proj  
    make  
#### Start the server  
    cd bin/  
    sudo ./Server -p 8888  
#### Start a client on another machine with server's ip and port specified  
    cd bin/  
    sudo ./Client <ipaddress> <port>  

#### Then you should follow the instruction displayed to type your nick name and userid.  
> NOTE: For now, you can only use user id to chat with people and for some reasons your id is decided by yourself, that is to say you must ensure your id to be unique, or you may hijack someone else's connection info in the server.   

#### To simply start chatting by type command like this  
    send 2 Hi there!  
> the pattern of the command is send <'id[1~255]'> <'message to send'>  

#### TODO  
* Fix bugs while transmitting socket descriptors between processes through shared memory using struct msghdr
* Fix messy characters while display messages that are received

#### Features to be support
* Support sending message using user's nick name
* Support UUID
* Support file transmitting between two user
* Support voice chatting
* Support video chatting

#### Send to all
    send 0 Hi all!

## TraceRoute
### Usage
    sudo ./traceroute <ip|host>
