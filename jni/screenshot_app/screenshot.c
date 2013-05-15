#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include <linux/fb.h>

#include <zlib.h>
#include <libpng/png.h>

#include "private/android_filesystem_config.h"

#define LOG_TAG "screenshot"
#include <utils/Log.h>


int shot_save(const char* file);


int shot_save(const char* filename) {
    FILE *png = NULL;
    FILE *fb_in = NULL;
    char outfile[PATH_MAX] = "";

    int do_increment = 0;

    strlcpy(outfile, filename, PATH_MAX);
    if (do_increment) {
        struct stat st;
        char base[PATH_MAX] = "";
        int i = 0;
        while (stat(outfile, &st) == 0) {
            if (!base[0]) {
                char *p = strrchr(outfile, '.');
                if (p) *p = '\0';
                strcpy(base, outfile);
            }
            snprintf(outfile, PATH_MAX, "%s-%d.png", base, ++i);
        }
    }

    fb_in = fopen("/dev/graphics/fb0", "r");
    if (!fb_in) {
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG,
				"error: could not read framebuffer\n");
        exit(1);
    }

    /* switch to non-root user and group */
    gid_t groups[] = { AID_LOG, AID_SDCARD_RW };
    setgroups(sizeof(groups)/sizeof(groups[0]), groups);
    setuid(AID_SHELL);

    png = fopen(outfile, "w");
    if (!png) {
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG,"error: writing file %s: %s\n",
                outfile, strerror(errno));
        exit(1);
    }

    take_screenshot(fb_in, png);

    exit(0);
}

void take_screenshot(FILE *fb_in, FILE *fb_out) {
    int fb;
    char imgbuf[0x10000];
    struct fb_var_screeninfo vinfo;
    png_structp png;
    png_infop info;
    unsigned int r,c,rowlen;
    unsigned int bytespp,offset;

    fb = fileno(fb_in);
    if(fb < 0) {
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG,"failed to open framebuffer\n");
        return;
    }
    fb_in = fdopen(fb, "r");

    if(ioctl(fb, FBIOGET_VSCREENINFO, &vinfo) < 0) {
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG,"failed to get framebuffer info\n");
        return;
    }
    fcntl(fb, F_SETFD, FD_CLOEXEC);

    png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png == NULL) {
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG,"failed png_create_write_struct\n");
        fclose(fb_in);
        return;
    }

    png_init_io(png, fb_out);
    info = png_create_info_struct(png);
    if (info == NULL) {
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG,"failed png_create_info_struct\n");
        png_destroy_write_struct(&png, NULL);
        fclose(fb_in);
        return;
    }
    if (setjmp(png_jmpbuf(png))) {
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG,"failed png setjmp\n");
        png_destroy_write_struct(&png, NULL);
        fclose(fb_in);
        return;
    }

    bytespp = vinfo.bits_per_pixel / 8;
    png_set_IHDR(png, info,
        vinfo.xres, vinfo.yres, vinfo.bits_per_pixel / 4, 
        PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png, info);

    rowlen=vinfo.xres * bytespp;
    if (rowlen > sizeof(imgbuf)) {
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG,"crazy rowlen: %d\n", rowlen);
        png_destroy_write_struct(&png, NULL);
        fclose(fb_in);
        return;
    }

    offset = vinfo.xoffset * bytespp + vinfo.xres * vinfo.yoffset * bytespp;
    fseek(fb_in, offset, SEEK_SET);

    for(r=0; r<vinfo.yres; r++) {
        int len = fread(imgbuf, 1, rowlen, fb_in);
        if (len <= 0) break;
        png_write_row(png, (png_bytep)imgbuf);
    }

    png_write_end(png, info);
    fclose(fb_in);
    png_destroy_write_struct(&png, NULL);
}
