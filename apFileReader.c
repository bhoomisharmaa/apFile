#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Map{
    char *key;
    char *value;
};

int mapIndex = -1;

FILE * fileReader(char filePath[]){
    //first.ap
    /*~name|Bhoomi~
    ~email|bhoomi@gmail.com~
    ~age|17~*/
    FILE *fileptr = fopen(filePath,"r");
    return fileptr;
    
}

void getData(struct Map **m,FILE *fileptr){
    
    char buffer[200];
    struct Map temp;
    while (fgets(buffer, sizeof(buffer), fileptr)) {
        int i = 1,index = 0;

        while(buffer[i] != '\0' && buffer[i] != '\n' && buffer[i] != '~'){
            // Keys
            temp.key = (char*)malloc(1 * sizeof(char));
            while (buffer[i] != '|') {
                temp.key = (char*)realloc(temp.key, (index + 2) * sizeof(char)); 
                temp.key[index++] = buffer[i++];
            } 
            temp.key[index] = '\0';

            index = 0;
            i++;//skips '|'

            // Values
            temp.value = (char*)malloc((sizeof(char)));
            while (buffer[i] != '~' && buffer[i] != '\0') {
                temp.value = (char*)realloc(temp.value, (index + 2) * sizeof(char));
                temp.value[index++] = buffer[i++];
            } 
            temp.value[index] = '\0';

            *m = (struct Map*)realloc(*m, (mapIndex + 2) * sizeof(struct Map));
            (*m)[++mapIndex] = temp;
        }
    }
    fclose(fileptr);
}

void display(struct Map *m){
    if(mapIndex == -1) printf("No data");
    else{
        for(int i = 0; i <= mapIndex; i++){
            printf("%s : %s\n",m[i].key,m[i].value);
        }
    } 
}

int main(){
    char filePath[] = "first.ap";
    FILE *fileptr = fileReader(filePath);
    struct Map *map = (struct Map*)malloc((sizeof(struct Map))); 
    getData(&map,fileptr);
    display(map);
    free(map);
    return 0;
}