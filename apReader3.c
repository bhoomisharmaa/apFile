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

void processDataStructures(char** dataString,int *dataStringIndex,struct Map **arr,int isNested,int index,char* stack,int *stackIndex){
    char *data = dataString[*dataStringIndex];
    char ch = data[strlen(data) - 1];

    struct Map temp;
    temp.key = NULL;
    temp.nestedValues = NULL;
    temp.value = NULL;
    
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
            *arr = (struct Map*)realloc(*arr, (index + 1) * sizeof(struct Map));
            (*arr)[index] = temp;
        }

        if(isNested){
            *dataStringIndex = *dataStringIndex+1;
            processDataStructures(dataString,dataStringIndex,arr,1,index+1,stack,stackIndex);
        }  
    }
    else if(ch == '~'){ 
        if(*stackIndex > 0){
            *dataStringIndex = *dataStringIndex+1;
            stack[*stackIndex] = ' ';
            *stackIndex = *stackIndex-1;
            processDataStructures(dataString,dataStringIndex,arr,1,index+1,stack,stackIndex);
        }
        else return;
    } 
    else if(ch == '|'){
        int i = 1,j = 0;
        *stackIndex = *stackIndex+1;
        stack[*stackIndex] = '|';
        
        while(data[i] != '\0' && data[i] != '\n' && data[i] != '|'){
            // Keys
            temp.key = (char*)malloc(1 * sizeof(char));
            while (data[i] != '|') {
                temp.key = (char*)realloc(temp.key, (j + 1) * sizeof(char)); 
                temp.key[j++] = data[i++];
            } 
            temp.key[j] = '\0';
        }
        temp.nestedValues = (struct Map*)malloc(1 * sizeof(struct Map));
        *arr = (struct Map*)realloc(*arr, (index + 1) * sizeof(struct Map));
        *dataStringIndex = *dataStringIndex+1;
        processDataStructures(dataString,dataStringIndex,&temp.nestedValues,1,0,stack,stackIndex);
        (*arr)[index] = temp;
    }
}

void readFileIntoArray(FILE *filePtr, char ***dataStringArr, int *dataIndex) {
    char buffer[100];

    while (fgets(buffer, sizeof(buffer), filePtr)) {
        size_t length = strlen(buffer);
        if (buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0'; 
        }

        // Reallocate memory for the array of strings
        char **temp = (char**)realloc(*dataStringArr, (*dataIndex + 1) * sizeof(char*));
        if (temp == NULL) {
            // Handle memory allocation failure
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        *dataStringArr = temp;

        // Allocate memory for the current string and copy the buffer
        (*dataStringArr)[*dataIndex] = (char*)malloc((strlen(buffer) + 1) * sizeof(char)); // +1 for null terminator
        if ((*dataStringArr)[*dataIndex] == NULL) {
            // Handle memory allocation failure
            fprintf(stderr, "Memory allocation for string failed\n");
            exit(1);
        }
        strcpy((*dataStringArr)[*dataIndex], buffer);

        (*dataIndex)++;
    }
}

void displayData(struct Map *arr){
    for(int i = 0; i < 2; i++){
        if(arr[i].value){
            printf("%s : %s\n",arr[i].key,arr[i].value);
        }
    }
}

int main(){
    FILE *filePtr = fileReader("third.ap");
    struct Map *arr = NULL;
    char **dataStringArr = (char**)malloc(sizeof(char*));
    char buffer[100];
    int dataIndex = 0;
    
    readFileIntoArray(filePtr,&dataStringArr,&dataIndex);
    char *stack = (char*)malloc(sizeof(char)*dataIndex);
    int stackIndex = -1;
    int i = 0,index = 0;
    while(i < dataIndex){
        char *data = dataStringArr[i];
        char ch = data[strlen(data) - 1];
        int isNested = 0;
        if(ch == '|') isNested = 1;
        processDataStructures(dataStringArr,&i,&arr,isNested,index,stack,&stackIndex);
        i++;
        index++;
    }

    for (i = 0; i < index; i++) {
        free(arr[i].key);
        free(arr[i].value);
        free(arr[i].nestedValues);
    }
    free(arr);
    for (int i = 0; i <= dataIndex; i++) {
        free(dataStringArr[i]);
    }
    free(dataStringArr);
    fclose(filePtr);
    return 0;
}