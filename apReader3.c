#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

struct Map{
    char *key;
    char *value;
    struct Map *nestedValues;
};

FILE* fileReader(const char *filePath){
    return fopen(filePath,"r");
}

int index = -1;
int dataIndex = 0;

void readLines(char** dataString,int *dataStringIndex,struct Map *arr,int isNested){
    char *data = dataString[*dataStringIndex];
    char ch = data[strlen(data) - 1];

    struct Map temp;
    temp.key = NULL;
    temp.nestedValues = NULL;
    temp.value = NULL;
    static int nestedIndex = 0;
    
    if(ch == '~' && strlen(data) > 1){
        int i = 1,j = 0;
        while(data[i] != '\0' && data[i] != '\n' && data[i] != '~'){
            // Keys
            temp.key = (char*)malloc(1 * sizeof(char));
            while (data[i] != '|') {
                temp.key = (char*)realloc(temp.key, (j + 2) * sizeof(char)); 
                temp.key[j++] = data[i++];
            } 
            temp.key[j] = '\0';

            j = 0;
            i++;//skips '|'

            // Values
            temp.value = (char*)malloc((sizeof(char)));
            while (data[i] != '~' && data[i] != '\0') {
                temp.value = (char*)realloc(temp.value, (j + 2) * sizeof(char));
                temp.value[j++] = data[i++];
            } 
            temp.value[j] = '\0';
            arr = (struct Map*)realloc(arr, (nestedIndex + 1) * sizeof(struct Map));
            arr[nestedIndex++] = temp;
            printf("%s %s\n",temp.key,temp.value);

        }

        if(isNested){
            *dataStringIndex = *dataStringIndex+1;
            readLines(dataString,dataStringIndex,temp.nestedValues,1);
        }
    }
    else if(ch == '~') return;
    else if(ch == '|'){
        int i = 1,j = 0;
        while(data[i] != '\0' && data[i] != '\n' && data[i] != '|'){
            // Keys
            temp.key = (char*)malloc(1 * sizeof(char));
            while (data[i] != '|') {
                temp.key = (char*)realloc(temp.key, (j + 1) * sizeof(char)); 
                temp.key[j++] = data[i++];
            } 
            temp.key[index] = '\0';
        }
        //printf("%s\n",buffer);
        temp.nestedValues = (struct Map*)malloc(1 * sizeof(struct Map));
        printf("%s\n",temp.key);
        *dataStringIndex = *dataStringIndex+1;
        readLines(dataString,dataStringIndex,temp.nestedValues,1);
        arr = (struct Map*)realloc(arr, (index + 2) * sizeof(struct Map));
        arr[++index] = temp;
    }
}

/*void readData(FILE *filePtr,char **dataStringArr){
    char buffer[100];
    
    while(fgets(buffer,sizeof(buffer),filePtr)){
        size_t length = strlen(buffer);
        if (buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0'; 
        }
        char **temp = (char**)realloc(dataStringArr, (dataIndex + 1) * sizeof(char*));
        dataStringArr = temp;
        // Allocate memory for the current string and copy the buffer
        dataStringArr[dataIndex] = (char*)malloc((strlen(buffer) + 1) * sizeof(char)); // +1 for null terminator
        strcpy(dataStringArr[dataIndex], buffer);
        dataIndex++;
    }

    
    //printf(":p %s %s\n",arr[0].key);
    
}*/

int main(){
    FILE *filePtr = fileReader("third.ap");
    struct Map *arr = NULL;
    char **dataStringArr = (char**)malloc(sizeof(char*));
    char buffer[100];
    
    while(fgets(buffer,sizeof(buffer),filePtr)){
        size_t length = strlen(buffer);
        if (buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0'; 
        }
        char **temp = (char**)realloc(dataStringArr, (dataIndex + 1) * sizeof(char*));
        dataStringArr = temp;
        // Allocate memory for the current string and copy the buffer
        dataStringArr[dataIndex] = (char*)malloc((strlen(buffer) + 1) * sizeof(char)); // +1 for null terminator
        strcpy(dataStringArr[dataIndex], buffer);
        dataIndex++;
    }
    //readData(fileptr,dataStringArr);

    int i = 0;
    while(i <= dataIndex){
        char *data = dataStringArr[i];
        char ch = data[strlen(data) - 1];
        int isNested = 0;
        if(ch == '|') isNested = 1;
        readLines(dataStringArr,&i,arr,isNested);
        i++;
    }

    for (i = 0; i < index; i++) {
        free(arr[i].key);
        free(arr[i].value);
    }
    free(arr);
    for (int i = 0; i < dataIndex; i++) {
        free(dataStringArr[i]);
    }
    free(dataStringArr);
    //display(map);
    //printf("%s\n",getValue(arr2=map,key="name",mainKey="user1"));
    //freeMap2(map);  
    fclose(filePtr);
    return 0;
}