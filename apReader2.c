#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

int mapIndex = -1;

struct Map{
    char *key;
    char *value;
};

struct Map2{
    char *key;
    struct Map *values;
    int valueCount;
};

FILE* fileReader(const char *filePath){
    return fopen(filePath,"r");
}

void freeMap2(struct Map2 *m) {
    for (int i = 0; i <= mapIndex; i++) {
        free(m[i].key);
        for (int j = 0; j < m[i].valueCount; j++) {
            free(m[i].values[j].key);
            free(m[i].values[j].value);
        }
        free(m[i].values);
    }
    free(m);
}


int keyReader(char *key, char *buffer){
    int i = 1,index = 0;
    key = (char*)malloc(1 * sizeof(char));
        while(buffer[i] != '|'){
            key = (char*)realloc(key, (index + 2) * sizeof(char)); 
            key[index++] = buffer[i++];
        } 
    key[index] = '\0';
    return i;
}

const char* getValueByKey(struct Map *arr,const char *key){
    int l = sizeof(arr) / sizeof(arr[0]); 
    for(int i = 0; i < l; i++){
        
        if (strcmp(arr[i].key, key) == 0) {
            return arr[i].value;
        }
    }
    return "!Invalid Key!";
}

const char* getValueByMainKey(struct Map2 *arr,const char* mainKey,const char* key){
    for(int i = 0; i <= mapIndex; i++){
        struct Map*temp = arr[i].values;
        if (strcmp(arr[i].key, mainKey) == 0){
            for(int j = 0; j < arr[i].valueCount; j++){
                if (strcmp(temp[j].key, key) == 0) return temp[j].value;
            }
        } 
    }
    return "!Invalid Key!";
}

// #define getValue(...) _getValue(NULL,NULL,NULL,NULL)

// const char* _getValue(struct Map *arr1,const char *key,struct Map2 *arr2,const char* mainKey){
//     if(!arr2 && !mainKey){
//         return getValueByMap(arr1,key);
//     } 
//     if(!arr1){
//         return getValueByMap2(arr2,mainKey,key);
//     }
// }

struct Map* getValuesArray(struct Map2 *arr,const char* mainKey){
    for(int i = 0; i <= mapIndex; i++){
        struct Map*temp = arr[i].values;
        if (strcmp(arr[i].key, mainKey) == 0) return arr[i].values;
    }

    return NULL;
}

void getData(struct Map2 **m,FILE* fileptr){
    char buffer[200];
    while(fgets(buffer, sizeof(buffer),fileptr)){
        if (strlen(buffer) <= 1) continue; // Skip empty lines
        int i = 1,index = 0;
        struct Map2 temp;
        temp.valueCount = 0;
        // reads main key (user1/user2)
        temp.key = (char*)malloc(1 * sizeof(char));
        while(buffer[i] != '|'){
            temp.key = (char*)realloc(temp.key, (index + 2) * sizeof(char)); 
            temp.key[index++] = buffer[i++];
        } 
        temp.key[index] = '\0';
        temp.values = NULL;
        
        while(fgets(buffer, sizeof(buffer), fileptr) && strlen(buffer) > 2){
            if (strlen(buffer) <= 1) break;
            i = 1,index = 0;
            struct Map t;
            //reads key values inside main (name,email,etc)
            t.key = (char*)malloc(1 * sizeof(char));
            while(buffer[i] != '|'){
                t.key = (char*)realloc(t.key, (index + 2) * sizeof(char)); 
                t.key[index++] = buffer[i++];
            }
            t.key[index] = '\0';
            i++; index = 0;//i++ skips '|'

            //reads values
            t.value =  (char*)malloc(1 * sizeof(char));
            while(buffer[i] != '~' && buffer[i] != '\0'){
                t.value = (char*)realloc(t.value, (index + 2) * sizeof(char)); 
                t.value[index++] = buffer[i++];
                
            }
            t.value[index] = '\0';
            temp.valueCount++;
            temp.values = (struct Map*)realloc(temp.values, temp.valueCount * sizeof(struct Map));
            temp.values[temp.valueCount - 1] = t;
        }

        // Add the current Map2 struct to the array
        *m = (struct Map2*)realloc(*m, (mapIndex + 2) * sizeof(struct Map2));
        (*m)[++mapIndex] = temp;
    }
}

void display(struct Map2 *m){
    if(mapIndex == -1) printf("No data");
    else{
        for(int i = 0; i <= mapIndex; i++){
            printf("%s : \n",m[i].key);
            struct Map*temp = m[i].values;
            for(int j = 0; j < m->valueCount; j++){
                printf("%s : %s\n",temp[j].key,temp[j].value);
            }
        }
    } 
}

int main(){
    FILE *fileptr = fileReader("second.ap");
    struct Map2 *map = (struct Map2*)malloc((sizeof(struct Map2))); 
    getData(&map,fileptr);
    display(map);
    //printf("%s\n",getValue(arr2=map,key="name",mainKey="user1"));
    freeMap2(map);  
    fclose(fileptr);
    return 0;
}