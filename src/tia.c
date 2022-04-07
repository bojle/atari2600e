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

static unsigned int COLOR_CLOCKS = 0;

void cnt_color_clocks(int inc) {
	COLOR_CLOCKS += inc;
}

int fetch_color_clocks() {
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

static pixel_t color_map[256];
#define color_assign(i, color) (color_map[i] = color)

void init_color_map() {
	 color_assign(0x00, 0x000000);
	 color_assign(0x02, 0x1A1A1A);
	 color_assign(0x04, 0x393939);
	 color_assign(0x06, 0x5B5B5B);
	 color_assign(0x08, 0x7E7E7E);
	 color_assign(0x0a, 0xA2A2A2);
	 color_assign(0x0c, 0xC7C7C7);
	 color_assign(0x0e, 0xEDEDED);
					             
	 color_assign(0x10, 0x190200);
	 color_assign(0x12, 0x3A1F00);
	 color_assign(0x14, 0x5D4100);
	 color_assign(0x16, 0x826400);
	 color_assign(0x18, 0xA78800);
	 color_assign(0x1a, 0xCCAD00);
	 color_assign(0x1c, 0xF2D219);
	 color_assign(0x1e, 0xFEFA40);
					             
	 color_assign(0x20, 0x370000);
	 color_assign(0x22, 0x5E0800);
	 color_assign(0x24, 0x832700);
	 color_assign(0x26, 0xA94900);
	 color_assign(0x28, 0xCF6C00);
	 color_assign(0x2a, 0xF58F17);
	 color_assign(0x2c, 0xFEB438);
	 color_assign(0x2e, 0xFEDF6F);
					             
	 color_assign(0x30, 0x470000);
	 color_assign(0x32, 0x730000);
	 color_assign(0x34, 0x981300);
	 color_assign(0x36, 0xBE3216);
	 color_assign(0x38, 0xE45335);
	 color_assign(0x3a, 0xFE7657);
	 color_assign(0x3c, 0xFE9C81);
	 color_assign(0x3e, 0xFEC6BB);
					             
	 color_assign(0x40, 0x440008);
	 color_assign(0x42, 0x6F001F);
	 color_assign(0x44, 0x960640);
	 color_assign(0x46, 0xBB2462);
	 color_assign(0x48, 0xE14585);
	 color_assign(0x4a, 0xFE67AA);
	 color_assign(0x4c, 0xFE8CD6);
	 color_assign(0x4e, 0xFEB7F6);
					             
	 color_assign(0x50, 0x2D004A);
	 color_assign(0x52, 0x570067);
	 color_assign(0x54, 0x7D058C);
	 color_assign(0x56, 0xA122B1);
	 color_assign(0x58, 0xC743D7);
	 color_assign(0x5a, 0xED65FE);
	 color_assign(0x5c, 0xFE8AF6);
	 color_assign(0x5e, 0xFEB5F7);
					             
	 color_assign(0x60, 0x0D0082);
	 color_assign(0x62, 0x3300A2);
	 color_assign(0x64, 0x550FC9);
	 color_assign(0x66, 0x782DF0);
	 color_assign(0x68, 0x9C4EFE);
	 color_assign(0x6a, 0xC372FE);
	 color_assign(0x6c, 0xEB98FE);
	 color_assign(0x6e, 0xFEC0F9);
					             
	 color_assign(0x70, 0x000091);
	 color_assign(0x72, 0x0A05BD);
	 color_assign(0x74, 0x2822E4);
	 color_assign(0x76, 0x4842FE);
	 color_assign(0x78, 0x6B64FE);
	 color_assign(0x7a, 0x908AFE);
	 color_assign(0x7c, 0xB7B0FE);
	 color_assign(0x7e, 0xDFD8FE);
					             
	 color_assign(0x80, 0x000072);
	 color_assign(0x82, 0x001CAB);
	 color_assign(0x84, 0x033CD6);
	 color_assign(0x86, 0x205EFD);
	 color_assign(0x88, 0x4081FE);
	 color_assign(0x8a, 0x64A6FE);
	 color_assign(0x8c, 0x89CEFE);
	 color_assign(0x8e, 0xB0F6FE);
					             
	 color_assign(0x90, 0x00103A);
	 color_assign(0x92, 0x00316E);
	 color_assign(0x94, 0x0055A2);
	 color_assign(0x96, 0x0579C8);
	 color_assign(0x98, 0x239DEE);
	 color_assign(0x9a, 0x44C2FE);
	 color_assign(0x9c, 0x68E9FE);
	 color_assign(0x9e, 0x8FFEFE);
					             
	 color_assign(0xa0, 0x001F02);
	 color_assign(0xa2, 0x004326);
	 color_assign(0xa4, 0x006957);
	 color_assign(0xa6, 0x008D7A);
	 color_assign(0xa8, 0x1BB19E);
	 color_assign(0xaa, 0x3BD7C3);
	 color_assign(0xac, 0x5DFEE9);
	 color_assign(0xae, 0x86FEFE);
					             
	 color_assign(0xb0, 0x002403);
	 color_assign(0xb2, 0x004A05);
	 color_assign(0xb4, 0x00700C);
	 color_assign(0xb6, 0x09952B);
	 color_assign(0xb8, 0x28BA4C);
	 color_assign(0xba, 0x49E06E);
	 color_assign(0xbc, 0x6CFE92);
	 color_assign(0xbe, 0x97FEB5);
					             
	 color_assign(0xc0, 0x002102);
	 color_assign(0xc2, 0x004604);
	 color_assign(0xc4, 0x086B00);
	 color_assign(0xc6, 0x289000);
	 color_assign(0xc8, 0x49B509);
	 color_assign(0xca, 0x6BDB28);
	 color_assign(0xcc, 0x8FFE49);
	 color_assign(0xce, 0xBBFE69);
					             
	 color_assign(0xd0, 0x001501);
	 color_assign(0xd2, 0x103600);
	 color_assign(0xd4, 0x305900);
	 color_assign(0xd6, 0x537E00);
	 color_assign(0xd8, 0x76A300);
	 color_assign(0xda, 0x9AC800);
	 color_assign(0xdc, 0xBFEE1E);
	 color_assign(0xde, 0xE8FE3E);
					             
	 color_assign(0xe0, 0x1A0200);
	 color_assign(0xe2, 0x3B1F00);
	 color_assign(0xe4, 0x5E4100);
	 color_assign(0xe6, 0x836400);
	 color_assign(0xe8, 0xA88800);
	 color_assign(0xea, 0xCEAD00);
	 color_assign(0xec, 0xF4D218);
	 color_assign(0xee, 0xFEFA40);
					             
	 color_assign(0xf0, 0x380000);
	 color_assign(0xf2, 0x5F0800);
	 color_assign(0xf4, 0x842700);
	 color_assign(0xf6, 0xAA4900);
	 color_assign(0xf8, 0xD06B00);
	 color_assign(0xfa, 0xF68F18);
	 color_assign(0xfc, 0xFEB439);
	 color_assign(0xfe, 0xFEDF70);
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
	return color_map[0x86];
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

void display() {
	SDL_UpdateTexture(gbl_texture, NULL, frame_buffer, VISIBLE_WIDTH * sizeof(pixel_t));
	SDL_RenderClear(gbl_renderer);
	SDL_RenderCopy(gbl_renderer, gbl_texture, NULL, NULL);
	SDL_RenderPresent(gbl_renderer);
}
