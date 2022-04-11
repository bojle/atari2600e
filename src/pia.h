#ifndef PIA_H
#define PIA_H

#include <stdint.h>

void pia_process_input(int code);
void cnt_pia_cycles(uint32_t cycles);
void set_timer(byte_t intervals, uint32_t number);

#endif
