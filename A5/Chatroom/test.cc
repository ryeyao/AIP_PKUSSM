#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

int main () {
    char a[] = "1234567890";
    char c[] = "1234";
    char *b = new char[15];
    strncpy (b, a, 10);
    strncpy (b + 10, c, 5); 

    printf("%s", b);
    delete[] b;
}
