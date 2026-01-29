#include<stdio.h>
#include<unistd.h>
#include<sys/mman.h>
#include<fnctl.h>
#include<sys/stat.h>
#include<linux/fb.h>


int temp = 0;

int main(int argc, char *argv[]){
    
    char *c = argv[1];
    int i = 0;
    while(c[i] != '\0'){

        temp = 10*temp + (int)(c[i] - '0');
        i++;
    }
    
    int framebuffer;
    mode_t permissions;
    int pixelsRead;
    int flags;
    permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_WGRP | S_IWOTH | S_IROTH;
    
    framebuffer = open("dev/fb0", S_WRONLY, permissions); 

    return 0;
}
