#ifndef TIA_H
#define TIA_H

#include "mspace.h"

typedef uint32_t pixel_t;

#define VISIBLE_HEIGHT 192
#define VSYNC_H 3
#define VBLANK_H 37
#define VOVERSCAN_H 30
#define TOTAL_HEIGHT 262 			// vsync + vblank + vheight + overscan

#define VISIBLE_WIDTH 160			
#define HBLANK_W 68				// HORIZANTAL BLANK
#define TOTAL_WIDTH 228 				// VISIBLE_WIDTH + HORZ_BLANK


int is_strobe(addr_t reg);
void strobe_dispatch(addr_t reg);

void tia_init();
void tia_exec();
void print_framebuf();

void cnt_color_clocks(int inc);
int fetch_color_clocks();

void display();

#endif
