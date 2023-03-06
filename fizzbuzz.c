#include <stdio.h>
#include <stdlib.h>

int get_in();

void do_fizz(int limit);

int main(){
    do_fizz(get_in());
    return 0;
}

int get_in(){
    int in;
    printf("enter an integer: ");
    int valid = scanf("%d", &in);
    if(valid != 1){
        printf("not an int\n");
    }else if(in <= 0){
        printf("not a valid number\n");
    }
    return in;
}

void do_fizz(int limit){
    for(int i = 1; i <= limit; i++){
        int fizz = i % 3;
        int buzz = i % 5;

        if(!fizz) 
            printf("fizz");
        if(!buzz) 
            printf("buzz");
        if(fizz && buzz) 
            printf("%d", i);
            
        printf("\n");
    

    }
}

