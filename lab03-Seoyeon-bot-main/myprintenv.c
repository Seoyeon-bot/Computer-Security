#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

extern char **environ; // global environmental variable. 

void printenv()   // print out environ variable. 
{
  int i = 0;
  while (environ[i] != NULL) {
     printf("%s\n", environ[i]);
     i++;
  }
}

int main()   // main method. 
{
  pid_t childPid;      // init childprocess 
  switch(childPid = fork()) {      // duplicate, create child process
    case 0:  /* child process */
      //printenv();          
      exit(0);
    default:  /* parent process */
      printenv();       
      exit(0);
  }
}
