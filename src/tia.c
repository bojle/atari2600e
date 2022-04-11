#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "tia.h"
#include "mspace.h"
#include "log.h"
#include "pia.h"

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
 * How Inputs from the keyboard are handled
 *
 * run_pia() calls handle_input() in main() after run_cpu() and run_tia() are 
 * called. All inputs meant for the PIA are handled by pia_process_input(). 
 *
 */

#define NSTROBE 14
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
	CXCLR,
	TIM1T,
	TIM8T,
	TIM64T,
	T1024T
};

static cycles_t COLOR_CLOCKS = 0;

void cnt_color_clocks(cycles_t inc) {
	COLOR_CLOCKS += inc;
}

cycles_t fetch_color_clocks() {
	return COLOR_CLOCKS;
}

int is_strobe(addr_t reg) {
	for (int i = 0; i < NSTROBE; ++i) {
		if (reg == strobe_registers[i]) {
			return 1;
		}
	}
	return 0;
}

void strobe_dispatch(addr_t reg, byte_t b) {
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
		case TIM1T:
			set_timer(b, 1);
			break;
		case TIM8T:
			set_timer(b, 8);
			break;
		case TIM64T:
			set_timer(b, 64);
			break;
		case T1024T:
			set_timer(b, 1024);
			break;
		default:
			/* Following line will never be reached by any code */
			log_debug("Not a strobe register: %04x", reg);
			break;
	}
}

static pixel_t color_map[256];
#define color_assign(i, color) (color_map[i] = color)

