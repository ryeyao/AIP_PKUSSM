#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <map>
using namespace std;

int main () {
    map<int, int> map_ptr;
    map<int, int> *pm;
    map_ptr[0] = 1;
    pm = &map_ptr;
    (*pm)[(*(&map_ptr)).begin()->first] = 2;


    printf ("%d\n", (*pm).begin()->second);
}
