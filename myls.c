#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

int sort = 0;
int a = 0;
int rows = 0; 
int cols = 0;

/*this is a test block comment
ads
ladfkj
lkafdlkje*/
void do_ls(char[]);

int compstr(const void* a, const void* b){
    const char* aa = *(const char**)a;
    const char* bb = *(const char**)b;
    return strcmp(aa, bb);
}

int main(int ac, char *av[]){

    struct winsize wbuf;
    if(ioctl(0, TIOCGWINSZ, &wbuf) != -1){
        rows = wbuf.ws_row;
        cols = wbuf.ws_col;
    }
    //printf("%d%d\n", rows, cols);

    if(ac == 1){
        do_ls(".");
    }else{
        if(av[1][0] == '-'){
            for(int i = 1; av[1][i] != '\0'; i++){
                if(av[1][i] == 's')
                    sort = 1;
                if(av[1][i] == 'r')
                    sort = 2;
                if(av[1][i] == 'a')
                    a = 1;
            }
            if(ac == 2)
                do_ls(".");
        }
        //*++av;
        //printf("%d\n", sort);
        for(int i = 1 + (sort == 2 | sort == 1 | a); i < ac; i++)
            do_ls(av[i]);
        
    }
}

void do_ls(char dirname[]){
    int largest = 0;
    //printf("%s", dirname);
    DIR *dir_ptr;
    struct dirent *direntp;
    char * dirs[100];

    if((dir_ptr = opendir(dirname)) == NULL){        
        fprintf(stderr, "nmyls: cannot open %s\n", dirname);
    }else{
        int num_ents = 0;
        printf("%s:\n", dirname);
        while((direntp = readdir(dir_ptr)) != NULL){
            dirs[num_ents] = (char*)malloc(strlen(direntp->d_name) * sizeof(char));
            char * name = direntp->d_name;
            if(!a && name[0] == '.' && (strcmp(name, "..") != 0 && strcmp(name, ".") != 0))
                continue;
            

            if(strlen(name) > largest)
                largest = strlen(name);

            strcpy(dirs[num_ents], direntp->d_name);
            //printf("%s\n", direntp -> d_name);
            num_ents++;
        }
        closedir(dir_ptr);
        if(sort != 0){
            qsort(dirs, num_ents, sizeof(char*), compstr);
        }
        int num_cols = cols / (largest + 2);
        if(sort == 2){
            for(int i = num_ents-1; i >= 0; i--){
                printf("%-*s  ", largest, dirs[i]);
                if(i % num_cols == 0 && i + 1 < num_ents)
                    printf("\n");
            }
        }else{
            for(int i = 0; i < num_ents; i++){
                printf("%-*s  ", largest, dirs[i]);
                if(i % num_cols == 0 && i + 1 < num_ents)
                    printf("\n");
            }
        }
        printf("\n");

    }
}