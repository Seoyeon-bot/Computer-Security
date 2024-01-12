#include<stdlib.h>
#include<stdio.h>

void main(){
    char* shell = getenv("MYSHELL");  // MYSHELL =/bin/sh is our environmental variable. 
    if(shell)
        printf("MYSHELL address + %x\n", (unsigned int)shell); 

    
}
// for task3 prtenv.c vs retlib.c