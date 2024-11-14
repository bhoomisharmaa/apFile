#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Map{
    char *key;
    char *value;
};

struct Map2{
    char *key;
    struct Map *values;
    int valueCount;
};

FILE* fileAppender(char* filePath){
    return fopen(filePath,"a");
}

/*[[user3,[[name,bhoomi],[grade,12]]],[user4,[[name,bhoomi],[grade,12]]]]*/