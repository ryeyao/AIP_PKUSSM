HTTP MP3 STREAM PLAYER
======================
* Find the source in GitHub
    git clone http://github.com/ryeyao/HLIPA_PKUSSM/

Preparation
-----------
* Install libasound2-dev
    sudo apt-get install libasound2-dev

* Install libav (choose either of the two)
    * From source  
            git clone http://github.com/libav/libav  
            cd libav  
            ./configure --prefix=/usr/local/libav --enable-small --disable-yasm --disable-encoders --disable-programs --disable-muxers --disable-filters --disable-bsfs --disable-doc --disable-decoders --enable-decoder=pcm* --enable-decoder=mp3* --enable-shared --disable-static  
            sudo make  
            make install  

    * Using the shared librarys attached(default)  
            Do nothing for now.  

Compile mp3player
-----------------
* If your libav is compiled from source  
        make LIBSDIR=-L/usr/local/libav/lib  
        export LD_LIBRARY_PATH=/usr/local/libav/lib:$LD_LIBRARY_PATH  

* If using the shared librarys attached  
        make  
        export LD_LIBRARY_PATH=libav/lib:$LD_LIBRARY_PATH  

Usage
-----
    ./mp3player [filename]  
    filename:  
        Can be http url or local file path.  
        e.g: ./mp3player http://127.0.0.1/test/dadi.mp3  
             ./mp3player test/dadi.mp3  