void init_color_map() {
	 color_assign(0x00, 0x000000FF);
	 color_assign(0x02, 0x1A1A1AFF);
	 color_assign(0x04, 0x393939FF);
	 color_assign(0x06, 0x5B5B5BFF);
	 color_assign(0x08, 0x7E7E7EFF);
	 color_assign(0x0a, 0xA2A2A2FF);
	 color_assign(0x0c, 0xC7C7C7FF);
	 color_assign(0x0e, 0xEDEDEDFF);
					             
	 color_assign(0x10, 0x190200FF);
	 color_assign(0x12, 0x3A1F00FF);
	 color_assign(0x14, 0x5D4100FF);
	 color_assign(0x16, 0x826400FF);
	 color_assign(0x18, 0xA78800FF);
	 color_assign(0x1a, 0xCCAD00FF);
	 color_assign(0x1c, 0xF2D219FF);
	 color_assign(0x1e, 0xFEFA40FF);
					             
	 color_assign(0x20, 0x370000FF);
	 color_assign(0x22, 0x5E0800FF);
	 color_assign(0x24, 0x832700FF);
	 color_assign(0x26, 0xA94900FF);
	 color_assign(0x28, 0xCF6C00FF);
	 color_assign(0x2a, 0xF58F17FF);
	 color_assign(0x2c, 0xFEB438FF);
	 color_assign(0x2e, 0xFEDF6FFF);
					             
	 color_assign(0x30, 0x470000FF);
	 color_assign(0x32, 0x730000FF);
	 color_assign(0x34, 0x981300FF);
	 color_assign(0x36, 0xBE3216FF);
	 color_assign(0x38, 0xE45335FF);
	 color_assign(0x3a, 0xFE7657FF);
	 color_assign(0x3c, 0xFE9C81FF);
	 color_assign(0x3e, 0xFEC6BBFF);
					             
	 color_assign(0x40, 0x440008FF);
	 color_assign(0x42, 0x6F001FFF);
	 color_assign(0x44, 0x960640FF);
	 color_assign(0x46, 0xBB2462FF);
	 color_assign(0x48, 0xE14585FF);
	 color_assign(0x4a, 0xFE67AAFF);
	 color_assign(0x4c, 0xFE8CD6FF);
	 color_assign(0x4e, 0xFEB7F6FF);
					            
	 color_assign(0x50, 0x2D004AFF);
	 color_assign(0x52, 0x570067FF);
	 color_assign(0x54, 0x7D058CFF);
	 color_assign(0x56, 0xA122B1FF);
	 color_assign(0x58, 0xC743D7FF);
	 color_assign(0x5a, 0xED65FEFF);
	 color_assign(0x5c, 0xFE8AF6FF);
	 color_assign(0x5e, 0xFEB5F7FF);
					             
	 color_assign(0x60, 0x0D0082FF);
	 color_assign(0x62, 0x3300A2FF);
	 color_assign(0x64, 0x550FC9FF);
	 color_assign(0x66, 0x782DF0FF);
	 color_assign(0x68, 0x9C4EFEFF);
	 color_assign(0x6a, 0xC372FEFF);
	 color_assign(0x6c, 0xEB98FEFF);
	 color_assign(0x6e, 0xFEC0F9FF);
					             
	 color_assign(0x70, 0x000091FF);
	 color_assign(0x72, 0x0A05BDFF);
	 color_assign(0x74, 0x2822E4FF);
	 color_assign(0x76, 0x4842FEFF);
	 color_assign(0x78, 0x6B64FEFF);
	 color_assign(0x7a, 0x908AFEFF);
	 color_assign(0x7c, 0xB7B0FEFF);
	 color_assign(0x7e, 0xDFD8FEFF);
					             
	 color_assign(0x80, 0x000072FF);
	 color_assign(0x82, 0x001CABFF);
	 color_assign(0x84, 0x033CD6FF);
	 color_assign(0x86, 0x205EFDFF);
	 color_assign(0x88, 0x4081FEFF);
	 color_assign(0x8a, 0x64A6FEFF);
	 color_assign(0x8c, 0x89CEFEFF);
	 color_assign(0x8e, 0xB0F6FEFF);
					             
	 color_assign(0x90, 0x00103AFF);
	 color_assign(0x92, 0x00316EFF);
	 color_assign(0x94, 0x0055A2FF);
	 color_assign(0x96, 0x0579C8FF);
	 color_assign(0x98, 0x239DEEFF);
	 color_assign(0x9a, 0x44C2FEFF);
	 color_assign(0x9c, 0x68E9FEFF);
	 color_assign(0x9e, 0x8FFEFEFF);
					             
	 color_assign(0xa0, 0x001F02FF);
	 color_assign(0xa2, 0x004326FF);
	 color_assign(0xa4, 0x006957FF);
	 color_assign(0xa6, 0x008D7AFF);
	 color_assign(0xa8, 0x1BB19EFF);
	 color_assign(0xaa, 0x3BD7C3FF);
	 color_assign(0xac, 0x5DFEE9FF);
	 color_assign(0xae, 0x86FEFEFF);
					             
	 color_assign(0xb0, 0x002403FF);
	 color_assign(0xb2, 0x004A05FF);
	 color_assign(0xb4, 0x00700CFF);
	 color_assign(0xb6, 0x09952BFF);
	 color_assign(0xb8, 0x28BA4CFF);
	 color_assign(0xba, 0x49E06EFF);
	 color_assign(0xbc, 0x6CFE92FF);
	 color_assign(0xbe, 0x97FEB5FF);
					             
	 color_assign(0xc0, 0x002102FF);
	 color_assign(0xc2, 0x004604FF);
	 color_assign(0xc4, 0x086B00FF);
	 color_assign(0xc6, 0x289000FF);
	 color_assign(0xc8, 0x49B509FF);
	 color_assign(0xca, 0x6BDB28FF);
	 color_assign(0xcc, 0x8FFE49FF);
	 color_assign(0xce, 0xBBFE69FF);
					             
	 color_assign(0xd0, 0x001501FF);
	 color_assign(0xd2, 0x103600FF);
	 color_assign(0xd4, 0x305900FF);
	 color_assign(0xd6, 0x537E00FF);
	 color_assign(0xd8, 0x76A300FF);
	 color_assign(0xda, 0x9AC800FF);
	 color_assign(0xdc, 0xBFEE1EFF);
	 color_assign(0xde, 0xE8FE3EFF);
					             
	 color_assign(0xe0, 0x1A0200FF);
	 color_assign(0xe2, 0x3B1F00FF);
	 color_assign(0xe4, 0x5E4100FF);
	 color_assign(0xe6, 0x836400FF);
	 color_assign(0xe8, 0xA88800FF);
	 color_assign(0xea, 0xCEAD00FF);
	 color_assign(0xec, 0xF4D218FF);
	 color_assign(0xee, 0xFEFA40FF);
					             
	 color_assign(0xf0, 0x380000FF);
	 color_assign(0xf2, 0x5F0800FF);
	 color_assign(0xf4, 0x842700FF);
	 color_assign(0xf6, 0xAA4900FF);
	 color_assign(0xf8, 0xD06B00FF);
	 color_assign(0xfa, 0xF68F18FF);
	 color_assign(0xfc, 0xFEB439FF);
	 color_assign(0xfe, 0xFEDF70FF);
	log_trace("Initialized Color Map");
}


