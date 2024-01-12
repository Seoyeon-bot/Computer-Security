#include<stdlib.h>
#include<stdio.h>

void main(){
    char* shell = getenv("MYSHELL");  // MYSHELL =/bin/sh is our environmental variable. 
    if(shell)
        printf("MYSHELL address + %x\n", (unsigned int)shell); 

    // this is for task4 
    char* shell2 = getenv("MYSHEL2"); // MYSHEL2 =-p 
    if(shell2)
         printf("MYSHEL2 address + %x\n", (unsigned int)shell2); 
}
// for task3 prtenv.c vs retlib.c
