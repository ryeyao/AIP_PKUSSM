Simple FTP Server & Client
==========================
# Usage
    1. Server
        You can specify the port like this
            ./Server -p 8888
        Or you can just let the program chooses port randomly by typing the following command
            ./Server

    2. Client
        You should specify the remote server's IP address and port
            ./Client \[ip\] \[port\]
        Then you can see a command line interface, the *PUT* and *GET* method are supported, eg.
            ftp> put firefox
        or
            ftp> get firefox
        the file *firefox* is in tests/Client/

# Getting Started
    Simply do as the following:
        cd tests/Server
        ./Server -p 8888
    Then in *another terminal*:
        cd ../tests/Client
        ./Client 127.0.0.1 8888
        ftp> put firefox
        ftp> get firefox
    
