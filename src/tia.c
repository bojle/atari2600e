#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "tia.h"
#include "mspace.h"
#include "log.h"

/*
 * General Structure of the TIA
 *
 * In the VCS, TIA and the CPU ran parallely. As a multi-threaded
 * program would lead to needless complexity, the CPU and the TIA,
 * in this program take 'turns' executing. The CPU executes one 
 * instruction and passes the number of cycles it took executing
 * that instruction to the TIA. As the TIA ran 3x faster than the
 * CPU in the VCS, it gets to do 3x the work that the CPU did during
 * its turn. For example, if an instruction took 4 cycles, the TIA
 * gets to execute 12 (4x3) cycles.
 *
 * TODO: Update this column as you go. Include HBLANK, VBLANK, strobes
 * etc.
 */

#define NSTROBE 10
static int strobe_registers[NSTROBE] = {
	WSYNC,
	RSYNC,
	RESP0,
	RESP1,
	RESM0,
	RESM1,
	RESBL,
	HMOVE,
	HMCLR,
	CXCLR
};


int is_strobe(addr_t reg) {
	for (int i = 0; i < NSTROBE; ++i) {
		if (reg == strobe_registers[i]) {
			return 1;
		}
	}
	return 0;
}

void strobe_dispatch(addr_t reg) {
	switch (reg) {
		case WSYNC:
			break;
		case RSYNC:
			break;
		case RESP0:
			break;
		case RESP1:
			break;
		case RESM0:
			break;
		case RESM1:
			break;
		case RESBL:
			break;
		case HMOVE:
			break;
		case HMCLR:
			break;
		case CXCLR:
			break;
		default:
			log_debug("Not a strobe register: %04x", reg);
			break;
	}
}


static pixel_t frame_buffer[VISIBLE_HEIGHT * VISIBLE_WIDTH];

static int line_i = 0;
static int scanline_i = 0;

#define cal_frame_buffer_i(scanline_i, line_i) \
	((scanline_i * VISIBLE_WIDTH) + line_i)

static int frame_buffer_i = 0;

pixel_t select_pixel() {
	return 0xff;
}

void plonk_pixel() {
	pixel_t p = select_pixel();
	frame_buffer[frame_buffer_i] = p;
	if (line_i + 1 > TOTAL_WIDTH) {
		line_i = 0;
		scanline_i++;
	}
	else {
		line_i++;
	}
	frame_buffer_i = cal_frame_buffer_i(scanline_i, line_i);
}

/* 
 * insert hblank at end of a scanline
 * decide when to plonk pixels
 * call update at the end of a frame
 * reset line_i and scanline_i at the end of a frame
 */

void tia_exec() {
	if (0) {
		//dont plonk
	}
	else {
		plonk_pixel();
	}

	if (scanline_i > VISIBLE_HEIGHT) {
		scanline_i = 0;
	}
	if (frame_buffer_i == VISIBLE_HEIGHT * VISIBLE_WIDTH - 1) {
		display();
	}
}

static SDL_Window *gbl_window;
static SDL_Renderer *gbl_renderer;
static SDL_Texture *gbl_texture;


void tia_init() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	int rv = 0;
	rv = SDL_CreateWindowAndRenderer(VISIBLE_WIDTH, VISIBLE_HEIGHT,
			SDL_WINDOW_RESIZABLE, &gbl_window, &gbl_renderer);
	if (rv != 0) {
		log_fatal("SDL_CreateWindowAndRenderer(): %s", SDL_GetError());
		goto exit;
	}
	SDL_SetRenderDrawColor(gbl_renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(gbl_renderer);
	gbl_texture = SDL_CreateTexture(gbl_renderer, SDL_PIXELFORMAT_UNKNOWN, 
			SDL_TEXTUREACCESS_STREAMING, VISIBLE_WIDTH, VISIBLE_HEIGHT);
	if (!gbl_texture) {
		log_fatal("SDL_CreateTexture(): %s", SDL_GetError());
		goto exit;
	}
	int scale = 8;
	SDL_SetWindowSize(gbl_window, VISIBLE_WIDTH * scale, VISIBLE_HEIGHT * scale);
	log_trace("TIA Init Success");
	return;

exit:
	exit(EXIT_FAILURE);
}

void display() {
	SDL_UpdateTexture(gbl_texture, NULL, frame_buffer, VISIBLE_WIDTH * sizeof(pixel_t));
	SDL_RenderClear(gbl_renderer);
	SDL_RenderCopy(gbl_renderer, gbl_texture, NULL, NULL);
	SDL_RenderPresent(gbl_renderer);
}
