#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Map{
    char *key;
    char *value;
};

FILE* fileWriter(char *filePath){
    FILE * filePtr = fopen(filePath,"a");
    return filePtr;
}

void storeData(struct Map *data,int dataSize, FILE *fileptr){
    if(!data){
        printf("No data found");
        return;
    }

    for(int i = 0; i < dataSize; i++){
        fprintf(fileptr, "\n~%s|%s~",data[i].key,data[i].value);
    }
    fclose(fileptr);
}

int main(){
    struct Map data[2];
    char k1[] = "last name",k2[] = "grade",v1[] = "sharma",v2[] = "12";
    data[0].key = k1;
    data[0].value = v1;
    data[1].key = k2;
    data[1].value = v2;
    char filePath[] = "first.ap";
    FILE *fileptr = fileWriter(filePath);
    storeData(data,2,fileptr);
}
