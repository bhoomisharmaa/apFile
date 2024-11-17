#include <stdio.h>
#include <stdlib.h>

struct List{
    char *key;
    char *value;
    struct List *next;
    struct NestedList *nestedNext;
}*firstList = NULL;

struct NestedList{
    char *key;
    struct List *value;
    struct NestedList *nestedNext;
    struct List *next;
}*firstNested = NULL;

FILE* fileReader(const char *filePath){
    return fopen(filePath,"r");
}

void readData(FILE *filePtr){
    char buffer[100];
    int flag = 1;
    while(fgets(buffer,sizeof(buffer),filePtr)){
        char ch = buffer[strlen(buffer) - 1];
        if(flag){
            if(ch == '|') firstNested = (struct NestedList*)malloc(sizeof(struct NestedList));
            else firstList = (struct List*)malloc(sizeof(struct List));
            flag = 0;
            continue;
        }
        
        

    }
}