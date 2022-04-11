#include <SDL2/SDL.h>
#include "mspace.h"
#include "pia.h"

void pia_process_input(int code) {
	int tmp = 0;
	switch (code) {
		case SDL_SCANCODE_W:
			set_byte(SWCHA, 0xff);
			tmp = fetch_byte(SWCHA);
			clear_bit(tmp, 4);
			set_byte(SWCHA, tmp);
			break;
		case SDL_SCANCODE_A:
			set_byte(SWCHA, 0xff);
			tmp = fetch_byte(SWCHA);
			clear_bit(tmp, 6);
			set_byte(SWCHA, tmp);
			break;
		case SDL_SCANCODE_S:
			set_byte(SWCHA, 0xff);
			tmp = fetch_byte(SWCHA);
			clear_bit(tmp, 5);
			set_byte(SWCHA, tmp);
			break;
		case SDL_SCANCODE_D:
			set_byte(SWCHA, 0xff);
			tmp = fetch_byte(SWCHA);
			clear_bit(tmp, 7);
			set_byte(SWCHA, tmp);
			break;
		case SDL_SCANCODE_UP:
			set_byte(SWCHA, 0xff);
			tmp = fetch_byte(SWCHA);
			clear_bit(tmp, 0);
			set_byte(SWCHA, tmp);
			break;
		case SDL_SCANCODE_LEFT:
			set_byte(SWCHA, 0xff);
			tmp = fetch_byte(SWCHA);
			clear_bit(tmp, 2);
			set_byte(SWCHA, tmp);
			break;
		case SDL_SCANCODE_DOWN:
			set_byte(SWCHA, 0xff);
			tmp = fetch_byte(SWCHA);
			clear_bit(tmp, 1);
			set_byte(SWCHA, tmp);
			break;
		case SDL_SCANCODE_RIGHT:
			set_byte(SWCHA, 0xff);
			tmp = fetch_byte(SWCHA);
			clear_bit(tmp, 3);
			set_byte(SWCHA, tmp);
			break;
		default:
			break;
	}
	printf("%02x\n", fetch_byte(SWCHA));
}
