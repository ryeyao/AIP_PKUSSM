#include <iostream>
#include <csignal>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

const string currentDateTime() {
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return asctime(timeinfo);
}

void sigHandler(int signum) {
    if (signum == SIGUSR1) {
        cout << currentDateTime() << endl;
    }
}

void sigHandler2(int signum) {
    if (signum == SIGUSR2) {
        cout << "exit" << endl;
        exit(0);
    }
}

void childProcess(pid_t ppid) {
    int max_count = 5;
    while (max_count--) {
        sleep(1);
        raise(SIGUSR1);
    }
    raise(SIGUSR2);
    exit(0);
    cout <<"hhhh"<<endl;
}

int main(void){

    signal(SIGUSR1, sigHandler);

    pid_t pid = fork();
    pid_t ppid = getpid();

    if (pid < 0) {
        cout << "error!" <<endl;
    } else if (pid == 0) {
        childProcess(ppid);
    } else {

    }

    return 0;
}
