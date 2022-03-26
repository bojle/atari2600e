#ifndef CPU_H
#define CPU_H

#include "mspace.h"

typedef struct state_t {
	byte_t A;			
	byte_t X;			
	byte_t Y;			
	byte_t S;			
	byte_t P;			
	addr_t PC;
	_Bool P_C;
	_Bool P_Z;
	_Bool P_I;
	_Bool P_D;
	_Bool P_B;
	_Bool P_V;
	_Bool P_N;
} state_t;


void inst_tbl_init();

char *inst_name(byte_t opcode);
byte_t inst_bytes(byte_t opcode);
byte_t inst_cycles(byte_t opcode);
byte_t inst_exec(byte_t opcode);

void record_state(state_t *s);
void disassembler_init();
void disassemble(byte_t opcode, state_t *s);

#endif
