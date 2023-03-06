#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFERSIZE 4096

void cat(char *str, int e, int b, int n, int s);

int main(int argc, char* argv[]){
    int b = 0;
    int e = 0;
    int n = 0;
    int s = 0;

    if(argc < 2){
        printf("%s\n", "to few args");
        return 1;
    }
    if(argv[1][0] == '-'){
        
        for(int i = 1; argv[1][i] != '\0'; i++){
            if(argv[1][i] == 'b')
                b = 1;
            if(argv[1][i] == 'e')
                e = 1;
            if(argv[1][i] == 'n')
                n = 1;
            if(argv[1][i] == 's')
                s = 1;
                
        }
    }
    for(int i = 1 + (b | e | n | s); i < argc; i++){
        cat(argv[i], e, b, n, s);
    }
}

void cat(char *str, int e, int b, int n, int s){
    int in_fd, in_count;
    char buf[BUFFERSIZE] = {'\0'};
    if((in_fd = open(str, O_RDONLY)) == -1){
        perror("Cannot open file ");
        exit(1);
    }
    static int line = 1;
    if(b || n){
        printf("%d ", line);
        line++;
    }
    while((in_count = read(in_fd, buf, BUFFERSIZE)) > 0){
        int i = 0;
        while(i < in_count){
            if(s){
                if(buf[i] == '\n'){
                    int j = i;
                    while(((int)buf[j] <= 32 || (int)buf[j] >= 127) && j < in_count){
                        if(buf[j] == '\n'){
                            i = j;
                        }
                        j++;
                    }
                }
            }
            if(n || (b && s) || e){
                while(buf[i] == '\n' && i < in_count){
                    if(e){
                        if(!n && !b)
                            printf("$\n");
                        else
                            printf("$\n%d ", line);
                        line++;
                        i++;
                    }else{
                        printf("\n%d ", line);
                        line++;
                        i++;
                    }
                }
            }else if(b){
                if(buf[i] == '\n'){
                    int j = i;
                    while(((int)buf[j] <= 32 || (int)buf[j] >= 127) && j < in_count){
                        if(buf[j] == '\n'){
                            printf("\n");
                            i = j;
                        }
                        j++;
                    }
                    if(i < in_count - 1){
                        printf("%d ", line);    
                        line++;
                        i++;  
                    }       
                }
            }
            printf("%c", buf[i]);
            i++;
        }
    }
    
    if(e)
        printf("$\n");
    else 
        printf("\n");

    if(close(in_fd) == -1){
        perror("error closing file");
        exit(1);
    }
}
