#include <SDL2/SDL.h>
#include "mspace.h"
#include "pia.h"

static cycles_t PIA_CYCLES = 0;

static cycles_t timer_cycles = 0;
static uint32_t timer_number = 1;

void set_timer(byte_t intervals, uint32_t number) {
	set_byte(INTIM, intervals);
	timer_number = number;
	timer_cycles = intervals * number;
}

void cnt_timer(cycles_t cycles) {
	byte_t interval = fetch_byte(INTIM);
	for (int i = 0; i < cycles; ++i) {
		if (interval == 0) {
			return;
		}
		timer_cycles--;
		if ((timer_cycles % timer_number) == 0) {
			interval--;
			set_byte(INTIM, interval);
		}
	}
#if 0
	if (interval <= 0) {
		return;
	}
	if ((scycles_t)timer_cycles - (scycles_t)cycles < 0) {
		goto dec_interval;
	}
	timer_cycles -= cycles;
	if ((timer_cycles % timer_number) == 0) {
		goto dec_interval;
	}
	return;

dec_interval:
		interval--;
		if (interval == 0) {
			set_STATUS(STATUS_Z);
		}
	set_byte(INTIM, interval);
#endif
}

void cnt_pia_cycles(cycles_t cycles) {
	PIA_CYCLES += cycles;
	cnt_timer(cycles);
}

void pia_process_input(int code) {
	int tmp = 0;
	set_byte(SWCHA, 0xff);
	tmp = fetch_byte(SWCHA);
	switch (code) {
		case SDL_SCANCODE_W:
			clear_bit(tmp, 4);
			break;
		case SDL_SCANCODE_A:
			clear_bit(tmp, 6);
			break;
		case SDL_SCANCODE_S:
			clear_bit(tmp, 5);
			break;
		case SDL_SCANCODE_D:
			clear_bit(tmp, 7);
			break;
		case SDL_SCANCODE_UP:
			clear_bit(tmp, 0);
			break;
		case SDL_SCANCODE_LEFT:
			clear_bit(tmp, 2);
			break;
		case SDL_SCANCODE_DOWN:
			clear_bit(tmp, 1);
			break;
		case SDL_SCANCODE_RIGHT:
			clear_bit(tmp, 3);
			break;
		default:
			break;
	}
	set_byte(SWCHA, tmp);
	printf("%02x\n", fetch_byte(SWCHA));
}
