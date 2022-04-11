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
}

void cnt_pia_cycles(cycles_t cycles) {
	PIA_CYCLES += cycles;
	cnt_timer(cycles);
}

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
