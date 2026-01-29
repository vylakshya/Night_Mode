#include<stdio.h>
#include<unistd.h>
#include<sys/mman.h>
#include<fnctl.h>
#include<sys/stat.h>
#include<linux/fb.h>


int main(int argc, char *argv[]){
    
    int framebuffer;
    mode_t permissions;
    int pixelsRead;
    int flags;
    permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_WGRP | S_IWOTH | S_IROTH;
    
    framebuffer = open("dev/fb0", S_WRONLY, permissions); 

    return 0;
}
