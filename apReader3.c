#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

struct Map{
    char *key;
    char *value;
    struct Map *nestedValues;
    int nestedValuesIndex;
};

FILE* fileReader(const char *filePath){
    return fopen(filePath,"r");
}

void processDataStructures(char** dataString,int *dataStringIndex,struct Map *arr,int isNested,int *index,struct Map* stack,int *stackIndex){
    char *data = dataString[*dataStringIndex];
    char ch = data[strlen(data) - 1];

    struct Map temp;
    temp.key = NULL;
    temp.nestedValues = NULL;
    temp.value = NULL;
    temp.nestedValuesIndex = 0;
    
    if(ch == '~' && strlen(data) > 1){
        
        int i = 1,j = 0;
        while(data[i] != '\0' && data[i] != '\n' && data[i] != '~'){
            // Keys
            temp.key = (char*)malloc(100 * sizeof(char));
            while (data[i] != '|') {
                temp.key[j++] = data[i++];
            } 
            temp.key[j] = '\0';

            j = 0;
            i++;//skips '|'

            // Values
            temp.value = (char*)malloc(100*(sizeof(char)));
            while (data[i] != '~' && data[i] != '\0') {
                temp.value[j++] = data[i++];
            } 
            temp.value[j] = '\0';
            arr[*index] = temp;
            //printf("%d %s\n",*index,arr[*index].key);
            (*index)++;
        }
        
        if(isNested){
            *dataStringIndex = *(dataStringIndex)+1;
            processDataStructures(dataString,dataStringIndex,stack[*stackIndex].nestedValues,1,index,stack,stackIndex);
        } 
    }
    else if(ch == '~'){
        (*stackIndex)--;
        if(*stackIndex >= 0){
            (*dataStringIndex)++;
            processDataStructures(dataString,dataStringIndex,stack[*stackIndex].nestedValues,1,&(stack[*stackIndex].nestedValuesIndex),stack,stackIndex);
        }
        return;
    } 
    else if(ch == '|'){
        int i = 1,j = 0;
        
            // Keys
            temp.key = (char*)malloc(100 * sizeof(char));
            while (data[i] != '|') {
                temp.key[j++] = data[i++];
            } 
            temp.key[j] = '\0';
        temp.nestedValues = (struct Map*)malloc(10 * sizeof(struct Map));
        (*dataStringIndex)++;
        (*stackIndex)++;
        stack[*stackIndex] = temp;
        processDataStructures(dataString,dataStringIndex,temp.nestedValues,1,&temp.nestedValuesIndex,stack,stackIndex);
        arr[*index] = temp;
        //printf("%d %s\n",*index,arr[*index].key);
        (*index)++;
    }
}

void readFileIntoArray(FILE *filePtr, char ***dataStringArr, int *dataIndex,int *numberOfNestedData) {
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
        if(strlen(buffer) <= 2) *(numberOfNestedData) = (*numberOfNestedData) + 1;

        (*dataIndex)++;
    }
}

void displayData(struct Map *arr,int indexRange){
    for(int i = 0; i < indexRange; i++){
        if(arr[i].value){
            printf("%s : %s\n",arr[i].key,arr[i].value);
        }
        else{
            printf("%s {\n",arr[i].key);
            displayData(arr[i].nestedValues,arr[i].nestedValuesIndex);
            printf("}\n");
        }
    }
}

int main(){
    FILE *filePtr = fileReader("third.ap");
    struct Map *arr = (struct Map*)malloc(sizeof(struct Map)*10);
    char **dataStringArr = (char**)malloc(sizeof(char*));
    char buffer[100];
    int dataIndex = 0,numberOfNestedData = 0;
    
    readFileIntoArray(filePtr,&dataStringArr,&dataIndex,&numberOfNestedData);
    struct Map *stack = (struct Map*)malloc(sizeof(struct Map)*numberOfNestedData);
    int stackIndex = -1;
    int i = 0,index = 0;
    while(i < dataIndex){
        char *data = dataStringArr[i];
        char ch = data[strlen(data) - 1];
        int isNested = 0;
        if(ch == '|') isNested = 1;
        processDataStructures(dataStringArr,&i,arr,isNested,&index,stack,&stackIndex);
        //printf("%s\n",arr[0].key);
        i++;
        //index++;
    }

    displayData(arr,index);

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