#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    char* fn = "/tmp/XYZ";
    char buffer[60];
    FILE* fp;

    // task5 setuid least previlage 
  //  uid_t realUID = getuid();
  //  uid_t effUID = geteuid();

    /* get user input */
    scanf("%50s", buffer);
    // right before opening the file, program should drop set uid previlage by setting 
    //euid = ruid.
  //  setuid(realUID); // set with real user id - least previlage. - disable root previlage

    if (!access(fn, W_OK)) { // check access permission.
        // add sleep for 4 task2 : launching race condition attack 
        //sleep(10); 
         fp = fopen(fn, "a+");
        if (!fp) {
            perror("Open failed");
            exit(1);
        }
        fwrite("\n", sizeof(char), 1, fp);
        fwrite(buffer, sizeof(char), strlen(buffer), fp);
        fclose(fp);
    } else {
        printf("No permission \n");
     //   setuid(effUID);  // set with effective user id 
        // restore previlage by setting euid = root.
      }

    return 0;
}
              

