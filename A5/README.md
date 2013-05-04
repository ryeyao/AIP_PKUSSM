USAGE
================

## Chatroom (alpha)
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
> NOTE: For now, you can only use user id to chat with people and for some reasons your id is decided by yourself, that is to say you must ensure the unique of your id, or you may hijack some one else's connection info in the server.   

#### To simply start chatting by type command like this  
    send 2 hi there!  
> the pattern of the command is <send \<id\> \<message to send\>>  

## TraceRoute
### Usage
