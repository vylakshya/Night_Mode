#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/mman.h>
#include <fcntl.h>     
#include <sys/stat.h>
#include <sys/ioctl.h>  
#include <linux/fb.h>
#include <stdint.h>    


int temp = 0;

void fill_lut(uint8_t* lut, float factor) {
    for (int i = 0; i < 256; i++) {
        lut[i] = (uint8_t)(i * factor);
    }
}

int main(int argc, char *argv[]) { 
    
    if (argc < 2) {
        printf("Usage: %s <blue_percentage_0_to_100>\n", argv[0]);
        return 1;
    }

    char *c = argv[1];
    int i = 0;
    while(c[i] != '\0'){
        temp = 10*temp + (int)(c[i] - '0');
        i++;
    }

    if (temp > 100) temp = 100;
    if (temp < 0) temp = 0;
    
    float red_f = 1.0f;
    float grn_f = 0.9f + (temp * 0.001f);
    float blu_f = (float)temp / 100.0f;   

    printf("Night Mode Strength: %d%% Blue Light\n", temp);

    uint8_t lut_r[256];
    uint8_t lut_g[256];
    uint8_t lut_b[256];
    fill_lut(lut_r, red_f);
    fill_lut(lut_g, grn_f);
    fill_lut(lut_b, blu_f);


    int fb;
 
    fb = open("/dev/fb0", O_RDWR); 
    if (fb == -1) {
        perror("Error: cannot open /dev/fb0");
        return 1;
    }

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    
    if (ioctl(fb, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed info");
        return 1;
    }
    if (ioctl(fb, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable info");
        return 1;
    }

    long int screensize = finfo.smem_len;
    
    uint8_t *framebuffer = (uint8_t*)mmap(0, screensize, 
                                          PROT_READ | PROT_WRITE, 
                                          MAP_SHARED, fb, 0);

    if ((intptr_t)framebuffer == -1) {
        perror("Error: failed to map framebuffer");
        return 1;
    }

    // Pre-calculate byte offsets for color channels
    int r_off = vinfo.red.offset / 8;
    int g_off = vinfo.green.offset / 8;
    int b_off = vinfo.blue.offset / 8;
    
    for (uint32_t y = 0; y < vinfo.yres; y++) {
        
        uint8_t *row_ptr = framebuffer + (y * finfo.line_length);

        for (uint32_t x = 0; x < vinfo.xres; x++) {
            
            uint8_t *pixel = row_ptr + (x * 4);

            pixel[r_off] = lut_r[ pixel[r_off] ];
            pixel[g_off] = lut_g[ pixel[g_off] ];
            pixel[b_off] = lut_b[ pixel[b_off] ];
        }
    }

    // Cleanup
    munmap(framebuffer, screensize);
    close(fb);
    
    return 0;
}
