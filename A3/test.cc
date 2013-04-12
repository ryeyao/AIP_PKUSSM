#include<iostream>
#include<stdio.h>
#include<strings.h>
#include<stdlib.h>
using namespace std;

typedef enum Test {
    Test1,
    Test2,
    Test3,
    Test4,
    Test5,
    Test6,
    Test7,
    Test8
}Test;

typedef int FP(int, int);

int p(int i, int j) {
    printf("%d + %d = %d\n", i, j, i+j);
}

void doCalculate(FP* p) {
    p(1, 2);
}
int main(int argc, char** argv) {
    Test t;
    cout<<sizeof(Test1)<<endl;
    FP* fp = &p;
    doCalculate(p);

    int size = 4;
    char buff[20];
    bcopy(&size, buff, 4);
    cout<<atoi(buff)<<endl;
}
