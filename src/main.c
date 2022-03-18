#include <stdio.h>
#include <stdint.h>
#include "add.c"
#include <SDL2/SDL.h>


#ifndef DEBUG_MODE
#define DEBUG_MODE 1
#endif

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window;
	SDL_Renderer *rend;
	if (SDL_CreateWindowAndRenderer(500, 500, 0, &window, &rend) != 0) {
		printf("Coudnt create a window/rend: %s", SDL_GetError());
		return 1;
	}
	SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(rend);
	SDL_Texture *tex = SDL_CreateTexture(rend, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 500, 500);
	if (!tex) {
		printf("no texture %s", SDL_GetError());
		return 1;
	}
	uint32_t *pixels = (uint32_t *) malloc(500 * 500 * sizeof(uint32_t));
	for (int i = 0; i < 500 * 500 * sizeof(uint32_t); ++i) {
			pixels[i] = 0xffffffff;
	}
	SDL_UpdateTexture(tex, NULL, pixels, 500 * sizeof(uint32_t));
	if (!pixels) {
		printf("malloc failed");
		return 1;
	}
	SDL_RenderClear(rend);
	SDL_RenderCopy(rend, tex, NULL, NULL);
	SDL_RenderPresent(rend);
	SDL_Delay(5000);
	SDL_Quit();
}