int is_vsync_on() {
	byte_t a = fetch_byte(VSYNC);
	return ((a & 0x02) >> 1);
}

int is_vblank_on() {
	byte_t a = fetch_byte(VBLANK);
	return ((a & 0x02) >> 1);
}

static pixel_t frame_buffer[VISIBLE_HEIGHT * VISIBLE_WIDTH];

pixel_t select_pixel() {
	return color_map[0x0e];
}


/* Pointers
 * hi - horizontal index
 * vi - vertical index
 * chi - horizontal index in the visible region
 * cvi - vertical index in the visible region
 *
 * ti - total index
 * cti - total index in the visible region
 */

static unsigned int hi = 0;
static unsigned int vi = 0;
static unsigned int chi = 0;
static unsigned int cvi = 0;

static unsigned int ti = 0;
static unsigned int cti = 0;

#define cal_total_index(h, v) ((v * TOTAL_WIDTH) + h)
#define cal_total_cindex(h, v) ((v * VISIBLE_WIDTH) + h)

/* If we are on the screen right now */
int isonscreen() {
	if (( (hi > HBLANK_W) && (vi > VBLANK_H + VSYNC_H) ) &&
		( (hi < TOTAL_WIDTH) && (vi < VBLANK_H + VSYNC_H + VISIBLE_HEIGHT) ) && 
		( (!is_vsync_on()) && (!is_vblank_on()) )) {
		return 1;
	}
	return 0;
}

/* responsible for handling cti */
void place_pixel() {
	pixel_t p = select_pixel();
	frame_buffer[cti] = p;

	chi++;
	if (chi > VISIBLE_WIDTH) {
		chi = 0;
		cvi++;
	}

	if (cvi > VISIBLE_HEIGHT) {
		cvi = 0;
	}
	cti = cal_total_cindex(chi, cvi);
}

/* 
 * insert hblank at end of a scanline
 * decide when to place pixels
 * call update at the end of a frame
 * reset line_i and scanline_i at the end of a frame
 */

/* responsible for boundary matching and handling ti*/

void tia_exec() {
	if (isonscreen()) {
		place_pixel();
		if (cti == VISIBLE_HEIGHT * VISIBLE_WIDTH - 1) {
			display();
		}
	}
	hi++;
	if (hi > TOTAL_WIDTH) {
		hi = 0;
		vi++;
	}
	if (vi > TOTAL_HEIGHT) {
		vi = 0;
	}
	ti = cal_total_index(hi, vi);

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
	gbl_texture = SDL_CreateTexture(gbl_renderer, SDL_PIXELFORMAT_RGBA8888, 
			SDL_TEXTUREACCESS_STREAMING, VISIBLE_WIDTH, VISIBLE_HEIGHT);
	if (!gbl_texture) {
		log_fatal("SDL_CreateTexture(): %s", SDL_GetError());
		goto exit;
	}
	int scale = 8;
	SDL_SetWindowSize(gbl_window, VISIBLE_WIDTH * scale, VISIBLE_HEIGHT * scale);

	init_color_map();

	log_trace("TIA Init Success");
	return;

exit:
	exit(EXIT_FAILURE);
}

void tia_free() {
	SDL_Quit();
	log_trace("%s", "Freed TIA");
}

void display() {
	SDL_UpdateTexture(gbl_texture, NULL, frame_buffer, VISIBLE_WIDTH * sizeof(pixel_t));
	SDL_RenderClear(gbl_renderer);
	SDL_RenderCopy(gbl_renderer, gbl_texture, NULL, NULL);
	SDL_RenderPresent(gbl_renderer);
}

static SDL_Event gbl_event;
#define iskey(e, code) (e.key.keysym.scancode == code)

static void process_input(int code) {
	switch (code) {
		case SDL_SCANCODE_Q:
		case SDL_SCANCODE_ESCAPE:
			exit(EXIT_SUCCESS);
			break;
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_S:
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_LEFT:
		case SDL_SCANCODE_RIGHT:
		case SDL_SCANCODE_UP:
		case SDL_SCANCODE_DOWN:
			pia_process_input(code);
			break;
		default:
			break;
	}
}

void handle_input() {
	while (SDL_PollEvent(&gbl_event) != 0) {
			if (gbl_event.type == SDL_QUIT) {
				exit(EXIT_SUCCESS);
			}
			else {
				process_input(gbl_event.key.keysym.scancode);
			}
	}
}
