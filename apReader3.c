#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

struct Map{
    char *key;
    char *value;
    struct Map *nestedValues;
    int nestedValuesIndex; //Index to keep track of nested values
};

FILE* fileReader(const char *filePath){
    return fopen(filePath,"r");
}

void processDataStructures(char** dataString,int *dataStringIndex,struct Map **arr,int isNested,int *index,struct Map* stack,int *stackIndex){
    char *data = dataString[*dataStringIndex];
    char ch = data[strlen(data) - 1];

    struct Map temp;//Temporary struct to process data
    temp.key = NULL;
    temp.nestedValues = NULL;
    temp.value = NULL;
    temp.nestedValuesIndex = 0;
    
    if(ch == '~' && strlen(data) > 1){ //key-value pair
        int i = 1,j = 0;
        while(data[i] != '\0' && data[i] != '\n' && data[i] != '~'){
            //Processing key
            temp.key = (char*)malloc(sizeof(char));
            while (data[i] != '|') {
                temp.key = (char*)realloc(temp.key, (j + 2) * sizeof(char)); 
                temp.key[j++] = data[i++];
            } 
            temp.key[j] = '\0';
            j = 0;
            i++;//Skip the '|' separator

            //Processing value
            temp.value = (char*)malloc((sizeof(char)));
            while (data[i] != '~' && data[i] != '\0') {
                temp.value = (char*)realloc(temp.value, (j + 2) * sizeof(char)); 
                temp.value[j++] = data[i++];
            } 
            temp.value[j] = '\0';
            *arr = (struct Map*)realloc(*arr, ((*index) + 1) * sizeof(struct Map));
            (*arr)[*index] = temp;
            (*index)++;
        }
        
        if(isNested){
            *dataStringIndex = *(dataStringIndex)+1;
            processDataStructures(dataString,dataStringIndex,arr,1,index,stack,stackIndex);
        } 
    }
    else if(ch == '|'){ //For key-nestedValue pair
        int i = 1,j = 0;
        
        //Processing key
        temp.key = (char*)malloc(100 * sizeof(char));
        while (data[i] != '|') {
            temp.key[j++] = data[i++];
        } 
        temp.key[j] = '\0';
        temp.nestedValues = (struct Map*)malloc(sizeof(struct Map));
        (*dataStringIndex)++;

        //stores the current structure in stack
        (*stackIndex)++;
        stack[*stackIndex] = temp;
        //Process the nested structure
        processDataStructures(dataString,dataStringIndex,&temp.nestedValues,1,&temp.nestedValuesIndex,stack,stackIndex);
        (*stackIndex)--;// Pop from stack when done

        //Add the processed nested structure to the array
        if(*stackIndex >= 0){
            *arr = (struct Map*)realloc(*arr, ((*index) + 1) * sizeof(struct Map));
            (*arr)[*index] = temp;
            (*index)++;
            (*dataStringIndex)++;
            processDataStructures(dataString,dataStringIndex,arr,1,index,stack,stackIndex);
        }else{
            *arr = (struct Map*)realloc(*arr, ((*index) + 1) * sizeof(struct Map));
            (*arr)[*index] = temp;
            (*index)++;
        }
        
    }
}

//Function to read data from the file into an array of strings
void readFileIntoArray(FILE *filePtr, char ***dataStringArr, int *dataIndex,int *numberOfNestedData) {
    char buffer[100];

    while (fgets(buffer, sizeof(buffer), filePtr)) {
        size_t length = strlen(buffer);
        if (buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0'; 
        }

        //Reallocate memory for the array of strings
        char **temp = (char**)realloc(*dataStringArr, (*dataIndex + 1) * sizeof(char*));
        if (temp == NULL) {
            //Handle memory allocation failure
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        *dataStringArr = temp;
        

        // Allocate memory for the current string and copy the buffer
        (*dataStringArr)[*dataIndex] = (char*)malloc((strlen(buffer) + 1) * sizeof(char)); //+1 for null terminator
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

//Function to display the processed data structures
void displayData(struct Map *arr,int indexRange){
    for(int i = 0; i < indexRange; i++){
        if(arr[i].value){
            printf("%s : %s\n",arr[i].key,arr[i].value);//Print key-value pair
        }
        else{
            //Print key-nestedValue pair
            printf("%s {\n",arr[i].key);
            displayData(arr[i].nestedValues,arr[i].nestedValuesIndex);
            printf("}\n");
        }
    }
}

int main(){
    FILE *filePtr = fileReader("third.ap");
    struct Map *arr = (struct Map*)malloc(sizeof(struct Map));
    char **dataStringArr = (char**)malloc(sizeof(char*));
    char buffer[100];
    int dataIndex = 0,numberOfNestedData = 0;
    
    readFileIntoArray(filePtr,&dataStringArr,&dataIndex,&numberOfNestedData);

    struct Map *stack = (struct Map*)malloc(sizeof(struct Map)*numberOfNestedData);
    int stackIndex = -1;
    int i = 0,index = 0;

    //Process each line in the data string array
    while(i < dataIndex){
        char *data = dataStringArr[i];
        char ch = data[strlen(data) - 1];
        int isNested = ch == '|' ? 1 : 0;// Check if the line is part of a nested structure
        processDataStructures(dataStringArr,&i,&arr,isNested,&index,stack,&stackIndex);
        i++;
    }

    //Displaying processed data
    displayData(arr,index);

    //Clean up memory allocations
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